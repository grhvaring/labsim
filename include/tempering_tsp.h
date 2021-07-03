/****************************************************************
 Definition of the class TemperingTSP
*****************************************************************/

#ifndef __genetic_tsp__
#define __genetic_tsp__

#include <algorithm>
#include <array>
#include <vector>

#include "progress.h"
#include "random.h"

using namespace std;

struct Route {
  int *steps;
  int size;
  void swap(int i, int j) {
    int k = steps[i];
    steps[i] = steps[j];
    steps[j] = k;
  }
  void to_indices(int n) {
    steps = new int[n];
    for (int i = 0; i < n; ++i) steps[i] = i;
  }
  bool contains(int value, int end_search = -1) {
    if (end_search == -1) end_search = size;
    for (int i = 0; i < end_search; ++i) {
      if (steps[i] == value) return true;
    }
    return false;  // if the element was not in the searched container
  }
};

class TemperingTSP {
  Random _random;
  vector<array<double, 2>> _positions;  // nodes' positions (fixed)
  Route _route, _new_route;
  int _nsteps;
  string _shape;
  double _temperature, _accepted;

 public:
  // initialization
  TemperingTSP(int nsteps, double temperature = 10, int *seed = nullptr);
  void generate_route();
  void generate_positions(string shape, ofstream &out);
  void set_positions(const vector<array<double, 2>> &positions) {
    _positions = positions;
  }
  vector<array<double, 2>> get_positions() { return _positions; }

  // evolution
  void metropolis_move();

  // other methods
  void shuffle_route();
  int random_index(int max_not_included, int min = 0) {
    return int(_random.Rannyu(min, max_not_included));
  }
  double route_length();
  double new_length();
  void mutate_new_route();
  double boltzmann_weight(double dL) { return exp(-dL / _temperature); }
  void decrease_temperature(double dT) {
    _temperature = max(_temperature - dT, 0.);
  }
  void set_temperature(double temperature) { _temperature = temperature; }
  int periodic_bc(int index);
  void print_route();      //  debugging
  void print_new_route();  // debugging
  void save_route(ofstream &);

  // for parallelization
  int *route() { return _route.steps; }
  double temperature() { return _temperature; }
};

#endif  // __tempering_tsp__