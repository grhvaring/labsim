/*****************************************************************
 Definition of the class Metropolis, daughter to Random
*****************************************************************/

#ifndef __metropolis__
#define __metropolis__

#include "random.h"

class Metropolis : public Random {
 protected:
  Random _random;
  vector<double> _x, _x_new;
  double _alpha;

 public:
  Metropolis(vector<double> x0, int* seed = nullptr);
  vector<double> uniform_mrt2(scalar_function p, double semi_width);
  double alpha() const { return _alpha; }
  void reset(vector<double> x0);

#endif  // __metropolis__