/****************************************************************
 Exercise 8.1: Use the variational quantum monte carlo method to
 find the values of sigma and mu that minimize the energy
*****************************************************************/

#include "quantum_mc.h"

using namespace std;

int read_steps(string phase = "dat") {
  // read first row of input.*, which contains the value for nsteps
  // TO CHANGE
  ifstream in("input/input." + phase);
  int nsteps, measure_every;
  in >> nsteps >> measure_every;
  in.close();
  return double(nsteps) / measure_every;
}

int main(int argc, char* argv[]) {
  // quantities to estimate
  vector<string> file{"minimization_energy"};

  // setup output
  string result_path = "../results/lab_08/";
  map<string, ofstream> out;
  open(out, result_path, file);

  // initialization
  int seed[] = {0, 0, 0, 1};
  double x0 = 1, semi_width = 2;

  QuantumMC* qmc = new QuantumMC{file, seed, x0};

  // minimization
  int runs = 1e5;
  int nblocks = 100;
  int nsteps = 100;

  double sigma_min = 0.55, sigma_max = 0.7,
         dsigma = (sigma_max - sigma_min) / (nsteps - 1);
  double mu_min = 0.74, mu_max = 0.86, dmu = (mu_max - mu_min) / (nsteps - 1);
  double mu_opt = mu_min, sigma_opt = sigma_min;

  double min_energy = 10, energy;

  progress_bar progress{clog, 70u};

  for (double sigma = sigma_min; sigma < sigma_max + 0.5 * dsigma;
       sigma += dsigma) {
    for (double mu = mu_min; mu < mu_max + 0.5 * dmu; mu += dmu) {
      qmc->set_parameters(sigma, mu);
      qmc->block_averages(runs, nblocks, out);
      energy = qmc->energy();
      // cout << "acceptance = " << qmc.acceptance() << endl;
      if (energy < min_energy) {
        min_energy = energy;
        sigma_opt = sigma;
        mu_opt = mu;
      }
    }
    progress.write((sigma - sigma_min) / (sigma_max - sigma_min));
  }

  cout << "E_min = " << min_energy << endl;
  cout << "sigma_opt = " << sigma_opt << endl;
  cout << "mu_opt = " << mu_opt << endl;

  delete qmc;
  close(out);

  // estimation of ground energy and ground state

  mu_opt = 0.812727;
  sigma_opt = 0.633333;
  runs = 1e6;
  nblocks = 100;
  file[0] = "ground_energy";
  open(out, result_path, file);
  qmc = new QuantumMC{file, seed, x0};
  qmc->set_parameters(sigma_opt, mu_opt);
  qmc->block_averages(runs, nblocks, out);

  close(out);

  return 0;
}