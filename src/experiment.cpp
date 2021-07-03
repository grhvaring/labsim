/****************************************************************
 implementation of the class Experiment
*****************************************************************/

#include "experiment.h"

void Experiment::block_averages(int iterations, int nblocks,
                                map<string, ofstream> &out) {
  // calculate block length (cast to integer) based on input parameters
  _block_length = iterations / nblocks;
  // clean any previous result before the calculation starts
  reset_containers();
  // iterate over blocks
  for (_block_counter = 0; _block_counter < nblocks; ++_block_counter) {
    // clean block estimates for each observable
    reset_block();
    // iterate inside the block
    for (int i = 0; i < _block_length; ++i) {
      run();      // internal processing (to overload)
      measure();  // measurements (to overload)
    }
    estimate_block();   // use measurements to calculate block estimates
    update_averages();  // calculate current averages and errors
    output_block(out);  // output current results
    // progress bar
    if (_show_progress) _progress->write(double(_block_counter + 1) / nblocks);
  }
  final_output(out);  // output final results
}

void Experiment::estimate_block() {
  // the default block estimate is the average of the measurements
  for (string s : _observables) estimate(s) /= _block_length;
}

void Experiment::update_averages() {
  for (string s : _observables) {
    sum(s) += estimate(s);
    sum2(s) += pow(estimate(s), 2);
    average(s) = sum(s) / (_block_counter + 1);
    sigma(s) = error(average(s), sum2(s), _block_counter);
  }
}

void Experiment::output_block(map<string, ofstream> &out) {
  // output the current best estimate with its uncertainty after each block
  for (string s : _observables)
    out[s] << step() << " " << estimate(s) << " " << average(s) << " "
           << sigma(s) << endl;
}

// single output stream overloads

Experiment::Experiment(string single_obs, int *seed, bool show_progress)
    : _random{seed}, _seed{seed}, _show_progress(show_progress) {
  vector<string> obs{single_obs};
  _observables = obs;
  reset_containers();
  if (show_progress) _progress = new progress_bar{clog, 70u};
}

void Experiment::block_averages(int iterations, int nblocks, ofstream &out) {
  // does not work: move(out) forgets the position iside the file
  map<string, ofstream> m_out;
  string observable = _observables[0];
  m_out[observable] = move(out);  // ofstream does not have a copy constructor
  block_averages(iterations, nblocks, m_out);
}