/****************************************************************
 Implementation of the class Hydrogen
*****************************************************************/

#include "hydrogen.h"

#include "linalg.h"

Hydrogen::Hydrogen(string file, int *seed, const vector<double> &x0,
                   double semi_width)
    : Experiment{file, seed}, _metropolis{x0, seed} {
  _semi_width = semi_width;
}

void Hydrogen::measure() {
  vector<double> x;
  string s = _observables[0];
  if (is_substring("1s", s)) x = _metropolis.uniform_mrt2(psi_1s, _semi_width);
  if (is_substring("2p", s)) x = _metropolis.uniform_mrt2(psi_2p, _semi_width);
  estimate(s) += norm(x);
}

double psi_1s(const vector<double> &x) {
  double r = norm(x);
  return exp(-2 * r);
}

double psi_2p(const vector<double> &x) {
  double r = norm(x);
  return pow(x[2], 2) * exp(-r);
}