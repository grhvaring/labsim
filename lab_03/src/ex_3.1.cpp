/****************************************************************
 Exercise 3.1: Black and Sholes put/option prices simulation
*****************************************************************/

#include <fstream>
#include <map>
#include <string>

#include "blacksholes.h"
#include "libsim.h"

using namespace std;

int main(int argc, char* argv[]) {
  int iterations = 1e5;
  int nblocks = 100;

  // output setup
  string result_path = "../results/lab_03/";
  vector<string> filenames{"call_direct", "call_discrete", "put_direct",
                           "put_discrete"};
  map<string, ofstream> out;
  open(out, result_path, filenames);

  // BnS arguments
  double S0 = 100, T = 1, K = 100, r = 0.1, sigma = 0.25, nsteps = 100;
  BNS_args parameters = {S0, T, K, r, sigma, nsteps};

  // initialization
  int seed[] = {0, 0, 0, 1};
  BlackNSholes bns{filenames, seed, parameters};

  // simulation
  bns.block_averages(iterations, nblocks, out);

  close(out);

  return 0;
}