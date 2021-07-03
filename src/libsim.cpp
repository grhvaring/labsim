/****************************************************************
 Implementation of the functions declared in libSim.h
****************************************************************/

#include "libsim.h"

#include <cmath>
#include <iostream>

double constant_function(double x) { return 1.; }

void open(ofstream& out, string path, string filename, bool message) {
  out.open(path + filename + default_ext);
  if (!out.is_open()) {
    cerr << "ERROR: The file " + filename + default_ext +
                " could not be opened."
         << endl;
    exit(-1);
  }
  if (message) log_file_opening(filename);
}

void open(map<string, ofstream>& out, string path,
          const vector<string>& filenames, bool message) {
  for (string f : filenames) open(out[f], path, f, false);
  if (message) log_file_opening(filenames);
}

void newline(map<string, ofstream>& out) {
  for (auto& output : out) output.second << endl;
}

void check_positive(int N) {
  if (N <= 0) {
    cerr << N << " <= 0. Please use a positive integer." << endl;
    exit(-1);
  }
}

void log_file_opening(string filename) {
  cout << endl
       << "Writing results to file '" << filename + default_ext << "'" << endl;
}

void log_file_opening(const vector<string>& filenames) {
  string plural = "";
  if (filenames.size() > 1) plural = "s";
  cout << endl << "Writing results to file" + plural + " ";
  for (string f : add_suffix(filenames, default_ext)) cout << "'" << f << "' ";
  cout << endl;
}

void close(ofstream& out, bool message) {
  out.close();
  if (message) cout << "Completed." << endl;
}
void close(map<string, ofstream>& out, bool message) {
  for (auto& mout : out) mout.second.close();
  if (message) cout << "Completed." << endl;
}

vector<string> add_suffix(const vector<string>& words, string suffix) {
  vector<string> new_words;
  for (string w : words) new_words.push_back(w + suffix);
  return new_words;
}

vector<string> add_prefix(const vector<string>& words, string prefix) {
  vector<string> new_words;
  for (string w : words) new_words.push_back(prefix + w);
  return new_words;
}

bool is_substring(string sub, string s) {
  return (s.find(sub) != string::npos);
}

double error(double average, double sum2, int N) {
  // assume N starts at zero
  return (N > 0) ? sqrt((sum2 / (N + 1) - pow(average, 2)) / N) : 0;
}