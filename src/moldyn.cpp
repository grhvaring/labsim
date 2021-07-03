/****************************************************************
 implementation of functions for molecular dynamics simulation
*****************************************************************/

#include "moldyn.h"

#include <cstdlib>

#include "linalg.h"

MolecularDynamics::MolecularDynamics(vector<string> files, int *seed,
                                     bool restart)
    : Experiment{files, seed, true} {
  _simulation.restart = restart;
  initialize(restart);
}

// setup

void MolecularDynamics::initialize(bool restart) {
  extract_names();
  read_input();
  if (restart) {
    read_old_positions();
    move_all();
  } else {
    starting_message();
    read_positions();
    set_random_velocities();
  }
  rescale_velocities();  // match the wanted temperature
  guess_old_positions();
  reset_histogram();
}

void MolecularDynamics::extract_names() {
  // very not efficient way of removing eventual suf/prefixes from the names
  // but it should be alright, since the list is very short and the operation
  // is performed just once
  vector<string> observables{"potential", "kinetic", "total", "temperature",
                             "pressure"};
  for (string s : _observables) {
    for (string obs : observables) {
      if (is_substring(obs, s)) _names[obs] = s;
    }
  }
}

void MolecularDynamics::read_input() {
  // open input file analizing the names passed to the constructor
  ifstream in("input/input." + check_phase());

  in >> _simulation.nsteps;
  in >> _simulation.measure_every;  // measure quantities only when needed
  in >> _simulation.dt;
  in >> _system.npart;
  in >> _system.density;
  in >> _system.temperature;
  in >> _system.rcut;

  in.close();

  _system.volume = double(_system.npart) / _system.density;
  _system.side = pow(_system.volume, 1. / 3.);
}

string MolecularDynamics::check_phase() const {
  string file = _observables[0];
  vector<string> phases{"gas", "liq", "sol"};
  string phase = "dat";
  for (string p : phases) {
    if (is_substring(p, file)) phase = p;
  }
  return phase;
}

void MolecularDynamics::reset_histogram(int nbins) {
  _nbins = nbins;
  _g.resize(0);
  _g.resize(_nbins, 0);
}

void MolecularDynamics::read_positions(string file) {
  cout << endl << "Reading initial configuration from file " + file << endl;
  ifstream in("config/" + file);
  double x, y, z;
  Particle p;
  for (int i = 0; !in.eof(); i++) {
    in >> x >> y >> z;
    p.r = {x, y, z};
    p.r *= _system.side;
    if (i < _system.particles.size())
      _system.particles[i] = p;
    else
      _system.particles.push_back(p);
  }
  in.close();
  _system.npart = _system.particles.size();
}

void MolecularDynamics::read_old_positions() {
  // read current positions (and initialize particles)
  read_positions("old_" + check_phase() + ".1");
  string file = "old_" + check_phase() + ".0";
  cout << "Reading old configuration from file " + file << endl;
  ifstream in("config/" + file);
  double x, y, z;
  for (int i = 0; !in.eof(); i++) {
    in >> x >> y >> z;
    _system.particles[i].r_old = {x, y, z};
    _system.particles[i].r_old *= _system.side;
  }
  in.close();
}

void MolecularDynamics::set_random_velocities() {
  cout << "Preparing random velocities with v_cm = 0" << endl;
  srand(1);
  for (Particle &p : _system.particles) {
    for (int k = 0; k < 3; k++) p.v[k] = rand() / double(RAND_MAX) - 0.5;
    // assume that any phase of matter is an ideal gas
    // random.gauss(0, sqrt(_system.temperature));
    // 0.5 * random.Rannyu(-1, 1);
  }
  // compensate for possible drift velocity of the center of mass
  array<double, 3> v_cm = velocity_cm();
  for (Particle &p : _system.particles) p.v -= v_cm;
}

array<double, 3> MolecularDynamics::velocity_cm() const {
  // calculate velocity of the center of mass
  array<double, 3> v_cm = {0., 0., 0.};
  for (Particle p : _system.particles) v_cm += p.v;
  return v_cm / double(_system.npart);
}

