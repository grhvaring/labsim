/****************************************************************
 Exercise 5.0: Test the Metropolis class by sampling the Hydrogen
 atom 1s and 2p orbitals probability densities
*****************************************************************/

#include <cmath>
#include <fstream>
#include <iostream>
#include <string>

#include "hydrogen.h"
#include "libsim.h"
#include "linalg.h"
#include "metropolis.h"

using namespace std;

int main(int argc, char *argv[]) {
  int npoints = 1e4;

  string result_path = "../results/lab_05/";
  vector<string> filenames{"distribution_1s", "distribution_2p"};
  map<string, ofstream> out;
  open(out, result_path, filenames, true);

  vector<double> x{1, 1, 1}, old_x;
  int seed[] = {0, 0, 0, 1};

  Metropolis metropolis(x, seed);

  double delta = 1.2;

  if (argc > 1) npoints = atof(argv[1]);
  if (argc > 2) delta = atof(argv[2]);

  double avg = 0;
  for (int i = 0; i < npoints; i++) {
    old_x = x;
    x = metropolis.uniform_mrt2(psi_1s, delta);
    avg += norm(x);
    out[filenames[0]] << x[0] << " " << x[1] << " " << x[2] << endl;
  }

  cout << "<r>_1s = " << avg / npoints
       << ", alpha_s = " << metropolis.alpha() / npoints << endl;

  avg = 0;

  vector<double> nx{1, 1, 1};
  metropolis.reset(nx);

  delta = 3;

  for (int i = 0; i < npoints; i++) {
    old_x = x;
    x = metropolis.uniform_mrt2(psi_2p, delta);
    avg += norm(x);
    out[filenames[1]] << x[0] << " " << x[1] << " " << x[2] << endl;
  }

  cout << "<r>_2p = " << avg / npoints
       << ", alpha_p = " << metropolis.alpha() / npoints << endl;

  close(out, false);

  return 0;
}