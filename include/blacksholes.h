/****************************************************************
 Definition of the class BlackNSholes
*****************************************************************/

#ifndef __blacksholes__
#define __blacksholes__

#include "experiment.h"

struct BNS_args {
  double S0, T, K, r, sigma, nsteps;
};

class BlackNSholes : public Experiment {
  BNS_args _args;

 public:
  BlackNSholes(vector<string> obs, int* seed, BNS_args args)
      : Experiment{obs, seed, false}, _args{args} {}
  void measure();
  double final_S(string);
  double gmb_step(double);
};

#endif  //__blacksholes__