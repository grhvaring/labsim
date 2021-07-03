/****************************************************************
 Definition of the class Ising, which simulates a 1D Ising model
*****************************************************************/

#ifndef __ising__
#define __ising__

#include "experiment.h"

struct System {
  int nspins;
  double T, J, h;  // temperature, coupling strength, ext. magnetic field
  vector<int> spins;
};

class Ising : public Experiment {
  System _system;
  bool _metro_sampling, _equilibration;
  map<string, string> _names;

 public:
  Ising(vector<string> obs, int *seed, double T);

  // setup
  void initialize();
  void read_input();
  void set_random_spins();
  void extract_names();

  // simulation
  void run();      // internal processing to perform before the measurements
  void measure();  // measurements (inside the block)
  void estimate_block();  // calculate estimates (with the block measurements)

  // output
  void starting_message() const;
  void output_block(map<string, ofstream> &);
  void final_output(map<string, ofstream> &);

  // other methods
  void flip(int);
  double boltzmann_weight(double);
  double gibbs_weight(double);
  double energy();
  double magnetization();
  int periodic_bc(int) const;
};

#endif  // __ising__
