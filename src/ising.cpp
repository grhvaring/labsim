/****************************************************************
 Implementation of the class Ising
*****************************************************************/

#include "ising.h"

#include <cmath>
#include <iostream>

Ising::Ising(vector<string> observables, int *seed, double T)
    : Experiment{observables, seed} {
  _system.T = T;
  initialize();
}

// setup

void Ising::initialize() {
  read_input();
  set_random_spins();
  extract_names();
}

void Ising::read_input() {
  ifstream in("input.dat");
  in >> _system.nspins;  // number of spins
  in >> _system.h;       // esternal magnetic field
  in >> _system.J;       // coupling interaction strength
  in.close();
}

void Ising::set_random_spins() {
  // start with a T=inf system, i.e. with spins oriented randomly
  double s;
  for (int i = 0; i < _system.nspins; ++i) {
    s = (_random.Rannyu() < 0.5) ? -1 : 1;
    // assume the spins are not already initialized
    _system.spins.push_back(s);
  }
}

void Ising::extract_names() {
  // very not efficient way of removing eventual suf/prefixes from the names
  // but it should be alright, since the list is very short and the operation
  // is performed just once
  vector<string> observables{"energy", "magnetization", "heat_capacity",
                             "susceptibility"};
  for (string s : _observables) {
    for (string obs : observables) {
      if (is_substring(obs, s)) _names[obs] = s;
    }
  }
  // if the names contain "eq" activate the flag _equilibration
  _equilibration = is_substring("eq", _observables[0]);
  // if the names do not contain "gibbs" use Metropolis sampling
  _metro_sampling = !is_substring("gibbs", _observables[0]);
}

// simulation

void Ising::run() {
  for (int i = 0; i < _system.nspins; ++i) {
    int index = _system.nspins * _random.Rannyu();  // pick spin at random
    flip(index);  // flip it according to the rule chosen
  }
}

void Ising::flip(int index) {
  int spin_i = _system.spins[index];  // store current spin for readability
  int sum_nearest_neighbors = _system.spins[periodic_bc(index - 1)] +
                              _system.spins[periodic_bc(index + 1)];
  // energy difference between the (proposed) new and current configuration
  double dE = 2.0 * (_system.J * sum_nearest_neighbors + _system.h);
  if (_metro_sampling) {
    double A = min(1., boltzmann_weight(spin_i * dE));
    if (_random.Rannyu() < A) spin_i *= -1;
  } else {
    spin_i = (_random.Rannyu() < gibbs_weight(dE)) ? 1 : -1;
  }
  _system.spins[index] = spin_i;
}

double Ising::boltzmann_weight(double dE) { return exp(-dE / _system.T); }

double Ising::gibbs_weight(double dE) { return 1 / (1 + boltzmann_weight(dE)); }

void Ising::measure() {
  double e = energy();
  estimate(_names["energy"]) += e;
  estimate(_names["heat_capacity"]) += pow(e, 2);
  double M = magnetization();
  estimate(_names["magnetization"]) += M;
  estimate(_names["susceptibility"]) += pow(M, 2);
}

void Ising::estimate_block() {
  // inernal energy
  string e = _names["energy"];
  estimate(e) /= _block_length;
  // heat capacity
  string C = _names["heat_capacity"];
  double beta = 1. / _system.T;
  estimate(C) =
      pow(beta, 2) * (estimate(C) / _block_length - pow(estimate(e), 2));
  // magnetization and magnetic susceptibility
  string M = _names["magnetization"], chi = _names["susceptibility"];
  estimate(M) /= _block_length;
  estimate(chi) = beta * estimate(chi) / _block_length;
  // per spin
  int N = _system.nspins;
  for (auto name : _names) estimate(name.second) /= N;
  // possible problems with invalid keys? Not really
}

double Ising::energy() {
  double e = 0;
  int s_curr, s_next;
  for (int i = 0; i < _system.nspins; ++i) {
    s_curr = _system.spins[i];
    s_next = _system.spins[periodic_bc(i + 1)];
    e += _system.J * s_curr * s_next + _system.h * s_curr;
  }
  return -e;
}

double Ising::magnetization() {
  double M = 0;
  for (int spin : _system.spins) M += spin;
  return M;
}

int Ising::periodic_bc(int index) const {
  // checked
  int correction = 0;
  if (index < 0)
    correction = 1;
  else if (index >= _system.nspins)
    correction = -1;
  return index + correction * _system.nspins;
}

// output

void Ising::output_block(map<string, ofstream> &out) {
  if (_equilibration) {
    for (string k : _observables)
      out[k] << step() << " " << estimate(k) << endl;
  }
}

void Ising::final_output(map<string, ofstream> &out) {
  if (!_equilibration) {
    double T = _system.T;
    for (string k : _observables)
      out[k] << T << " " << average(k) << " " << sigma(k) << endl;
  }
}

void Ising::starting_message() const {
  cout << "Classic 1D Ising model" << endl;
  cout << "Monte Carlo simulation" << endl;
  cout << endl;
  cout << "Nearest neighbour interaction" << endl << endl;
  cout << "Boltzmann weight exp(- beta * H ), beta = 1/T" << endl;
  cout << endl;
  cout << "The program uses k_B = 1 and mu_B = 1 units" << endl;
  cout << "Temperature = " << _system.T << endl;
  cout << "Number of spins = " << _system.nspins << endl;
  cout << "Exchange interaction = " << _system.J << endl;
  cout << "External field = " << _system.h << endl;
}