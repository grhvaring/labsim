/****************************************************************
 Exercise 2.1: Calculate the integral of pi/2 * cos(x*pi/2) between
 0 and 1 using a Monte Carlo method with uniform distribution.
 Write into a file the progressive estimate of the integral and its
 statistical uncertainty calculated dividing the total number of
 ``throws'' M>=10^4 into N=100 blocks.
 Then do the same using importance sampling.
*****************************************************************/

#include <cmath>
#include <fstream>
#include <string>

#include "lib02.h"
#include "libsim.h"

using namespace std;

double f(double x) { return M_PI_2 * cos(M_PI_2 * x); }
double p(double x) { return 2. * (1. - x); }

int main(int argc, char *argv[]) {
  int npoints = 1e4;
  int nblocks = 100;

  // output setup
  string result_path = "../results/lab_02/";
  vector<string> files{"uniform_integral", "importance_integral"};
  map<string, ofstream> out;
  open(out, result_path, files);

  // initialization
  int seed[] = {0, 0, 0, 1};
  Integral integral{files, seed};
  integral.set_f(f);
  double pmax = p(0);
  integral.set_p(p, pmax);

  // simulation
  integral.block_averages(npoints, nblocks, out);

  close(out);

  return 0;
}