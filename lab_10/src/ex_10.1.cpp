/****************************************************************
 Exercise 10.1: TSP problem solved using simulated annealing
*****************************************************************/

#include "libsim.h"
#include "tempering_tsp.h"

int main(int argc, char* argv[]) {
  int ncities = 32;

  int seed[] = {0, 0, 0, 1};

  string result_path = "../results/lab_10/";
  map<string, ofstream> out;
  vector<string> files{"length", "route"};
  progress_bar* progress =
      new progress_bar{clog, 70u};  // progress bar displayer

  // tsp solver
  double temperature = 0.5;
  TemperingTSP tsp{ncities, temperature, seed};
  ofstream out_points;

  for (string shape : {"circle", "square"}) {
    // setup
    vector<string> files_shape = add_suffix(files, "_" + shape);
    open(out, result_path, files_shape);
    out_points.open(result_path + shape + "_points.dat");
    tsp.set_temperature(temperature);
    tsp.generate_route();
    tsp.generate_positions(shape, out_points);
    out_points.close();

    // simulation

    double temperature_steps = 20;
    if (argc > 1) temperature_steps = atof(argv[1]);
    int iterations = 2 * (pow(2, temperature_steps) - 1);
    int k = 1;
    double dT = temperature / temperature_steps;
    for (int i = 0; i < iterations; ++i) {
      tsp.metropolis_move();
      out[files_shape[0]] << tsp.route_length() << endl;
      tsp.save_route(out[files_shape[1]]);
      progress->write((i + 1.) / iterations);
      if ((i % int(pow(2, k))) == 0) {
        tsp.decrease_temperature(dT);
        ++k;
      }
    }
    close(out);
  }
  return 0;
}