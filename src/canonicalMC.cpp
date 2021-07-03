/****************************************************************
 implementation of functions for NVT monte carlo simulation
*****************************************************************/

#include "canonicalMC.h"

#include <cstdlib>

#include "linalg.h"

canonicalMC::canonicalMC(vector<string> files, int *seed, bool restart)
    : Experiment{files, seed, true} {
  _restart = restart;
  initialize(restart);
}

// setup

void canonicalMC::initialize(bool restart) {
  // needs more work
  extract_names();
  read_input();
  reset_histogram();
  calculate_tail_corrections();
  if (restart) {
    read_positions("old_" + check_phase());
  } else {
    read_positions("config.0");
    starting_message();
  }
  _accepted = 0;
  _attempted = 0;
}

void canonicalMC::extract_names() {
  // very not efficient way of removing eventual suf/pre-fixes from the names
  // but it should be alright, since the list is very short and the operation
  // is performed just once
  vector<string> observables{"potential", "pressure", "radial_dist",
                             "avg_radial_dist"};
  for (string s : _observables) {
    for (string obs : observables) {
      if (is_substring(obs, s)) _names[obs] = s;
    }
  }
}

void canonicalMC::read_input() {
  // open input file analizing the names passed to the constructor
  ifstream in("input/input." + check_phase());

  in >> _system.npart;
  in >> _system.density;
  in >> _system.temperature;
  in >> _system.rcut;
  in >> _delta;
  in >> _nbins;

  in.close();

  _system.volume = double(_system.npart) / _system.density;
  _system.side = pow(_system.volume, 1. / 3.);
}

string canonicalMC::check_phase() const {
  string file = _observables[0];
  vector<string> phases{"gas", "liq", "sol"};
  string phase = "dat";
  for (string p : phases) {
    if (is_substring(p, file)) phase = p;
  }
  return phase;
}

void canonicalMC::reset_histogram() {
  _g.resize(0);
  _g.resize(_nbins, 0);
}

void canonicalMC::read_positions(string file) {
  cout << endl << "Reading initial configuration from file " + file << endl;
  ifstream in("config/" + file);
  double x, y, z;
  array<double, 3> p;
  for (int i = 0; !in.eof(); i++) {
    in >> x >> y >> z;
    p = {x, y, z};
    p *= _system.side;
    if (i < _system.positions.size()) {
      _system.positions[i] = p;
      _system.new_positions[i] = p;
    } else {
      _system.positions.push_back(p);
      _system.new_positions.push_back(p);
    }
  }
  in.close();
  _system.npart = _system.positions.size();
}

void canonicalMC::calculate_tail_corrections() {
  double prefactor = 8. / 3. * M_PI * _system.density;
  double fraction = 1. / pow(_system.rcut, 3);
  _system.vtail = prefactor * fraction * (pow(fraction, 2) / 3. - 1.);
  _system.wtail = 4. * prefactor * fraction * (pow(fraction, 2) / 3. - 0.5);
}

// simulation

void canonicalMC::run() {
  for (int j = 0; j < _system.npart; ++j) move(random_index());
}

void canonicalMC::move(int i) {
  array<double, 3> new_pos = _random.uniform(_system.positions[i], _delta);
  _system.new_positions[i] = periodic_bc(new_pos);
  bool new_positions = true;
  double dE = energy(i, new_positions) - energy(i);
  if (_random.Rannyu() <= boltzmann_weight(dE)) {
    _system.positions[i] = _system.new_positions[i];
    ++_accepted;
  }
  ++_attempted;
}

double canonicalMC::energy(int index, bool new_position) {
  array<double, 3> pi;
  array<double, 3> p0 =
      (new_position) ? _system.new_positions[index] : _system.positions[index];
  double e = 0;
  double dr;
  for (int i = 0; i < _system.npart; ++i) {
    if (i != index) {
      pi = _system.positions[i];
      // distance index-i in pbc
      dr = norm(periodic_bc(pi - p0));
      if (dr < _system.rcut) {
        e += 1.0 / pow(dr, 12) - 1.0 / pow(dr, 6);
      }
    }
  }
  return 4 * e;
}

void canonicalMC::calculate_g(int runs, int nblocks,
                              map<string, ofstream> &out) {
  int block_length = runs / nblocks;
  for (int block = 0; block < nblocks; ++block) {
    reset_histogram();
    for (int i = 0; i < block_length; ++i) {
      run();  // MC step
      fill_g();
    }
    average_g(block_length);
    output_g(out);
    _progress->write((block + 1) / double(nblocks));
  }
}

void canonicalMC::average_g(double block_length) {
  for (int i = 0; i < _nbins; ++i) {
    _g[i] /= block_length;
  }
}

// measurement

void canonicalMC::measure() {
  string potential = _names["potential"], P = _names["pressure"];
  estimate(potential) += potential_energy() / _system.npart + _system.vtail;
  estimate(P) += pressure();
}

