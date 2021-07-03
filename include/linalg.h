#ifndef _linalg_h_
#define _linalg_h_

#include <array>
#include <cassert>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

// vector operators overloadings

template <typename T>
// sum of generic vectors
vector<T> operator+(const vector<T> &a, const vector<T> &b) {
  assert(a.size() == b.size());
  vector<T> result(a.size());
  for (int i = 0; i < int(a.size()); i++) result[i] = a[i] + b[i];
  return result;
}

template <typename T>
vector<T> operator-(const vector<T> &a) {
  vector<T> result(a.size());
  for (int i = 0; i < static_cast<int>(a.size()); i++) result[i] = -a[i];
  return result;
}

template <typename T>
vector<T> operator-(const vector<T> &a, const vector<T> &b) {
  return a + (-b);
}

template <typename T>
void operator+=(vector<T> &a, const vector<T> &b) {
  a = a + b;
}

template <typename T>
void operator-=(vector<T> &a, const vector<T> &b) {
  a = a - b;
}

template <typename T>
// inner product
T operator*(const vector<T> &a, const vector<T> &b) {
  assert(a.size() == b.size());
  T result = 0;
  for (int i = 0; i < int(a.size()); i++) result += a[i] * b[i];
  return result;
}

template <typename T>
// division term by term
vector<T> operator/(const vector<T> &a, const vector<T> &b) {
  assert(a.size() == b.size());
  vector<double> result(a.size());
  for (int i = 0; i < static_cast<int>(a.size()); i++)
    result[i] = (b[i] != 0) ? a[i] / b[i] : 0;
  return result;
}

template <typename T>
void operator/=(vector<T> &a, const vector<T> &b) {
  a = a / b;
}

template <typename T>
vector<T> operator/(T alpha, const vector<T> v) {
  vector<double> result(v.size());
  for (int i = 0; i < v.size(); i++) result[i] = (v[i] != 0) ? alpha / v[i] : 0;
  return result;
}

template <typename T>
// product with a scalar
vector<T> operator*(T alpha, const vector<T> &v) {
  vector<T> result(v.size());
  for (int i = 0; i < static_cast<int>(v.size()); i++) result[i] = alpha * v[i];
  return result;
}

template <typename T>
vector<T> operator*(const vector<T> &v, T alpha) {
  return alpha * v;
}

template <typename T>
vector<T> operator/(const vector<T> &v, T alpha) {
  assert(alpha != 0);
  return v * (1. / alpha);
}

template <typename T>
void operator*=(vector<T> &v, T alpha) {
  v = v * alpha;
}

template <typename T>
void operator/=(vector<T> &v, T alpha) {
  v = v / alpha;
}

// array operators overloadings

template <typename T, size_t N>
// sum of generic vectors
array<T, N> operator+(const array<T, N> &a, const array<T, N> &b) {
  array<T, N> result;
  for (int i = 0; i < N; i++) result[i] = a[i] + b[i];
  return result;
}

template <typename T, size_t N>
array<T, N> operator-(const array<T, N> &a) {
  array<T, N> result;
  for (int i = 0; i < N; i++) result[i] = -a[i];
  return result;
}

template <typename T, size_t N>
array<T, N> operator-(const array<T, N> &a, const array<T, N> &b) {
  return a + (-b);
}

template <typename T, size_t N>
void operator+=(array<T, N> &a, const array<T, N> &b) {
  a = a + b;
}

template <typename T, size_t N>
void operator-=(array<T, N> &a, const array<T, N> &b) {
  a = a - b;
}

template <typename T, size_t N>
// inner product
T operator*(const array<T, N> &a, const array<T, N> &b) {
  T result = 0;
  for (int i = 0; i < N; i++) result += a[i] * b[i];
  return result;
}

template <typename T, size_t N>
// division term by term
array<T, N> operator/(const array<T, N> &a, const array<T, N> &b) {
  array<T, N> result;
  for (int i = 0; i < N; i++) result[i] = (b[i] != 0) ? a[i] / b[i] : 0;
  return result;
}

template <typename T, size_t N>
void operator/=(array<T, N> &a, const array<T, N> &b) {
  a = a / b;
}

template <typename T, size_t N>
// product with a scalar
array<T, N> operator*(T alpha, const array<T, N> &v) {
  array<T, N> result;
  for (int i = 0; i < N; i++) result[i] = alpha * v[i];
  return result;
}

template <typename T, size_t N>
array<T, N> operator*(const array<T, N> &v, T alpha) {
  return alpha * v;
}

template <typename T, size_t N>
array<T, N> operator/(const array<T, N> &v, T alpha) {
  assert(alpha != 0);
  return v * (1. / alpha);
}

template <typename T, size_t N>
void operator*=(array<T, N> &v, T alpha) {
  v = v * alpha;
}

template <typename T, size_t N>
void operator/=(array<T, N> &v, T alpha) {
  v = v / alpha;
}

// other functions

template <typename T>
double norm_squared(const vector<T> &a) {
  return a * a;
}

template <typename T, size_t N>
double norm_squared(const array<T, N> &a) {
  return a * a;
}

template <typename T>
double norm(const vector<T> &a) {
  return sqrt(norm_squared(a));
}

template <typename T, size_t N>
double norm(const array<T, N> &a) {
  return sqrt(norm_squared(a));
}

template <typename T>
vector<T> parallel(const vector<T> &a, const vector<T> &b) {
  // component of a parallel to b
  assert(a.size() == b.size());
  vector<T> versb = b / norm(b);
  return (a * versb) * versb;
}

template <typename T, size_t N>
array<T, N> parallel(const array<T, N> &a, const array<T, N> &b) {
  // component of a parallel to b
  array<T, N> versb = b / norm(b);
  return (a * versb) * versb;
}

template <typename T>
vector<T> orthogonal(const vector<T> &a, const vector<T> &b) {
  return a - parallel(a, b);
}

template <typename T, size_t N>
array<T, N> orthogonal(const array<T, N> &a, const array<T, N> &b) {
  return a - parallel(a, b);
}

template <typename T>
void print(const vector<T> &v, string name = "v") {
  cout << name << " = (";
  for (int i = 0; i < int(v.size()) - 1; i++) {
    cout << v[i] << ", ";
  }
  cout << v[int(v.size()) - 1] << ")" << endl;
}

template <typename T, size_t N>
void print(const array<T, N> &v, string name = "v") {
  cout << name << " = (";
  for (int i = 0; i < int(v.size()) - 1; i++) {
    cout << v[i] << ", ";
  }
  cout << v[int(v.size()) - 1] << ")" << endl;
}

#endif
