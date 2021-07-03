#include <iostream>

#include "mpi.h"

using namespace std;

int main(int argc, char* argv[]) {
  int size, rank;
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  cout << " I am node " << rank << " of the " << size << " currently used."
       << endl;

  MPI_Finalize();
  return 0;
}