double canonicalMC::potential_energy() const {
  double v = 0, vij, dr, fraction;
  array<double, 3> pi, pj;
  // cycle over (unique) pairs of positions
  for (int i = 0; i < _system.npart - 1; ++i) {
    pi = _system.positions[i];
    for (int j = i + 1; j < _system.npart; ++j) {
      pj = _system.positions[j];
      dr = norm(periodic_bc(pi - pj));
      if (dr < _system.rcut) {
        fraction = 1. / pow(dr, 6);
        vij = fraction * (fraction - 1);
        v += vij;
      }
    }
  }
  return 4.0 * v;
}

double canonicalMC::pressure() const {
  return _system.density * _system.temperature +
         (virial() + _system.wtail * _system.npart) / _system.volume;
}

double canonicalMC::virial() const {
  double w = 0, wij, dr, fraction;
  array<double, 3> pi, pj;
  // cycle over (unique) pairs of positions
  for (int i = 0; i < _system.npart - 1; ++i) {
    pi = _system.positions[i];
    for (int j = i + 1; j < _system.npart; ++j) {
      pj = _system.positions[j];
      dr = norm(periodic_bc(pi - pj));
      if (dr < _system.rcut) {
        fraction = 1. / pow(dr, 6);
        wij = fraction * (fraction - 0.5);
        w += wij;
      }
    }
  }
  return 48.0 * w / 3.0;
}

void canonicalMC::fill_g() {
  double r, dV, normalization;
  double dr = 0.5 * _system.side / _nbins;
  array<double, 3> pi, pj;
  for (int i = 0; i < _system.npart - 1; ++i) {
    pi = _system.positions[i];
    for (int j = i + 1; j < _system.npart; ++j) {
      pj = _system.positions[j];
      r = norm(periodic_bc(pi - pj));
      dV = 4. / 3. * M_PI * (pow(r + dr, 3) - pow(r, 3));
      normalization = _system.density * _system.npart * dV;
      _g[int(r / dr)] += 2 / normalization;
    }
  }
}

// results

void canonicalMC::final_output(map<string, ofstream> &out) {
  // NOTE: maybe overload the function in Experiment with no output argument
  ofstream out0("config/old_" + check_phase());
  cout << endl
       << "Writing final configuration to file old_" + check_phase() << endl;
  array<double, 3> pos;
  for (int i = 0; i < _system.npart; ++i) {
    pos = _system.positions[i] / _system.side;
    out0 << pos[0] << " " << pos[1] << " " << pos[2];
    if (i < _system.npart - 1) out0 << endl;
  }
  out0.close();
  cout << "acceptance = " << double(_accepted) / _attempted << endl;
}

void canonicalMC::output_g(map<string, ofstream> &out) {
  for (string s : _observables) {
    for (int i = 0; i < _nbins; ++i) {
      out[s] << _g[i] << ' ';
    }
    out[s] << endl;
  }
}

void canonicalMC::starting_message() {
  cout << "Classic Lennard-Jones fluid        " << endl;
  cout << "Monte Carlo simulation             " << endl << endl;
  cout << "Interatomic potential v(r) = 4 * [(1/r)^12 - (1/r)^6]" << endl
       << endl;
  cout << "Boltzmann weight exp(- beta * sum_{i<j} v(r_ij) ), beta = 1/T "
       << endl
       << endl;

  cout << "The program uses Lennard-Jones units " << endl;
  cout << "Temperature = " << _system.temperature << endl;
  cout << "Number of particles = " << _system.npart << endl;
  cout << "Density of particles = " << _system.density << endl;
  cout << "Volume of the simulation box = " << _system.volume << endl;
  cout << "Edge of the simulation box = " << _system.side << endl;
  cout << "Cutoff of the interatomic potential = " << _system.rcut << endl
       << endl;
  cout << "Tail correction for the potential energy = " << _system.vtail
       << endl;
  cout << "Tail correction for the virial           = " << _system.wtail
       << endl;

  cout << "The program performs Metropolis moves with uniform translations"
       << endl;
  cout << "Moves parameter = " << _delta << endl;

  // Print initial values for the potential energy and virial
  cout << "Initial potential energy (with tail corrections) = "
       << potential_energy() / _system.npart + _system.vtail << endl;
  cout << "Virial                   (with tail corrections) = "
       << virial() / _system.npart + _system.wtail << endl;
  cout << "Pressure                 (with tail corrections) = " << pressure()
       << endl
       << endl;
  measure();
}

// boundary conditions

double canonicalMC::periodic_bc(double x) const {
  // keep x within a box of side `_system.side` using periodic boundary
  // conditions
  return x - _system.side * rint(x / _system.side);
}

array<double, 3> canonicalMC::periodic_bc(const array<double, 3> &x) const {
  // keep x within a box of side `_system.side` using periodic boundary
  // conditions
  array<double, 3> x_new;
  for (int i = 0; i < 3; ++i) x_new[i] = periodic_bc(x[i]);
  return x_new;
}