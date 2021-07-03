/****************************************************************
 class definitions used in the second lab
*****************************************************************/

#ifndef __lib02__
#define __lib02__

#include "experiment.h"

// Integral

class Integral : public Experiment {
  simple_function _f, _p;
  double _a, _b, _pmax;

 public:
  Integral(vector<string> files, int* seed, double a = 0, double b = 1)
      : Experiment{files, seed}, _a{a}, _b{b} {}
  void measure();
  void set_f(simple_function);
  void set_p(simple_function, double pmax);
};

// random walks

class RandomWalk : public Experiment {
 protected:
  double _a;
  vector<double> _r;
  int _nsteps, _dim;

 public:
  RandomWalk(vector<string> steps, int* seed, double step_size = 1,
             int dimensions = 3);

  // initialization
  void reset_position() { _r.assign(_dim, 0); }
  // simulation
  void measure();
  virtual void jump() = 0;
  // output
  void output_block(map<string, ofstream>&) {}  // do not output partial results
  void final_output(map<string, ofstream>&);
  // other methods
  void estimate_block();
};

class DiscreteRW : public RandomWalk {
  using RandomWalk::RandomWalk;

 public:
  void jump();
};

class ContinuousRW : public RandomWalk {
  using RandomWalk::RandomWalk;

 public:
  void jump();
};

#endif  // __lib02__