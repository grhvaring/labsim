/****************************************************************
*****************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
*****************************************************************
*****************************************************************/

#ifndef __Random__
#define __Random__

#include "libsim.h"

class Random {
 private:
  int m1, m2, m3, m4, l1, l2, l3, l4, n1, n2, n3, n4;

 public:
  // constructors
  Random();
  Random(int*, int p1 = 2892, int p2 = 1263);
  // destructor
  ~Random();
  // methods
  void SetRandom(int*, int p1 = 2892, int p2 = 1263);
  void SaveSeed();
  double Rannyu(void);
  double Rannyu(double min, double max);
  double Rannyu(double max) { return Rannyu(0, max); }
  double gauss(double mean = 0, double sigma = 1);
  double lorentz(double mean = 0, double Gamma = 1);
  double exponential(double lambda = 1);
  double angle_sin(void);
  double uniform_sin_angle(void);
  double cos_pi2(void);
  double accept_reject(simple_function p, double pmax, double a = 0,
                       double b = 1);

  // multiple dimentions
  vector<double> uniform(int dim);
  vector<double> uniform(vector<double> min, vector<double> max);
  vector<double> uniform(vector<double> x0, double semi_width);
  array<double, 3> uniform(array<double, 3> min, array<double, 3> max);
  array<double, 3> uniform(array<double, 3> x0, double semi_width);
  vector<double> gauss(int dim);
  vector<double> gauss(vector<double> mu, double sigma);
};

#endif  // __Random__

/****************************************************************
*****************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
*****************************************************************
*****************************************************************/
