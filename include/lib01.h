/****************************************************************
 class definitions used in the first lab
*****************************************************************/

#include "experiment.h"

// simulation of a uniformly distributed random variable

class Uniform : public Experiment {
  using Experiment::Experiment;  // inherit Experiment constructor
 public:
  void measure();
};

// uniform, exponential and lorentz die simulation
class Three_dice : public Experiment {
  using Experiment::Experiment;  // inherit Experiment constructor
 public:
  void measure();  // dice throw
  void output_block(map<string, ofstream>&) {
    /* do not output the partial results */
  }
  void final_output(map<string, ofstream>&);
};

// Buffon's experiment
class Buffon : public Experiment {
  map<string, double> _args;

 public:
  // initialize with custom parameters
  Buffon(string file, int* seed, map<string, double> args)
      : Experiment{file, seed, true}, _args{args} {}
  void measure();         // throw the stick once
  void estimate_block();  // use the counts to estimate pi (in a given block)
};