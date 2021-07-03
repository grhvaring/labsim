/*************************
 MPI_Send + Recv test
*************************/

#include <iostream>

#include "mpi.h"

using namespace std;

int main(int argc, char* argv[]) {
  int size, rank;
  int n = 100;
  if (argc > 1) n = atoi(argv[1]);

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  MPI_Status stat1, stat2;
  MPI_Request req;

  int* imesg = new int[n];
  int* imesg2 = new int[n];

  int itag = 1;
  int itag2 = 2;

  for (int i = 0; i < n; i++) {
    imesg[i] = rank;
    imesg2[i] = rank + 1;
  }

  if (rank == 1) {
    MPI_Isend(imesg, n, MPI_INTEGER, 0, itag, MPI_COMM_WORLD, &req);
    MPI_Recv(imesg2, n, MPI_INTEGER, 0, itag2, MPI_COMM_WORLD, &stat2);
    cout << "messaggio = " << imesg2[0] << endl;
  } else if (rank == 0) {
    MPI_Send(imesg2, n, MPI_INTEGER, 1, itag2, MPI_COMM_WORLD);
    MPI_Recv(imesg, n, MPI_INTEGER, 1, itag, MPI_COMM_WORLD, &stat1);
    cout << "messaggio = " << imesg[0] << endl;
  }
  MPI_Finalize();
  return 0;
}