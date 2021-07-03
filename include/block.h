/****************************************************************
 definition of the class Block, that performs blocking averages
*****************************************************************/

#ifndef __block__
#define __block__

#include <fstream>
#include <map>
#include <string>

#include "experiment.h"
#include "progress.h"

using namespace std;

// standard deviation
double error(double average, double sum2, int N);

class Block {
  Containers _containers;
  int _counter, _length;
  vector<string> _observables;
  bool _show_progress;
  progress_bar *_progress;

 public:
  Block(vector<string> observables, bool progress = true);
  ~Block() {
    if (_show_progress) delete _progress;
  }
  void reset_all();
  void averages(int iterations, int nblocks, Experiment &,
                map<string, ofstream> &out);
  void reset_block();
  void update_block(Experiment &);
  void update_estimates(const Experiment &);
  void output_block(const Experiment &, map<string, ofstream> &out) const;
  void final_output(const Experiment &, map<string, ofstream> &out) const;

  // read-write
  Containers &containers() { return _containers; }
  int block_counter() { return _counter; }
  int length() { return _length; }
  int step() { return (_counter + 1) * _length; }

  // single observable overloads
  Block(string observable, bool progress = true);
  void averages(int iterations, int nblocks, Experiment &, ofstream &out);
  void output_block(const Experiment &, ofstream &out) const;
  void final_output(const Experiment &, ofstream &out) const;
};

#endif  // __block__