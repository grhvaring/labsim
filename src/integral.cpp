/****************************************************************
 Implementation of the class Integral
****************************************************************/

#include "integral.h"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <string>

#include "libSim.h"

using namespace std;

Integral::Integral(){};
Integral::~Integral(){};

double Integral::uniform(int N_points, simple_function f, int a, int b) {
  check_positive(N_points);
  sort(a, b);
  double I = 0;
  for (int i = 0; i < N_points; i++) {
    I += f(_random.Rannyu(a, b));
  }
  I /= N_points;
  return I;
}

double Integral::from_dist(int N_points, simple_function f, simple_function p,
                           double pmax, int a, int b) {
  check_positive(N_points);
  sort(a, b);
  double I = 0;
  double x;
  for (int i = 0; i < N_points; i++) {
    x = _random.accept_reject(p, pmax);
    I += f(x) / p(x);
  }
  I /= N_points;
  return I;
}