/****************************************************************
*****************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
*****************************************************************
*****************************************************************/

#include "random.h"

#include <cassert>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>

using namespace std;

Random ::Random() {
  int p1 = 2892;
  int p2 = 1263;
  int seed[] = {0, 0, 0, 1};
  SetRandom(seed, p1, p2);
}

Random ::Random(int* seed, int p1, int p2) { SetRandom(seed, p1, p2); }
Random ::~Random() {}

void Random ::SaveSeed() {
  ofstream WriteSeed;
  WriteSeed.open("seed.out");
  if (WriteSeed.is_open()) {
    WriteSeed << l1 << " " << l2 << " " << l3 << " " << l4 << endl;
    ;
  } else
    cerr << "PROBLEM: Unable to open random.out" << endl;
  WriteSeed.close();
  return;
}

double Random ::gauss(double mean, double sigma) {
  double s = Rannyu();
  double t = Rannyu();
  double x = sqrt(-2. * log(1. - s)) * cos(2. * M_PI * t);
  return mean + x * sigma;
}

double Random ::lorentz(double mean, double Gamma) {
  double y = Rannyu();
  return mean + Gamma * tan(M_PI * (y - 0.5));
}

double Random ::exponential(double lambda) {
  double y = Rannyu();
  return -1. / lambda * log(1. - y);
}

double Random ::angle_sin() {
  double y = Rannyu();
  return acos(1. - 2. * y);
}

double Random::uniform_sin_angle() {
  // return the sine of an angle distributed uniformly in [0, pi/2]
  double x, y, r2;
  do {
    x = Rannyu();
    y = Rannyu();
    r2 = pow(x, 2) + pow(y, 2);
  } while (r2 > 1);
  double cos2 = x * x / r2;
  return sqrt(1 - cos2);
}

double Random::cos_pi2() {
  double y = Rannyu();
  return asin(y) / M_PI_2;
}

double Random::accept_reject(simple_function p, double pmax, double a,
                             double b) {
  double x, r;
  do {
    x = Rannyu(a, b);
    r = Rannyu();
  } while (r >= p(x) / pmax);
  return x;
}

vector<double> Random::uniform(int dim) {
  vector<double> x;
  double r;
  for (int i = 0; i < dim; i++) {
    r = Rannyu();
    x.push_back(r);
  }
  return x;
}

vector<double> Random::uniform(vector<double> min, vector<double> max) {
  assert(min.size() == max.size());
  vector<double> x;
  double r;
  for (int i = 0; i < min.size(); i++) {
    r = Rannyu(min[i], max[i]);
    x.push_back(r);
  }
  return x;
}

vector<double> Random::uniform(vector<double> x0, double semi_width) {
  vector<double> x;
  double r;
  for (int i = 0; i < x0.size(); i++) {
    r = x0[i] + semi_width * (Rannyu() - 0.5);
    x.push_back(r);
  }
  return x;
}

array<double, 3> Random::uniform(array<double, 3> min, array<double, 3> max) {
  assert(min.size() == max.size());
  array<double, 3> x;
  for (int i = 0; i < min.size(); i++) {
    x[i] = Rannyu(min[i], max[i]);
  }
  return x;
}

array<double, 3> Random::uniform(array<double, 3> x0, double semi_width) {
  array<double, 3> x;
  for (int i = 0; i < x0.size(); i++) {
    x[i] = x0[i] + semi_width * (Rannyu() - 0.5);
  }
  return x;
}

vector<double> Random::gauss(int dim) {
  vector<double> x;
  double r;
  for (int i = 0; i < dim; i++) {
    r = gauss();
    x.push_back(r);
  }
  return x;
}

vector<double> Random::gauss(vector<double> mu, double sigma) {
  vector<double> x;
  double r;
  for (int i = 0; i < mu.size(); i++) {
    r = gauss(mu[i], sigma);
    x.push_back(r);
  }
  return x;
}

double Random ::Rannyu(double min, double max) {
  return min + (max - min) * Rannyu();
}

double Random ::Rannyu(void) {
  const double twom12 = 0.000244140625;
  int i1, i2, i3, i4;
  double r;

  i1 = l1 * m4 + l2 * m3 + l3 * m2 + l4 * m1 + n1;
  i2 = l2 * m4 + l3 * m3 + l4 * m2 + n2;
  i3 = l3 * m4 + l4 * m3 + n3;
  i4 = l4 * m4 + n4;
  l4 = i4 % 4096;
  i3 = i3 + i4 / 4096;
  l3 = i3 % 4096;
  i2 = i2 + i3 / 4096;
  l2 = i2 % 4096;
  l1 = (i1 + i2 / 4096) % 4096;
  r = twom12 * (l1 + twom12 * (l2 + twom12 * (l3 + twom12 * (l4))));

  return r;
}

void Random ::SetRandom(int* s, int p1, int p2) {
  if (s == nullptr) {
    int default_s[] = {0, 0, 0, 1};
    s = default_s;
  }
  m1 = 502;
  m2 = 1521;
  m3 = 4071;
  m4 = 2107;
  l1 = s[0] % 4096;
  l2 = s[1] % 4096;
  l3 = s[2] % 4096;
  l4 = s[3] % 4096;
  l4 = 2 * (l4 / 2) + 1;
  n1 = 0;
  n2 = 0;
  n3 = p1;
  n4 = p2;

  return;
}

/****************************************************************
*****************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
*****************************************************************
*****************************************************************/