void MolecularDynamics::rescale_velocities() {
  cout << "Rescaling velocities to match T* = " << _system.temperature;
  double scaling_factor = sqrt(_system.temperature / temperature());
  cout << "  (scaling factor: " << scaling_factor << ")" << endl;
  for (Particle &p : _system.particles) p.v *= scaling_factor;
}

void MolecularDynamics::guess_old_positions() {
  for (Particle &p : _system.particles)
    p.r_old = periodic_bc(p.r - p.v * _simulation.dt);
}

double MolecularDynamics::temperature() const {
  return 2. / 3. * kinetic_energy() / _system.npart;
}

void MolecularDynamics::calculate_tail_corrections() {
  double prefactor = 8. / 3. * M_PI * _system.density;
  double fraction = 1. / pow(_system.rcut, 3);
  _system.vtail = prefactor * fraction * (pow(fraction, 2) / 3. - 1.);
  _system.wtail = 4. * prefactor * fraction * (pow(fraction, 2) / 3. - 0.5);
}

// simulation

void MolecularDynamics::run() {
  for (int step = 0; step < _simulation.measure_every; ++step) move_all();
}

void MolecularDynamics::move_all() {
  calculate_forces();  // calculate forces to apply before moving the particles
  for (int i = 0; i < _system.npart; ++i) move(i);
}

void MolecularDynamics::calculate_forces() {
  array<double, 3> rij, rj, ri, fij;
  double dr;
  reset_forces();
  // cycle over all pairs of particles
  for (int i = 0; i < _system.npart; ++i) {
    ri = _system.particles[i].r;
    // only once per pair
    for (int j = i + 1; j < _system.npart; ++j) {
      if (j != i) {
        rj = _system.particles[j].r;
        rij = periodic_bc(ri - rj);  // vector joining i-j in pbc
        dr = norm(rij);              // distance i-j in pbc
        if (dr < _system.rcut) {
          fij = rij * (48.0 / pow(dr, 14) - 24.0 / pow(dr, 8));
          _forces[i] += fij;
          _forces[j] -= fij;  // Newton's 3rd Law helps with efficiency
        }
      }
    }
  }
}

void MolecularDynamics::reset_forces() {
  for (int i = 0; i < _system.npart; ++i) {
    if (i < _forces.size())
      _forces[i].fill(0);
    else
      _forces.push_back({0., 0., 0.});
  }
}

void MolecularDynamics::move(int i) {
  // copy the i-th particle in a temporary variable for readability
  Particle p = _system.particles[i];

  // single step of Verlet algorithm
  p.r_new =
      periodic_bc(2. * p.r - p.r_old + _forces[i] * pow(_simulation.dt, 2));

  // estimate velocity in position r
  p.v = periodic_bc(p.r_new - p.r_old) / (2. * _simulation.dt);

  // update positions
  p.r_old = p.r;  // the velocity estimated refers to r_old now
  p.r = p.r_new;  // ... remember it when calculating K and V

  // copy back updated particle (checked)
  _system.particles[i] = p;
}

// measurement

void MolecularDynamics::measure() {
  string pot = _names["potential"], kin = _names["kinetic"],
         tot = _names["total"], temp = _names["temperature"],
         press = _names["pressure"];
  double v = potential_energy() / _system.npart;
  double t = kinetic_energy() / _system.npart;
  estimate(pot) += v;
  estimate(kin) += t;
  estimate(tot) += t + v;
  estimate(temp) += 2. / 3. * t;
  estimate(press) += pressure();
}

double MolecularDynamics::potential_energy() const {
  double v = 0, vij, dr;
  array<double, 3> pi, pj;
  // cycle over (unique) pairs of particles
  for (int i = 0; i < _system.npart - 1; i++) {
    pi = _system.particles[i].r_old;
    for (int j = i + 1; j < _system.npart; j++) {
      pj = _system.particles[j].r_old;
      dr = norm(periodic_bc(pi - pj));
      if (dr < _system.rcut) {
        vij = 4.0 / pow(dr, 12) - 4.0 / pow(dr, 6);
        v += vij;
      }
    }
  }
  return v;
}

