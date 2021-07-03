/****************************************************************
 definition of the class canonicalMC, that simulates the canonical
 ensemble of statistical physics using Monte Carlo methods
*****************************************************************/

#ifndef __canonicalMC__
#define __canonicalMC__

#include "experiment.h"
#include "libsim.h"

using namespace std;

struct mcSystem {
  int npart;
  double temperature, density, rcut, volume, side;
  double vtail, wtail;  // tail corrections for potential energy and virial
  vector<array<double, 3>> positions, new_positions;
};

double periodic_bc(double, double);
array<double, 3> periodic_bc(array<double, 3>, double);

class canonicalMC : public Experiment {
 protected:
  mcSystem _system;
  vector<double> _g;
  int _nbins;
  double _delta;
  bool _restart;
  int _accepted, _attempted;
  map<string, string> _names;

 public:
  canonicalMC(vector<string> files, int *seed, bool restart = false);
  // setup
  void initialize(bool restart = false);
  void extract_names();
  void read_input();
  string check_phase() const;
  void reset_histogram();
  void read_positions(string file = "config.0");
  void calculate_tail_corrections();

  // simulation
  void run();      // internal processing to perform before the measurements
  void measure();  // measurements (inside the block)
  void calculate_g(int runs, int nblocks, map<string, ofstream> &out);
  void average_g(double);

  // output
  void starting_message();
  void output_g(map<string, ofstream> &);
  void final_output(map<string, ofstream> &);

  // other methods
  void move(int);
  int random_index() { return int(_system.npart * _random.Rannyu()); }
  double boltzmann_weight(double dE) { return exp(-dE / _system.temperature); }
  double energy(int index, bool new_position = false);
  double potential_energy() const;
  double virial() const;
  double pressure() const;
  void fill_g();
  double periodic_bc(double) const;
  array<double, 3> periodic_bc(const array<double, 3> &) const;
};

#endif  // __mcmoldyn__