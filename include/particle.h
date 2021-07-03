/****************************************************************
 definition of the struct Particle
*****************************************************************/

#ifndef __particle__
#define __particle__

#include <array>
#include <fstream>

using namespace std;

double periodic_BC(double, double);
array<double, 3> periodic_BC(const array<double, 3> &, double);

struct Particle {
  array<double, 3> r, v, r_old, r_new;
  Particle();
  Particle(double, double, double);
  Particle(double, double, double, double, double, double);
  Particle(double *);
  Particle(double *, double *);
  Particle(array<double, 3>);
  Particle(array<double, 3>, array<double, 3>);
  Particle(const Particle &);
  ~Particle();
  void reset();
  // Particle &operator=(const Particle &);
  void write_position(ofstream &) const;
};

#endif