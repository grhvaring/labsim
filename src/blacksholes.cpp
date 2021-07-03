/****************************************************************
 implementation of the class BlackNSholes
*****************************************************************/

#include "blacksholes.h"

#include <cmath>

void BlackNSholes::measure() {
  double profit;
  int sign;
  double discount = exp(-_args.r * _args.T);
  for (string name : _observables) {
    // infer option type and calculation method from filename
    sign = is_substring("call", name) ? 1 : -1;
    profit = max(0., sign * (final_S(name) - _args.K));
    estimate(name) += discount * profit;
  }
}

double BlackNSholes::final_S(string name) {
  double S = _args.S0;
  if (is_substring("direct", name))
    S *= gmb_step(_args.T);
  else if (is_substring("discrete", name)) {
    double time_step = _args.T / _args.nsteps;
    for (int i = 0; i <= _args.nsteps; i++) S *= gmb_step(time_step);
  }  // TODO: deal with possible name typos
  return S;
}

double BlackNSholes::gmb_step(double dt) {
  double z = _random.gauss();
  return exp((_args.r - pow(_args.sigma, 2) / 2) * dt +
             _args.sigma * z * sqrt(dt));
}