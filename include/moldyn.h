/****************************************************************
 definition of the MolecularDynamics class, which performs
 molecular dynamics by solving Newton's equations of motion
*****************************************************************/

#ifndef __moldyn__
#define __moldyn__

#include <array>
#include <iostream>
#include <string>
#include <vector>

#include "experiment.h"
#include "libsim.h"
#include "particle.h"
#include "progress.h"

using namespace std;

struct Simulation {
  bool restart;
  int nsteps, measure_every;
  double dt;
};

struct System {
  int npart;
  double temperature, density, rcut, volume, side;
  double vtail, wtail;  // tail corrections for potential energy and virial
  vector<Particle> particles;
};

double periodic_bc(double, double);
array<double, 3> periodic_bc(array<double, 3>, double);

class MolecularDynamics : public Experiment {
 protected:
  Simulation _simulation;
  System _system;
  vector<double> _g;
  int _nbins;
  map<string, string> _names;
  vector<array<double, 3>> _forces;

 public:
  MolecularDynamics(vector<string> files, int *seed, bool restart = false);
  // setup
  void initialize(bool restart = false);
  void extract_names();
  void read_input();
  string check_phase() const;
  void reset_histogram(int nbins = 100);
  void read_positions(string file = "config.0");
  void read_old_positions();
  void set_random_velocities();
  void rescale_velocities();
  void guess_old_positions();
  array<double, 3> velocity_cm() const;
  double temperature() const;
  void calculate_tail_corrections();

  // simulation
  void run();      // internal processing to perform before the measurements
  void measure();  // measurements (inside the block)
  void calculate_g(int runs, int nblocks, map<string, ofstream> &out);

  // output
  void starting_message() const;
  void final_output(map<string, ofstream> &);
  void ConfXYZ(int) {}  // not implemented

  // other methods
  void move_all();
  void calculate_forces();
  void reset_forces();
  void move(int);
  double potential_energy() const;
  double kinetic_energy() const;
  void average_g(double block_length);
  void fill_g();
  void output_g(map<string, ofstream> &);
  double virial() const;
  double pressure() const;
  double periodic_bc(double) const;
  array<double, 3> periodic_bc(const array<double, 3> &) const;
};

#endif