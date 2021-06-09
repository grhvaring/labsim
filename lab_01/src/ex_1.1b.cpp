/****************************************************************
 Exercise 1.1c: Generate a large sequence (n=10^4) of pseudo-random
 numbers distributed uniformly in [0,1]. Then divide [0,1] in M=100
identical sub-intervals and implement the chi^2 test with respect
to a uniform distribution. Repeat for 100 iterations and write the
results in a text file to be loaded and plotted using Python.
*****************************************************************/

#include <cmath>
#include <fstream>
#include <iostream>

#include "libsim.h"
#include "progress.h"
#include "random.h"

using namespace std;

int main(int argc, char** argv) {
  int nthrows = 1e4;
  int NBINS = 500;
  int iterations = 1e4;  // 10^4 instead of 100 to better see the distribution

  if (argc > 1) NBINS = atoi(argv[1]);

  int seed[] = {0, 0, 0, 1};
  Random random(seed);

  string result_path = "../results/lab_01/";
  ofstream out;
  string filename = "chi2";
  open(out, result_path, filename);

  double r = 0;  // random number container
  double* bins;  // histogram
  double chi2 = 0, chi2_approx = 0, chi2_bin;
  double expected_counts;  // exact average value
  progress_bar progress{clog, 70u};

  for (int i = 0; i < iterations; i++) {
    for (int nbins = 100; nbins <= NBINS; nbins += 100) {
      expected_counts = nthrows / nbins;
      bins = new double[nbins];
      // reset histogram
      for (int bin = 0; bin < nbins; bin++) bins[bin] = 0;
      // fill histogram
      for (int t = 0; t < nthrows; t++) {
        r = nbins * random.Rannyu();  // random number uniform in [0,nbins)
        bins[int(r)]++;
      }
      // calculate chi2
      for (int bin = 0; bin < nbins - 1; bin++) {
        chi2_bin = pow(bins[bin] - expected_counts, 2) / expected_counts;
        chi2_approx += chi2_bin;              // variance approximated to np
        chi2 += chi2_bin / (1 - 1. / nbins);  // correct variance
        // bins[bin] = 0;  // reset histogram for the next iteration
      }
      out << chi2_approx << " " << chi2 << " ";
      chi2_approx = 0;  // reset chi2_approx for the next iteration
      chi2 = 0;         // reset chi2 for the next iteration
      delete[] bins;
      progress.write(double(i) / iterations);
    }
    out << endl;
  }

  close(out, false);

  return 0;
}