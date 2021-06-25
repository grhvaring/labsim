/****************************************************************
 Exercise 2.2: Simulate a 3D random walk in a cubic lattice with
 lattice constant a=1, where at each discrete time step the walker
 makes a forward or backward step of equal length a in one of the
 3 principal directions of the lattice: x, y or z.

 Then simulate a 3D RW in a continuous 3D space, where at each
 discrete time step the walker makes a step of equal length a=1
 along a random direction, obtained by sampling uniformly the solid
 angle: theta in [0,pi] and phi in [0,2*pi].

 For both configurations, repeat M=10^4 times and write to a file
 the value of the "mean square distance" sqrt(<|r_N|^2>) at every
 step N in [0,100], with its statistical uncertainty.
*****************************************************************/

#include <array>
#include <cmath>
#include <fstream>
#include <string>

#include "lib02.h"
#include "libsim.h"

using namespace std;

int main(int argc, char** argv) {
  int iterations = 1e4;
  int nblocks = 100;
  int nsteps = 100;

  if (argc > 2) {
    iterations = atoi(argv[1]);
    nblocks = atoi(argv[2]);
  }

  if (iterations < 1e3) nblocks = 10;

  // output setup
  string result_path = "../results/lab_02/";
  ofstream out;

  // each step in [0, nsteps] corresponds to one observable
  vector<string> steps;
  for (int step = 0; step <= nsteps; ++step) steps.push_back(to_string(step));

  int seed[] = {0, 0, 0, 1};

  string filename = "discrete_rw";
  open(out, result_path, filename);

  DiscreteRW discrete_rw{steps, seed};
  discrete_rw.block_averages(iterations, nblocks, out);
  close(out);

  filename = "continuous_rw";
  open(out, result_path, filename);

  ContinuousRW continuous_rw{steps, seed};
  continuous_rw.block_averages(iterations, nblocks, out);
  close(out);

  return 0;
}
