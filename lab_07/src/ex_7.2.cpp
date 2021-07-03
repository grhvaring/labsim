/****************************************************************
 Exercise 7.2: Calculation of the radial distribution function
*****************************************************************/

#include "canonicalMC.h"
#include "moldyn.h"

using namespace std;

int main(int argc, char* argv[]) {
  int nsteps = 5 * 1e4;
  int nblocks = 100;

  if (argc > 1) {
    nsteps = atoi(argv[1]);
  }

  // output setup
  string result_path = "../results/lab_07/";  // result folder
  vector<string> measure{"radial_dist"};
  map<string, ofstream> out;

  // initialization
  int seed[] = {0, 0, 0, 1};
  vector<string> phases{"gas"};
  vector<string> file_phase;
  canonicalMC* mcNVT;

  for (string phase : phases) {
    // initialization
    file_phase = add_suffix(measure, "_" + phase);
    open(out, result_path, file_phase);
    bool restart = true;
    mcNVT = new canonicalMC{file_phase, seed, restart};
    // simulation
    mcNVT->calculate_g(nsteps, nblocks, out);
    close(out);
  }

  return 0;
}