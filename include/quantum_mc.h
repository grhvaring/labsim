/****************************************************************
 definition of the class QuantumMC
*****************************************************************/

#ifndef __quantum_mc__
#define __quantum_mc__

#include "experiment.h"

class QuantumMC : public Experiment {
 protected:
  double _x, _x_new, _acceptance, _semi_width, _sigma, _mu;
  bool _minimize;
  vector<double> _histogram, _sum_histogram, _sum2_histogram, _error_histogram,
      _mean_histogram;
  int _nbins;
  double _semi_width_histogram;

 public:
  QuantumMC(vector<string> file, int *seed, double x0)
      : Experiment{file, seed, false}, _x{x0} {
    _minimize = is_substring("min", file[0]);
  }

  void set_parameters(double sigma, double mu);
  void initialize_histograms(int bins = 500, double semi_width = 3);

  double metropolis_step();
  double psi_trial(double x);
  double psi_trial_squared(double x) { return pow(psi_trial(x), 2); }

  double relative_d2_psi_trial(double x);

  void measure();  // "elementary" measurement
  double kinetic_term(double x);
  double potential_term(double x);
  void fill_histogram(double x);
  void update_histogram();
  void finalize_histogram();

  // read/write

  double energy() { return average(_observables[0]); }
  double acceptance() { return _acceptance; }

  // output
  void output_block(map<string, ofstream> &out);
  void output_histogram();
  void final_output(map<string, ofstream> &out);
};

#endif  // __quantum_mc__