double MolecularDynamics::kinetic_energy() const {
  double ke = 0;
  for (Particle p : _system.particles) ke += norm_squared(p.v);
  return 0.5 * ke;
}

double MolecularDynamics::pressure() const {
  return _system.density * _system.temperature + virial() / _system.volume;
}

double MolecularDynamics::virial() const {
  double w = 0, wij, dr, fraction;
  array<double, 3> pi, pj;
  // cycle over (unique) pairs of positions
  for (int i = 0; i < _system.npart - 1; ++i) {
    pi = _system.particles[i].r;
    for (int j = i + 1; j < _system.npart; ++j) {
      pj = _system.particles[j].r;
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

void MolecularDynamics::calculate_g(int runs, int nblocks,
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

void MolecularDynamics::fill_g() {
  double r, dV, normalization;
  double dr = 0.5 * _system.side / _nbins;
  array<double, 3> pi, pj;
  for (int i = 0; i < _system.npart - 1; ++i) {
    pi = _system.particles[i].r;
    for (int j = i + 1; j < _system.npart; ++j) {
      pj = _system.particles[j].r;
      r = norm(periodic_bc(pi - pj));
      dV = 4. / 3. * M_PI * (pow(r + dr, 3) - pow(r, 3));
      normalization = _system.density * _system.npart * dV;
      _g[int(r / dr)] += 2 / normalization;
    }
  }
}

void MolecularDynamics::average_g(double block_length) {
  for (int i = 0; i < _nbins; ++i) {
    _g[i] /= block_length;
  }
}

// results

void MolecularDynamics::output_g(map<string, ofstream> &out) {
  for (string s : _observables) {
    if (is_substring("radial", s)) {
      for (int i = 0; i < _nbins; ++i) {
        out[s] << _g[i] << ' ';
      }
      out[s] << endl;
    }
  }
}

void MolecularDynamics::final_output(map<string, ofstream> &out) {
  // NOTE: maybe overload the function in Experiment with no output argument
  ofstream out0("config/old_" + check_phase() + ".0"),
      out1("config/old_" + check_phase() + ".1");
  cout << endl
       << "Writing final configuration to files old_" + check_phase() +
              ".0 and old_" + check_phase() + ".1 "
       << endl;
  array<double, 3> pos;
  for (int i = 0; i < _system.npart; i++) {
    pos = _system.particles[i].r / _system.side;
    out1 << pos[0] << " " << pos[1] << " " << pos[2];
    pos = _system.particles[i].r_old / _system.side;
    out0 << pos[0] << " " << pos[1] << " " << pos[2];
    if (i < _system.npart - 1) {
      out1 << endl;
      out0 << endl;
    }
  }
  out1.close();
  out0.close();
}

void MolecularDynamics::starting_message() const {
  cout << endl << "Classic Lennard-Jones fluid" << endl;
  cout << "Molecular dynamics simulation in NVE ensemble  " << endl << endl;
  cout << "Interatomic potential v(r) = 4 * [(1/r)^12 - (1/r)^6]" << endl
       << endl;
  cout << "The program uses Lennard-Jones units " << endl;
  cout << "Number of particles = " << _system.npart << endl;
  cout << "Density of particles = " << _system.density << endl;
  cout << "Volume of the simulation box = " << _system.volume << endl;
  cout << "Edge of the simulation box = " << _system.side << endl;
  cout << "The program integrates Newton equations with the Verlet method "
       << endl;
  cout << "Time step = " << _simulation.dt << endl;
  cout << "Number of steps = " << _simulation.nsteps << endl;
}

// boundary conditions

double MolecularDynamics::periodic_bc(double x) const {
  // keep x within a box of side `_system.side` using periodic boundary
  // conditions
  return periodic_BC(x, _system.side);
}

array<double, 3> MolecularDynamics::periodic_bc(
    const array<double, 3> &x) const {
  // keep x within a box of side `_system.side` using periodic boundary
  // conditions
  return periodic_BC(x, _system.side);
}