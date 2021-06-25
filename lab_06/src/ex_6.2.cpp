/****************************************************************
 Exercise 6.1: Simulation of a 1D Ising model using MC methods with
 2 sampling algorithms: Metropolis and Gibbs.
 This program calculates the magnetization with non zero external
 magnetic field.
 *****************************************************************/

#include <iostream>

#include "ising.h"
#include "libsim.h"

using namespace std;

int main(int argc, char* argv[]) {
  // remember to set h = 0.02 in input.dat!
  int iterations = 1e5;
  int nblocks = 20;

  string result_path = "../results/lab_06/";

  // output setup
  vector<string> equilibrate{"magnetization_eq"};
  map<string, ofstream> out;
  open(out, result_path, equilibrate);

  // initialization
  int seed[] = {0, 0, 0, 1};
  double T_eq = 1.5;
  Ising* ising = new Ising{equilibrate, seed, T_eq};
  ising->starting_message();

  // equilibration
  ising->block_averages(iterations, iterations / 100, out);
  close(out, false);

  // simulation
  vector<string> measure{"magnetization"};

  // gibbs
  vector<string> gibbs = add_suffix(measure, "_gibbs");
  open(out, result_path, gibbs);
  progress_bar* progress = new progress_bar{clog, 70u};
  double steps = 100;
  if (argc > 1) steps = atoi(argv[1]);
  double T_initial = 0.1;
  double T_final = 3;
  double increment = (T_final - T_initial) / steps;
  for (double T = T_initial; T <= T_final; T += increment) {
    ising = new Ising{gibbs, seed, T};
    ising->block_averages(iterations, nblocks, out);
    progress->write((T - T_initial) / (T_final - T_initial));
  }

  close(out, false);

  return 0;
}