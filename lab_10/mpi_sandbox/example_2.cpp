#include <iostream>

#include "mpi.h"

using namespace std;

int main(int argc, char* argv[]) {
  int size, rank;
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (size > 3) {
    cout << "too many processes!" << endl;
    return 1;
  }
  int recieved_indices[3];
  for (int i = 0; i < 3; i++) recieved_indices[i] = 0;
  int sender_index = rank + 1;

  MPI_Gather(&sender_index, 1, MPI_INTEGER, recieved_indices, 1, MPI_INTEGER, 0,
             MPI_COMM_WORLD);

  if (rank == 0)
    cout << "recieved indices: " << recieved_indices[0] << " "
         << recieved_indices[1] << " " << recieved_indices[2] << endl;
  MPI_Finalize();
  return 0;
}