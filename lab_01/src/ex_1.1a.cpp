/******************************************************************
 Exercise 1.1a: Generate a large sequence (M=10^5) of pseudo-random
 numbers distributed uniformly in [0,1]. Then divide it in N=100
 blocks each of lenght L, calculate the average values of these
 blocks as well as the squares of the average values, and write
 the progressive average with its uncertainty into a text file.
*******************************************************************/

#include "lib01.h"
#include "libsim.h"

using namespace std;

int main(int argc, char** argv) {
  int nthrows = 1e5;
  int nblocks = 100;

  // output setup
  string result_path = "../results/lab_01/";
  vector<string> observables{"average", "variance"};
  map<string, ofstream> out;
  open(out, result_path, observables);

  // initialization
  int seed[] = {0, 0, 0, 1};
  Uniform uniform(observables, seed);

  // simulation
  uniform.block_averages(nthrows, nblocks, out);

  close(out);

  return 0;
}