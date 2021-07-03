/****************************************************************
 Library of useful functions and objects used in the labs
****************************************************************/

#ifndef __libsim__
#define __libsim__

#define default_ext ".dat"  // defaul extension given to new files

#include <array>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "progress.h"

using namespace std;

typedef double (*simple_function)(double);
typedef double (*scalar_function)(const vector<double> &);
typedef vector<double> (*vector_function)(const vector<double> &);

double constant_function(double x);

struct Containers {
  map<string, double> estimates, sums, sums2, averages, sigmas;
  void reset_all(vector<string> keys) {
    // assume all containers use the same keys and set them to zero
    for (string key : keys) {
      estimates[key] = 0;
      sums[key] = 0;
      sums2[key] = 0;
      averages[key] = 0;
      sigmas[key] = 0;
    }
  }
  void reset_block() {
    // set to zero just the block estimates
    for (auto &estimate : estimates) estimate.second = 0;
  }
};

void open(ofstream &, string path, string file, bool message = true);
void open(map<string, ofstream> &, string path, const vector<string> &filenames,
          bool message = true);
void newline(map<string, ofstream> &);
void check_positive(int N);
void log_file_opening(string);
void log_file_opening(const vector<string> &);
void close(ofstream &, bool message = false);
void close(map<string, ofstream> &, bool message = false);
vector<string> add_suffix(const vector<string> &, string);
vector<string> add_prefix(const vector<string> &, string);
bool is_substring(string sub, string s);

double error(double average, double sum2, int N);

#endif  // __libsim__
