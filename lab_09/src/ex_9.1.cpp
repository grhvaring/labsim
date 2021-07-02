/****************************************************************
 Exercise 9.1: travelling salesman problem solved with a genetic
 algorithm approach
*****************************************************************/

#include "genetic_tsp.h"
#include "libsim.h"
#include "progress.h"

using namespace std;

int main(int argc, char* argv[]) {
  int nroutes = 1;
  int ncities = 32;

  if (argc > 1) nroutes = atoi(argv[1]);

  int seed[] = {0, 0, 0, 1};
  Random random(seed);

  string result_path = "../results/lab_09/";  // result folder
  map<string, ofstream> out;
  vector<string> files{"lengths", "best_average", "best_routes",
                       "worst_routes"};
  progress_bar* progress =
      new progress_bar{clog, 70u};  // progress bar displayer

  // tsp solver
  GeneticTSP tsp{nroutes, ncities, seed};
  int generations = 100;
  if (argc > 2) generations = atoi(argv[2]);

  for (string shape : {"circle", "square"}) {
    // setup
    vector<string> files_shape = add_suffix(files, "_" + shape);
    open(out, result_path, files_shape);
    tsp.generate_population();
    tsp.generate_positions(shape);

    // simulation
    for (int generation = 0; generation < generations; ++generation) {
      tsp.evolve();
      out[files_shape[0]] << tsp.route_length(0) << endl;
      out[files_shape[1]] << tsp.best_average_length() << endl;
      tsp.save_best_route(out[files_shape[2]]);
      tsp.save_worst_route(out[files_shape[3]]);
      progress->write((generation + 1.) / generations);
    }
    close(out);
  }

  return 0;
}