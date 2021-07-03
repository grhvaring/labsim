/************
test
************/
#include <iostream>
#include <vector>

#include "mpi.h"

using namespace std;

void print(int a[], int n) {
  cout << "a = [ " << a[0];
  for (int i = 1; i < n; ++i) cout << ", " << a[i];
  cout << " ]" << endl;
}

void print(const vector<int>& a) {
  cout << "a = [ " << a[0];
  for (int i = 1; i < a.size(); ++i) cout << ", " << a[i];
  cout << " ]" << endl;
}

int main(int argc, char* argv[]) {
  int size, rank;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  MPI_Status status;
  double tstart = MPI_Wtime();
  int n = 2;
  int dest, send;
  vector<int> a;
  int sum = 0;
  a.resize(n);

  for (int i = 0; i < n; i++) {
    a[i] = rank;
    cout << "\nsetting a[" << i << "] = " << a[i] << endl;
    if (rank % 2 == 0) {
      dest = 1 - rank;
      cout << "\nsending " << rank << " to " << dest << endl;
      MPI_Send(&a, n, MPI_INTEGER, dest, i, MPI_COMM_WORLD);
    } else {
      cout << "\nrank: " << rank << ", i = " << i << ", ";
      print(a);
      cout << "\nrecieving from " << send << " to change " << rank << endl;
      send = 1 - rank;
      MPI_Recv(&a, n, MPI_INTEGER, send, i, MPI_COMM_WORLD, &status);
      cout << "\nnow rank: " << rank << ", i = " << i << ", ";
      print(a);
    }
    sum += a[i];
  }

  double tend = MPI_Wtime();
  double dt = tend - tstart;
  cout << endl
       << "io sono " << rank << "; somma = " << sum << "; tempo = " << dt
       << endl
       << endl;
  MPI_Finalize();
  return 0;
}
