/****************************************************************
 Implementation of the class GeneticTSP
*****************************************************************/

#include "genetic_tsp.h"

#include <algorithm>
#include <iostream>

#include "linalg.h"

// initialization

GeneticTSP::GeneticTSP(int nroutes, int nsteps, int *seed)
    : _nroutes{nroutes}, _nsteps{nsteps}, _random{seed} {
  _cut = int(0.6 * nsteps);  // hard-coded for now, could be passed to the class
  _mutation_probability = 0.1;  // same
  generate_population();
}

void GeneticTSP::generate_population() {
  _routes.clear();
  Route route;
  for (int route_index = 0; route_index < _nroutes; ++route_index) {
    // generate a random permutation of indices as the n-th route
    _routes.push_back(route);
    _routes[route_index].to_indices(_nsteps);
    shuffle(route_index);
    _children.push_back(route);  // initialize children to something
    _children[route_index].to_indices(_nsteps);
  }
}

void GeneticTSP::generate_positions(string shape) {
  // description
  _positions.clear();
  _shape = shape;
  ofstream out{"../results/lab_09/" + shape + "_points.dat"};
  double x, y;
  if (shape == "circle") {
    double theta = 0;
    for (int n = 0; n < _nsteps; ++n, theta += 2 * M_PI / _nsteps) {
      x = cos(theta);
      y = sin(theta);
      _positions.push_back({x, y});
      out << x << " " << y << endl;
    }
  } else if (shape == "square") {
    for (int i = 0; i < _nsteps; ++i) {
      x = _random.Rannyu();
      y = _random.Rannyu();
      _positions.push_back({x, y});
      out << x << " " << y << endl;
    }
  }
  _positions.push_back(_positions[0]);  // last point === first point
  out << _positions[0][0] << " " << _positions[0][1] << endl;
  out.close();
}

// evolution
void GeneticTSP::evolve() {
  // generate two children for every couple and keep the 2 most fit between
  // parents and children
  int index1, index2;
  for (int route = 0; route < _nroutes / 2; ++route) {
    // pick two parents at random (can be improved)
    index1 = random_index(_nroutes);
    do {
      index2 = random_index(_nroutes);
    } while (index2 == index1);
    generate_children(index1, index2);
    mutate(index1);
    mutate(index2);
    select(index1, index2);
  }
  sort_by_length(0, _nroutes - 1);
}

void GeneticTSP::generate_children(int index1, int index2) {
  for (int i = 0; i < _cut; ++i) {
    _children[index1].steps[i] = _routes[index1].steps[i];
    _children[index2].steps[i] = _routes[index2].steps[i];
  }
  complete_child(index1, index2);
  complete_child(index2, index1);
}

void GeneticTSP::complete_child(int index1, int index2) {
  int parent1, parent2;
  int shift = _cut;
  for (int step2 = 1; step2 < _nsteps || _cut + shift < _nsteps; ++step2) {
    parent2 = _routes[index2].steps[step2];
    for (int step1 = _cut; step1 < _nsteps; ++step1) {
      parent1 = _routes[index1].steps[step1];
      if (parent2 == parent1) {
        _children[index1].steps[shift++] = parent2;
        continue;
      }
    }
  }
}

void GeneticTSP::mutate(int index) {
  if (_random.Rannyu() < _mutation_probability) {
    // random permutation (with a non zero chance of getting two identical
    // indices) of a random subset of the cities
    int start = _random.Rannyu(1, _nsteps);
    int end = _random.Rannyu(start, _nsteps);
    for (int i = 0; i < 0.5 * _random.Rannyu(0, end - start); ++i)
      _children[index].swap(random_index(end, start), random_index(end, start));
  }
  if (_random.Rannyu() < _mutation_probability) {
    // random inversion of a section of the route
    int start = _random.Rannyu(1, _nsteps);
    int end = _random.Rannyu(start, _nsteps);
    reverse(_children[index].steps + start, _children[index].steps + end);
  }
}

