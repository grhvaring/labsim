/****************************************************************
 Exercise 1.2: Generate 3 histograms containing the results of
 n=10^4 realizations of the quantity S_N = sum_1^N x_i (for N=1,
 2, 10, 100), x_i being a random variable sampled throwing:
  1. a standard (uniform) die
  2. an exponential die, using lambda=1
  3. a Lorentzian die, using mu=0 and Gamma=1
*****************************************************************/

#include "lib01.h"
#include "libsim.h"

using namespace std;

int main(int argc, char** argv) {
  int repetitions = 1e4;
  vector<int> sample_sizes{1, 2, 10, 100};

  // output setup
  string result_path = "../results/lab_01/";
  vector<string> names{"uniform_die", "exponential_die", "lorentz_die"};
  map<string, ofstream> out;
  open(out, result_path, names);

  for (string name : names) {
    // write to the first line of each file the sample sizes used
    for (int size : sample_sizes) out[name] << size << ' ';
    // shift stream to second line
    out[name] << endl;
  }

  // initialization
  int seed[] = {0, 0, 0, 1};
  Three_dice dice(names, seed);
  progress_bar progress{clog, 70u};

  // simulation
  for (int i = 0; i < repetitions; ++i) {
    for (int size : sample_sizes) {
      dice.block_averages(size, out);
    }
    newline(out);
    progress.write(double(i + 1) / repetitions);
  }

  close(out);

  return 0;
}
