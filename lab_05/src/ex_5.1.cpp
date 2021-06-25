/****************************************************************
 Exercise 5.1: Estimate the average value for the radius of an
 electron in an Hydrogen atom 1s and 2p orbital using the
 Metropolis algorithm to sample the two probability densities.
*****************************************************************/

#include <fstream>
#include <iostream>
#include <string>

#include "block.h"
#include "hydrogen.h"
#include "libsim.h"

int main(int argc, char* argv[]) {
  int npoints = 1e6;
  int nblocks = 100;

  string result_path = "../results/lab_05/";
  ofstream out;

  vector<double> x0{1, 1, 1};  // starting  position
  int seed[] = {0, 0, 0, 1};
  Hydrogen* hydrogen;

  // 1s orbital
  string file = "avg_r_1s";
  open(out, result_path, file);
  double semi_width = 1.2;  // gives an acceptance rate of about 50%
  hydrogen = new Hydrogen(file, seed, x0, semi_width);
  hydrogen->block_averages(npoints, nblocks, out);
  delete hydrogen;
  close(out);

  // 2p orbital
  file = "avg_r_2p";
  open(out, result_path, file);
  semi_width = 3;  // gives an acceptance rate of about 50%
  hydrogen = new Hydrogen(file, seed, x0, semi_width);
  hydrogen->block_averages(npoints, nblocks, out);
  delete hydrogen;
  close(out);

  return 0;
}