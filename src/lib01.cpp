/****************************************************************
 implementation of the classes used in the fist lab
*****************************************************************/

#include "lib01.h"

#include <cmath>

using namespace std;

// uniform distribution average and variance estimations

void Uniform::measure() {
  estimate("average") += _random.Rannyu();
  estimate("variance") += pow(_random.Rannyu() - 0.5, 2);
  // TODO: what if the string is something else?
}

// uniform, exponential and lorentz dice throws

void Three_dice::measure() {
  // TODO: deal with pre/suf-fixes for the filenames
  estimate("uniform_die") += _random.Rannyu();
  estimate("exponential_die") += _random.exponential(1);
  estimate("lorentz_die") += _random.lorentz(0, 1);
}

void Three_dice::final_output(map<string, ofstream>& out) {
  for (string k : _observables) out[k] << average(k) << ' ';
}

// Buffon's experiment for estimating the value of pi

void Buffon::measure() {
  // simulate a y coordinate between 0 and the grid spacing
  double h = _random.Rannyu(0, _args["d"]);
  // simulate uniformly distributed angle (without using pi)
  double sine = _random.uniform_sin_angle();      // sine of angle in [0,pi/2]
  double min_distance = 0.5 * _args["L"] * sine;  // hit distance
  bool lines_cross = (h < min_distance) || (h > _args["d"] - min_distance);
  // increase by 1 if the stick touches a line
  if (lines_cross) estimate(_observables[0]) += 1;
}

void Buffon::estimate_block() {
  double hits = estimate(_observables[0]);
  estimate(_observables[0]) =
      2 * _args["L"] * _block_length / (hits * _args["d"]);
}
