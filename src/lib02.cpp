/****************************************************************
 implementation of the classes used in the second lab
*****************************************************************/

#include "lib02.h"

#include <cmath>

#include "linalg.h"

void Integral::measure() {
  double x, factor, I;
  for (string file : _observables) {
    if (is_substring("uniform", file)) {
      x = _random.Rannyu(_a, _b);
      factor = (_a != _b) ? 1. / (_b - _a) : 0;
      I = factor * _f(x);
    } else if (is_substring("importance", file)) {
      x = _random.accept_reject(_p, _pmax);
      I = _f(x) / _p(x);
    }  // TODO: what if none of the above applies?
    estimate(file) += I;
  }
}

void Integral::set_f(simple_function f) { _f = f; }

void Integral::set_p(simple_function p, double pmax) {
  _p = p;
  _pmax = pmax;
}

// random walks

RandomWalk::RandomWalk(vector<string> steps, int* seed, double step_size,
                       int dimensions)
    : Experiment(steps, seed), _a(step_size), _dim(dimensions) {
  reset_position();
}

void RandomWalk::measure() {
  for (string step : _observables) {
    estimate(step) += norm_squared(_r);
    jump();
  }
  reset_position();
}

void RandomWalk::estimate_block() {
  for (string step : _observables)
    estimate(step) = sqrt(estimate(step) / _block_length);
}

void RandomWalk::final_output(map<string, ofstream>& out) {
  for (string step : _observables)
    out[_observables[0]] << average(step) << " " << sigma(step) << endl;
}

// discrete random walk

void DiscreteRW::jump() {
  // generate a 3D (_dim-dimentional actually) vector of length `a` along one of
  // the main axes
  vector<double> shift;
  shift.resize(_dim, 0);
  bool forward = (_random.Rannyu() < 0.5) ? true : false;
  shift[int(_dim * _random.Rannyu())] += (forward) ? _a : -_a;
  _r += shift;
}

// continuous random walk

void ContinuousRW::jump() {
  // generate a 3D unit vector in a random direction, uniform in the solid
  // angle
  double theta = _random.angle_sin();
  double phi = _random.Rannyu(0, 2 * M_PI);
  vector<double> n{sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta)};
  _r += _a * n;
}