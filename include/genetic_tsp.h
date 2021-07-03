/****************************************************************
 Definition of the class GeneticTSP
*****************************************************************/

#ifndef __genetic_tsp__
#define __genetic_tsp__

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
  void to_indices(int size) {
    steps = new int[size + 1];
    for (int i = 0; i < size; ++i) steps[i] = i;
    steps[size] = 0;  // last = first
  }
  bool contains(int value, int end_search = -1) {
    if (end_search == -1) end_search = size;
    for (int i = 0; i < end_search; ++i) {
      if (steps[i] == value) return true;
    }
    return false;  // if the element was not in the searched container
  }
};

class GeneticTSP {
  Random _random;
  vector<array<double, 2>> _positions;  // nodes' positions (fixed)
  vector<Route> _routes, _children;
  int _nsteps, _nroutes, _cut;
  double _mutation_probability;
  string _shape;

 public:
  // initialization
  GeneticTSP(int nroutes, int nsteps, int *seed = nullptr);
  void generate_population();
  void generate_positions(string shape = "circle");
  void set_positions(const vector<array<double, 2>> &positions) {
    _positions = positions;
  }

  // evolution
  void evolve();
  void generate_children(int index1, int index2);
  void mutate(int i);
  void select(int index1, int index2);

  // other methods
  void shuffle(int route);
  int random_index(int max_not_included, int min = 0) {
    return int(_random.Rannyu(min, max_not_included));
  }
  void complete_child(int index1, int index2);
  double route_length(int route);
  double child_length(int route);
  double shortest_length();
  double best_average_length();
  void sort_by_length(int left, int right);
  void merge(int left, int right);
  int periodic_bc(int index);
  void print(int route);
  void print_child(int route);
  void save_best_route(ofstream &out);
  void save_worst_route(ofstream &out);

  // for parallelization
  int *route(int i) { return _routes[i].steps; }
};

#endif  // __genetic_tsp__