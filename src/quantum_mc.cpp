/****************************************************************
 Implementation of the class QuantumMC
*****************************************************************/

#include "quantum_mc.h"

#include "linalg.h"

double normal(double y) { return exp(-0.5 * y * y); }
double A(double q) { return min(1., q); }

void QuantumMC::set_parameters(double sigma, double mu) {
  _sigma = sigma;
  _mu = mu;
  _semi_width = 2 * sigma;
  _acceptance = 0;
  initialize_histograms();
}

void QuantumMC::initialize_histograms(int nbins, double semi_width) {
  _nbins = nbins;
  _semi_width_histogram = semi_width;
  _histogram.resize(nbins, 0);
  _sum_histogram.resize(nbins, 0);
  _sum2_histogram.resize(nbins, 0);
  _error_histogram.resize(nbins, 0);
  _mean_histogram.resize(nbins, 0);
}

void QuantumMC::measure() {
  double x = metropolis_step();
  string s = _observables[0];
  estimate(s) += kinetic_term(x) + potential_term(x);
  fill_histogram(x);
}

double QuantumMC::metropolis_step() {
  _x_new = _random.Rannyu(_x - _semi_width, _x + _semi_width);
  double q = psi_trial_squared(_x_new) / psi_trial_squared(_x);
  if (_random.Rannyu() <= A(q)) _x = _x_new;
  _acceptance += A(q);
  return _x;
}

double QuantumMC::kinetic_term(double x) {
  return -0.5 * relative_d2_psi_trial(x);  // hbar = m = 1
}

double QuantumMC::potential_term(double x) {
  // potential with two symmetric wells
  return pow(x, 4) - 2.5 * pow(x, 2);
}

void QuantumMC::fill_histogram(double x) {
  // increase bin corresponding to x by one
  double bin_width = 2. * _semi_width_histogram / _nbins;
  int index = 0;
  for (double xi = -_semi_width_histogram; xi < _semi_width_histogram;
       xi += bin_width, ++index) {
    if (xi <= x && x < xi + bin_width) {
      _histogram[index]++;
      return;
    }
  }
}

void QuantumMC::update_histogram() {
  double bin_width = 2. * _semi_width_histogram / _nbins;
  _histogram /= bin_width * _block_length;  // normalization
  for (int i = 0; i < _nbins; ++i) {
    _sum_histogram[i] += _histogram[i];
    _sum2_histogram[i] += _histogram[i] * _histogram[i];
  }
  _histogram.resize(_nbins, 0);
}

void QuantumMC::finalize_histogram() {
  for (int i = 0; i < _nbins; ++i) {
    _mean_histogram[i] = _sum_histogram[i] / double(_block_counter);
    _error_histogram[i] = sqrt((_sum2_histogram[i] / double(_block_counter) -
                                _mean_histogram[i] * _mean_histogram[i]) /
                               (_block_counter - 1.));
  }
}

double QuantumMC::psi_trial(double x) {
  // trial wave function (not normalized)
  double ym = (x - _mu) / _sigma;
  double yp = (x + _mu) / _sigma;
  return normal(ym) + normal(yp);
}

double QuantumMC::relative_d2_psi_trial(double x) {
  // return second derivative of psi_trial, divided by psi_trial
  double ym = (x - _mu) / _sigma;
  double yp = (x + _mu) / _sigma;
  double unsimplified_term = ym * ym * normal(ym) + yp * yp * normal(yp);
  return 1 / pow(_sigma, 2) * (unsimplified_term / psi_trial(x) - 1);
}

// results

void QuantumMC::output_block(map<string, ofstream> &out) {
  // print to output the block results for the optimized parameters
  if (!_minimize) {
    Experiment::output_block(out);
    update_histogram();
  }
}

void QuantumMC::output_histogram() {
  ofstream out_histogram{"../results/lab_08/psi_sampled.dat"};
  double x;
  double bin_width = 2 * _semi_width_histogram / _nbins;
  for (int i = 0; i < _nbins; ++i) {
    x = -_semi_width_histogram + (i + 0.5) * bin_width;
    out_histogram << x << ' ' << _mean_histogram[i] << ' '
                  << _error_histogram[i] << endl;
  }
}

void QuantumMC::final_output(map<string, ofstream> &out) {
  // print to output the final results in the minimization phase
  if (_minimize) {
    string s = _observables[0];
    out[s] << _mu << " " << _sigma << " " << average(s) << " " << sigma(s)
           << endl;
  } else {
    finalize_histogram();
    output_histogram();
  }
  _acceptance /= _block_length * _block_counter;
}
