/****************************************************************
 Implementation of the class TemperingTSP
*****************************************************************/

#include "tempering_tsp.h"

#include <iostream>

#include "linalg.h"

// initialization

TemperingTSP::TemperingTSP(int nsteps, double temperature, int *seed)
    : _nsteps{nsteps}, _temperature{temperature}, _random{seed} {
  generate_route();
}

void TemperingTSP::generate_route() {
  _route.size = _nsteps;
  _route.to_indices(_nsteps);
  _new_route.to_indices(_nsteps);
  shuffle_route();
  _accepted = 0;
}

void TemperingTSP::generate_positions(string shape, ofstream &out) {
  // description
  _positions.clear();
  _shape = shape;
  double x, y;
  if (shape == "circle") {
    double theta = 0;
    for (int n = 0; n < _nsteps; ++n, theta += 2 * M_PI / _nsteps) {
      x = cos(theta);
      y = sin(theta);
      _positions.push_back({x, y});
      out << x << " " << y << endl;
    }
  } else if (shape == "square") {
    for (int i = 0; i < _nsteps; ++i) {
      x = _random.Rannyu();
      y = _random.Rannyu();
      _positions.push_back({x, y});
      out << x << " " << y << endl;
    }
  }
}

// evolution

void TemperingTSP::metropolis_move() {
  for (int i = 0; i < _nsteps; ++i) {
    _new_route.steps[i] = _route.steps[i];
  }
  mutate_new_route();
  double dL = new_length() - route_length();
  double A = min(1., boltzmann_weight(dL));
  if (_random.Rannyu() < A) {
    for (int i = 0; i < _nsteps; ++i) {
      _route.steps[i] = _new_route.steps[i];
    }
    _accepted++;
  }
}

void TemperingTSP::mutate_new_route() {
  if (_random.Rannyu() < 0.5) {
    // random permutation (with a non zero chance of getting two identical
    // indices) of a random subset of the cities
    int start = _random.Rannyu(1, _nsteps);
    int end = _random.Rannyu(start, _nsteps);
    int ri1, ri2;
    for (int i = 0; i < 0.5 * _random.Rannyu(0, end - start); ++i) {
      ri1 = random_index(end, start), ri2 = random_index(end, start);
      _new_route.swap(ri1, ri2);
    }
  } else {
    // random inversion of a section of the route
    int start = _random.Rannyu(1, _nsteps);
    int end = _random.Rannyu(start, _nsteps);
    reverse(_new_route.steps + start, _new_route.steps + end);
  }
}

// other methods

void TemperingTSP::shuffle_route() {
  // generate a random permutation of the wanted route, with fixed starting
  // point
  for (int step = 1; step < _nsteps; ++step) {
    // permutation obtained by a chain of swaps
    _route.swap(step, random_index(_nsteps, 1));
  }
}

double TemperingTSP::route_length() {
  double L2 = 0, dr2;
  int curr, next;
  for (int i = 0; i < _nsteps - 1; ++i) {
    curr = _route.steps[i];
    next = _route.steps[i + 1];
    dr2 = norm_squared(_positions[next] - _positions[curr]);
    L2 += dr2;
  }
  // instead of using pbc for just this one case
  curr = _route.steps[_nsteps - 1];
  next = _route.steps[0];
  dr2 = norm_squared(_positions[next] - _positions[curr]);
  return L2 + dr2;
}

double TemperingTSP::new_length() {
  double L2 = 0, dr2;
  int curr, next;
  for (int i = 0; i < _nsteps - 1; ++i) {
    curr = _new_route.steps[i];
    next = _new_route.steps[i + 1];
    dr2 = norm_squared(_positions[next] - _positions[curr]);
    L2 += dr2;
  }
  // sacrificing a little bit of clarity for better efficiency
  curr = _new_route.steps[_nsteps - 1];
  next = _new_route.steps[0];
  dr2 = norm_squared(_positions[next] - _positions[curr]);
  return L2 + dr2;
}

int TemperingTSP::periodic_bc(int index) {
  // not used
  int shift = 0;
  if (index > _nsteps) shift = _nsteps;
  return index - shift;
}

void TemperingTSP::print_route() {
  cout << "Route: [ ";
  int step = 0;
  for (; step < _nsteps - 1; ++step) cout << _route.steps[step] << ", ";
  cout << _route.steps[step] << " ]" << endl;
}

void TemperingTSP::print_new_route() {
  cout << "New route: [ ";
  int step = 0;
  for (; step < _nsteps - 1; ++step) cout << _new_route.steps[step] << ", ";
  cout << _new_route.steps[step] << " ]" << endl;
}

void TemperingTSP::save_route(ofstream &out) {
  for (int i = 0; i < _nsteps; ++i) out << _route.steps[i] << " ";
  out << endl;
}