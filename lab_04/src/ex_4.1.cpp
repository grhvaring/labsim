/****************************************************************
 Exercise 4.1: Equilibration of a molecular dynamics simulation
*****************************************************************/

#include "linalg.h"
#include "moldyn.h"

using namespace std;

int read_steps(string phase = "dat") {
  // read first row of input.*, which contains the value for nsteps
  ifstream in("input/input." + phase);
  int nsteps, measure_every;
  in >> nsteps >> measure_every;
  in.close();
  return double(nsteps) / measure_every;
}

int main(int argc, char* argv[]) {
  // quantities to estimate
  vector<string> files{"potential", "kinetic", "total", "temperature"};

  // setup output
  string result_path = "../results/lab_04/";
  map<string, ofstream> out;
  open(out, result_path, files);

  // read number of runs for the single simulation from file input
  int runs = read_steps();

  // number of times to repeat the simulation (for equilibration)
  int reps = 20;
  if (argc > 1) reps = atoi(argv[1]);

  vector<string> phases{"sol", "liq", "gas"};

  int seed[] = {0, 0, 0, 1};
  vector<string> files_phase;
  MolecularDynamics* moldyn;

  for (string phase : phases) {
    // initialization
    files_phase = add_suffix(files, "_" + phase);
    open(out, result_path, files_phase);
    moldyn = new MolecularDynamics{files_phase, seed};
    for (int rep = 0; rep < reps; ++rep) {
      if (rep > 0) moldyn->initialize(true);
      // simulation
      moldyn->block_averages(runs, out);
      // log messages
      if (reps > 1) cout << "Iteration " << rep + 1 << " completed." << endl;
      // reset
    }
    close(out);
  }

  return 0;
}