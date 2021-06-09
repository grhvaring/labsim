/****************************************************************
 Exercise 1.3: Simulate Buffon's experiment.
 Take a needle of length L and throw it at random onto a horizontal
 plane ruled with straight lines a distance d apart. The probability
 P that the needle will intersect one of the lines is: P = 2L/(π*d).
 Remark: choose L < d, but not L << d otherwise P << 1.
 This could be used to evaluate π from throws of the needle: if the
 needle is thrown down N_throws times and is observed to land on a
 line N_hits of those times, we can make the estimate
 π = 2L/(P*d) = lim_(N_throws -> infty) 2L*N_throws/(N_hits*d)
*****************************************************************/

#include <fstream>
#include <map>
#include <string>

#include "block.h"
#include "lib01.h"
#include "libsim.h"

using namespace std;

int main(int argc, char *argv[]) {
  int nthrows = 1e5;
  int nblocks = 100;

  // output setup
  string result_path = "../results/lab_01/";
  string filename = "pi_estimate";
  ofstream out;
  open(out, result_path, filename);

  // experiment settings
  double L = 1.;      // length of the needle
  double ratio = 5.;  // (grid spacing) / L

  // allowing for custom settings from cl
  if (argc > 1) nthrows = atoi(argv[1]);
  if (argc > 3) ratio = atoi(argv[3]);

  double d = ratio * L;  // grid spacing

  map<string, double> args;
  args["L"] = L;
  args["d"] = d;
  int seed[] = {0, 0, 0, 1};

  Buffon buffon{filename, seed, args};

  // simulation
  buffon.block_averages(nthrows, nblocks, out);

  close(out);

  return 0;
}