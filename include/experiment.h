/****************************************************************
 definition of the class Experiment
*****************************************************************/

#ifndef __experiment__
#define __experiment__

#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "libsim.h"
#include "progress.h"
#include "random.h"

using namespace std;

// base class Experiment

class Experiment {
 protected:
  Containers _containers;  // struct used for block averaging
  int _block_length, _block_counter;
  vector<string> _observables;  // list of observables to estimate
  Random _random;               // pseudo-random number generator
  int *_seed;                   // seed needed to initialize _random
  progress_bar *_progress;
  bool _show_progress;  // flag for displaying the progress bar TODO: USE IT

 public:
  Experiment(vector<string> observables, int *seed = nullptr,
             bool show_progress = false)
      : _observables{observables},
        _random{seed},
        _seed{seed},
        _show_progress{show_progress} {
    reset_containers();
    if (show_progress) _progress = new progress_bar{clog, 70u};
  }

  void reset_containers() { _containers.reset_all(_observables); }

  // data blocking algorithm
  virtual void block_averages(int iterations, int nblocks,
                              map<string, ofstream> &out);

  // simulation
  void reset_block() { _containers.reset_block(); }
  virtual void run() {}  // internal processing to perform before "measurement"
  virtual void measure() = 0;      // "elementary" measurement
  virtual void estimate_block();   // use measurements to obtain block estimates
  virtual void update_averages();  // update averages and uncertainties

  // output
  virtual void output_block(map<string, ofstream> &);    // default implemented
  virtual void final_output(map<string, ofstream> &) {}  // not implemented

  // read-write
  Containers &containers() { return _containers; }
  int step() { return (_block_counter + 1) * _block_length; }
  double &estimate(string s) { return _containers.estimates[s]; }
  double &sum(string s) { return _containers.sums[s]; }
  double &sum2(string s) { return _containers.sums2[s]; }
  double &average(string s) { return _containers.averages[s]; }
  double &sigma(string s) { return _containers.sigmas[s]; }

  // single output stream and other overloads
  Experiment(string single_obs, int *seed = nullptr,
             bool show_progress = false);
  virtual void block_averages(int iterations, int nblocks, ofstream &out);

  virtual void block_averages(int iterations, map<string, ofstream> &out) {
    block_averages(iterations, iterations, out);
  }
  virtual void block_averages(int iterations, ofstream &out) {
    block_averages(iterations, iterations, out);
  }
};

#endif  // __experiment__