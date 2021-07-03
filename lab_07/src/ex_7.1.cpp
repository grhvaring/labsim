/****************************************************************
 Exercise 7.1: Equilibration of the MC NVT code
*****************************************************************/

#include "canonicalMC.h"

using namespace std;

int main(int argc, char* argv[]) {
  int nsteps = 5 * 1e5;
  int nblocks = 100;

  if (argc > 1) {
    nsteps = atoi(argv[1]);
    nblocks = nsteps;
  }
  if (argc > 2) nblocks = atoi(argv[2]);

  // output setup
  string result_path = "../results/lab_07/";  // result folder
  vector<string> measure{"potential", "pressure"};
  map<string, ofstream> out;

  // initialization
  int seed[] = {0, 0, 0, 1};
  vector<string> phases{"liq", "gas"};
  vector<string> files_phase;
  canonicalMC* mcNVT;

  for (string phase : phases) {
    // initialization
    files_phase = add_suffix(measure, "_" + phase);
    open(out, result_path, files_phase);
    mcNVT = new canonicalMC{files_phase, seed};
    // equilibration
    mcNVT->block_averages(nsteps, nsteps, out);

    close(out);
  }

  return 0;
}