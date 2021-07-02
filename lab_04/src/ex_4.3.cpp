/****************************************************************
 Exercise 4.3: After having the system equilibrate (4.1) calculate
 quantities of interest such as the temperature, and the potential,
 kinetic, and internal energy per particle. (+ pressure optionally)
 Calculate all the quantities using block averages.
*****************************************************************/

#include <fstream>
#include <map>
#include <string>
#include <vector>

#include "moldyn.h"

using namespace std;

int main(int argc, char* argv[]) {
  int runs = 1e4;
  int nblocks = 100;

  // output setup
  string result_path = "../results/lab_04/";  // result folder
  vector<string> files{"potential", "kinetic", "total", "temperature",
                       "pressure"};
  files = add_prefix(files, "avg_");
  map<string, ofstream> out;

  // initialization
  vector<string> phases{"sol", "liq", "gas"};
  int seed[] = {0, 0, 0, 1};
  vector<string> files_phase;
  MolecularDynamics* moldyn;

  // simulation
  for (string phase : phases) {
    files_phase = add_suffix(files, "_" + phase);
    open(out, result_path, files_phase);
    moldyn = new MolecularDynamics(files_phase, seed, true);
    moldyn->block_averages(runs, nblocks, out);
    close(out);
  }

  return 0;
}