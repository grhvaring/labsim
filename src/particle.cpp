/****************************************************************
 implementation of the class Particle
*****************************************************************/

#include "particle.h"

#include <cmath>

#include "linalg.h"

double periodic_BC(double x, double L) {
  // keep x within a box of side L using periodic boundary conditions
  return x - L * rint(x / L);
}

array<double, 3> periodic_BC(const array<double, 3> &x, double L) {
  // keep x within a box of side L using periodic boundary conditions
  array<double, 3> x_in;
  for (int i = 0; i < 3; ++i) x_in[i] = periodic_BC(x[i], L);
  return x_in;
}

Particle::Particle() { reset(); }

Particle::Particle(double x, double y, double z) {
  r[0] = x;
  r[1] = y;
  r[2] = z;
  v.fill(0);
}

Particle::Particle(double x, double y, double z, double vx, double vy,
                   double vz) {
  r[0] = x;
  r[1] = y;
  r[2] = z;
  v[0] = vx;
  v[1] = vy;
  v[2] = vz;
}

Particle::Particle(double *r0) {
  for (int i = 0; i < 3; i++) r[i] = r0[i];
  v.fill(0);
}

Particle::Particle(double *r0, double *v0) {
  for (int i = 0; i < 3; i++) {
    r[i] = r0[i];
    v[i] = v0[i];
  }
}

Particle::Particle(array<double, 3> r0) : r(r0) { v.fill(0); }

Particle::Particle(array<double, 3> r0, array<double, 3> v0) : r(r0), v(v0) {}

Particle::Particle(const Particle &p) {
  r = p.r;
  v = p.v;
  r_old = p.r_old;
  r_new = p.r_new;
}

Particle::~Particle() {}

void Particle::reset() {
  r.fill(0);
  v.fill(0);
  r_old.fill(0);
  r_new.fill(0);
}

/*Particle &operator=(const Particle &p) {
  Particle q(p);
  return *q;
}*/

void Particle::write_position(ofstream &out) const {
  out << r[0] << "   " << r[1] << "   " << r[2] << endl;
}