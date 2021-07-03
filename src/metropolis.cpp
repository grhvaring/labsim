/*****************************************************************
 Implementation of the class Metropolis, daughter to Random
*****************************************************************/

#include "metropolis.h"

double A(double q) { return min(1., q); }

Metropolis::Metropolis(vector<double> x0, int* seed) : Random(seed) {
  reset(x0);
}

vector<double> Metropolis::uniform_mrt2(scalar_function p, double semi_width) {
  _x_new = _random.uniform(_x, semi_width);
  double q = p(_x_new) / p(_x);
  if (_random.Rannyu() <= A(q)) _x = _x_new;
  _alpha += A(q);
  return _x;
}

void Metropolis::reset(vector<double> x0) {
  _x = x0;
  _alpha = 0;
}