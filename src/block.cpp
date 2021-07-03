/****************************************************************
 implementation of the class Block
*****************************************************************/

#include "block.h"

#include <cmath>
#include <iostream>

double error(double average, double sum2, int N) {
  return (N > 0) ? sqrt((sum2 / (N + 1) - pow(average, 2)) / N) : 0;
}

vector<double> error(vector<double> avg, vector<double> sum2, int N) {
  vector<double> temp;
  for (int i = 0; i < avg.size(); i++) {
    temp.push_back(error(avg[i], sum2[i], N));
  }
  return temp;
}

Block::Block(vector<string> obs, bool progress)
    : _observables{obs}, _show_progress{progress} {}

// heart of the calculation: blocking average

void Block::averages(int iterations, int nblocks, Experiment &experiment,
                     map<string, ofstream> &out) {
  // calculate block length (cast to integer autom.) based on input parameters
  _length = iterations / nblocks;
  // clean every previous estimate before the calculation starts
  reset_all();
  // iterate over blocks
  for (_counter = 0; _counter < nblocks; ++_counter) {
    // clean block estimates for each observable
    reset_block();
    // iterate inside the block
    for (int i = 0; i < _length; ++i) {
      // internal processing of the experiment
      experiment.run();
      // update block estimate based on the experiment
      update_block(experiment);
    }
    // calculate current averages and errors (based on the experiment)
    update_estimates(experiment);
    // output current results (based on the eperiment)
    output_block(experiment, out);
    if (_show_progress) _progress->write(double(_counter + 1) / nblocks);
  }
  // output final results (based on the experiment)
  final_output(experiment, out);
}

void Block::reset_all() {
  // self-explainatory name
  for (string s : _observables) {
    _estimates[s] = 0;
    _sums[s] = 0;
    _sums2[s] = 0;
    _averages[s] = 0;
    _sigmas[s] = 0;
  }
  _counter = 0;
  if (_show_progress) _progress = new progress_bar{clog, 70u};
}

void Block::reset_block() {
  for (string s : _observables) _estimates[s] = 0;
}

void Block::update_block(Experiment &exp) {
  // _estimate stores whatever exp tells it to store for the current block
  for (string k : _observables) exp.update(k);
}

void Block::update_estimates(const Experiment &exp) {
  for (string s : _observables) {
    // exp reads the content of the block object and calculates the estimate
    _estimates[s] = exp.estimate(s, this);
    _sum[s] += _estimate[s];
    _sum2[s] += pow(_estimate[s], 2);
    _average[s] = _sum[s] / (_counter + 1.);
    _sigma[s] = error(_average[s], _sum2[s], _counter);
  }
}

void Block::output_block(const Experiment &exp,
                         map<string, ofstream> &out) const {
  // exp decides how to format the output of each block
  exp.output_block(this, out);
}

void Block::final_output(const Experiment &exp,
                         map<string, ofstream> &out) const {
  // exp decides how to format the final output
  exp.final_output(this, out);
}

// single observable overloads

Block::Block(string single_obs, bool progress) : _show_progress{progress} {
  vector<string> obs{single_obs};
  _observables = obs;
}

void Block::averages(int iterations, int nblocks, Experiment &experiment,
                     ofstream &out) {
  _length = iterations / nblocks;  // automatically cast to integer
  reset_all();
  for (_counter = 0; _counter < nblocks; ++_counter) {
    reset_block();
    for (int i = 0; i < _length; ++i) {
      experiment.run();
      update_block(experiment);
    }
    update_estimates(experiment);
    output_block(experiment, out);
    if (_show_progress) _progress->write(double(_counter + 1) / nblocks);
  }
  final_output(experiment, out);
  // TODO: find a way not to write this code twice
}

void Block::output_block(const Experiment &exp, ofstream &out) const {
  // let the experiment decide how to format the output of each block
  exp.output_block(this, out);
  // TODO: don't repeat yourself
}

void Block::final_output(const Experiment &exp, ofstream &out) const {
  // let the experiment decide how to format the final output
  exp.final_output(this, out);
}
