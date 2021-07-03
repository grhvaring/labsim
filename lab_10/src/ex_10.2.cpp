/****************************************************************
 Exercise 10.2: TSP problem solved with a genetic algorithm
 on multiple cores at the same time
*****************************************************************/

#include <iostream>

#include "genetic_tsp.h"
#include "libsim.h"
#include "linalg.h"
#include "mpi.h"

using namespace std;

vector<array<double, 2>> read_positions(string path_to_file) {
  ifstream in{path_to_file};
  vector<array<double, 2>> v;
  double x, y;
  for (; !in.eof();) {
    in >> x >> y;
    v.push_back({x, y});
  }
  return v;
}

int pbc(int index, int imax) {
  int i = index;
  if (i < 0 || i >= imax) i = i - (int)floor((double)i / (double)imax) * imax;
  return i;
}

using namespace std;

int main(int argc, char* argv[]) {
  // output setup
  string result_path = "../results/lab_10/";
  vector<string> files{"mpi_lengths", "mpi_best_average", "mpi_best_routes",
                       "mpi_worst_routes"};

  // parallel algorithm
  int size, rank, dest, from;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  vector<MPI_Status> status;
  for (int i = 0; i < size; ++i) {
    MPI_Status si;
    status.push_back(si);
  }
  double t_start = MPI_Wtime();

  // output
  map<string, ofstream> out;
  vector<string> filenames =
      add_suffix(files, '_' + to_string(size) + "_" + to_string(rank));
  open(out, result_path, filenames);

  // random seed
  int seed[] = {0, 0, 0, 1};
  int primes[] = {2892, 2587, 2892, 2707, 2892, 2801, 2892, 2899};
  Random random{seed, primes[2 * rank], primes[2 * rank + 1]};
  for (int i = 0; i < 4; ++i) {
    seed[i] = 100 * rank * random.Rannyu();  //  initializing different seeds
  }

  // create population
  int nroutes = 1000;
  int nsteps = 32;
  GeneticTSP tsp{nroutes, nsteps, seed};
  tsp.generate_population();
  string path_to_file = "../results/lab_09/square_points.dat";
  vector<array<double, 2>> positions = read_positions(path_to_file);
  tsp.set_positions(positions);

  int generations = 1000;
  int migrate_every = 150;

  vector<int*> message_index;

  for (int s = 0; s < size; ++s) {
    int* message_index_s = new int[nsteps];
    for (int step = 0; step < nsteps; ++step) {
      message_index_s[step] = tsp.route(0)[step];
    }
    message_index.push_back(message_index_s);
  }

  vector<int> index_tag;
  for (int r = 0; r < size; ++r) index_tag.push_back(r + 1);

  for (int generation = 1; generation <= generations; ++generation) {
    tsp.evolve();

    if (size > 1 && generation % migrate_every == 0) {
      for (int r = 0; r < size; ++r) {
        int* message_index_r = new int[nsteps];
        for (int step = 0; step < nsteps; ++step) {
          message_index_r[step] = tsp.route(0)[step];
        }
        message_index[r] = message_index_r;
      }

      dest = pbc(rank + 1, size);  // send best to next continent
      from = pbc(rank - 1, size);  // recieve best from prev continent

      if (generation == migrate_every) {
        cout << endl << "before: " << endl;
        tsp.print(0);
      }
      MPI_Send(message_index[rank], nsteps, MPI_INTEGER, dest, index_tag[rank],
               MPI_COMM_WORLD);
      MPI_Recv(message_index[from], nsteps, MPI_INTEGER, from, index_tag[from],
               MPI_COMM_WORLD, &status[rank]);

      for (int city = 0; city < nsteps; ++city) {
        if (generation == migrate_every)
          cout << message_index[from][city] << " ";
        tsp.route(0)[city] = message_index[from][city];
      }
      if (generation == migrate_every) {
        cout << endl << "after: " << endl;
        tsp.print(0);
        cout << endl;
      }
    }
    out[filenames[0]] << tsp.route_length(0) << endl;
    out[filenames[1]] << tsp.best_average_length() << endl;
    tsp.save_best_route(out[filenames[2]]);
    tsp.save_worst_route(out[filenames[3]]);
  }

  close(out);

  double t_end = MPI_Wtime();
  double dt = t_end - t_start;
  cout << endl
       << "Node " << rank << ": L^2 = " << tsp.route_length(0)
       << ", time = " << dt << endl;
  MPI_Finalize();

  return 0;
}