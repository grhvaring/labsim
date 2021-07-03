/****************************************************************
 Definition of the class Integral
****************************************************************/

#ifndef __Integral__
#define __Integral__

#include <vector>

#include "random.h"

class Integral {
 private:
  Random _random;

 protected:
 public:
  // constructors
  Integral();
  // destructor
  ~Integral();
  // methods
  double uniform(int N_points, simple_function, int a = 0, int b = 1);
  double from_dist(int N_points, simple_function f, simple_function p,
                   double pmax, int a = 0, int b = 1);
};

#endif