/****************************************************************
 definition of the class Hydrogen, daughter to Experiment
*****************************************************************/

#ifndef __hydrogen__
#define __hydrogen__

#include "experiment.h"
#include "metropolis.h"

double psi_1s(const vector<double> &);
double psi_2p(const vector<double> &);

class Hydrogen : public Experiment {
 protected:
  Metropolis _metropolis;
  double _semi_width;

 public:
  Hydrogen(string file, int *seed, const vector<double> &x0, double semi_width);
  void measure();  // "elementary" measurement
};

#endif  // __hydrogen__