void GeneticTSP::select(int index1, int index2) {
  // select one parent and one child to survive based on their length

  // best parent suvives
  Route route = (route_length(index1) < route_length(index2)) ? _routes[index1]
                                                              : _routes[index2];

  // best child survives
  Route child = (child_length(index1) < child_length(index2))
                    ? _children[index1]
                    : _children[index2];
  for (int i = 0; i < _nsteps; ++i) {
    _routes[index1].steps[i] = route.steps[i];
    _routes[index2].steps[i] = child.steps[i];
  }
}

// other methods

void GeneticTSP::shuffle(int route) {
  // generate a random permutation of the wanted route, with fixed starting
  // point
  for (int step = 1; step < _nsteps; ++step) {
    // permutation obtained by a chain of swaps
    _routes[route].swap(step, random_index(_nsteps, 1));
  }
}

double GeneticTSP::route_length(int route) {
  // assume the point after the last corresponds to the first one
  double L2 = 0, dr2;
  int curr, next;
  for (int i = 0; i < _nsteps; ++i) {
    curr = _routes[route].steps[i];
    next = _routes[route].steps[i + 1];
    dr2 = norm_squared(_positions[curr] - _positions[next]);
    L2 += dr2;
  }
  return L2;
}

double GeneticTSP::child_length(int child) {
  // repeating yourself is not very elegant...
  double L2 = 0, dr2;
  int curr, next;
  for (int i = 0; i < _nsteps; ++i) {
    curr = _children[child].steps[i];
    next = _children[child].steps[i + 1];
    dr2 = norm_squared(_positions[curr] - _positions[next]);
    L2 += dr2;
  }
  return L2;
}

double GeneticTSP::best_average_length() {
  // assume routes already ordered by increasing length
  double average = 0;
  int half = _nroutes / 2;
  for (int i = 0; i < half; ++i) {
    average += route_length(i);
  }
  return average / half;
}

void GeneticTSP::sort_by_length(int left, int right) {
  // merge sort algorithm
  if (left < right) {
    sort_by_length(left, (left + right) / 2);
    sort_by_length((left + right) / 2 + 1, right);
    merge(left, right);
  }
}

void GeneticTSP::merge(int left, int right) {
  int midpoint = (left + right) / 2;
  int i = left;
  int j = midpoint + 1;
  vector<Route> temp;
  while (i <= midpoint && j <= right) {
    if (route_length(i) < route_length(j))
      temp.push_back(_routes[i++]);
    else
      temp.push_back(_routes[j++]);
  }
  for (; i <= midpoint; ++i) temp.push_back(_routes[i]);
  for (; j <= right; ++j) temp.push_back(_routes[j]);

  int k = 0;
  for (i = left; i <= right; ++i, ++k) _routes[i] = temp[k];
}

int GeneticTSP::periodic_bc(int index) {
  int shift = 0;
  if (index > _nsteps) shift = _nsteps;
  return index - shift;
}

void GeneticTSP::print(int route) {
  cout << "Route number " << route + 1 << ": [ ";
  int step = 0;
  for (; step < _nsteps - 1; ++step) cout << _routes[route].steps[step] << ", ";
  cout << _routes[route].steps[step] << " ]" << endl;
}

void GeneticTSP::print_child(int route) {
  cout << "Child route number " << route + 1 << ": [ ";
  int step = 0;
  for (; step < _nsteps - 1; ++step)
    cout << _children[route].steps[step] << ", ";
  cout << _children[route].steps[step] << " ]" << endl;
}

void GeneticTSP::save_best_route(ofstream &out) {
  for (int i = 0; i < _nsteps; ++i) out << _routes[0].steps[i] << " ";
  out << endl;
}

void GeneticTSP::save_worst_route(ofstream &out) {
  for (int i = 0; i < _nsteps; ++i)
    out << _routes[_nroutes - 1].steps[i] << " ";
  out << endl;
}