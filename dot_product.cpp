#include <mpi.h>
#include <iostream>
#include <cstring>
#include <cmath>

using namespace std;

int main(int argc, char** argv)
{
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int n = 10;
    double* x = new double[n];
    double* y = new double[n];

    // Initialize the arrays with some data
    if (rank == 0) {
        for (int i = 0; i < n; i++) {
            x[i] =  1;
            y[i] = 2 ;
        }
    }

    // Broadcast the arrays to all processes
    double start_bcast = MPI_Wtime();
    MPI_Bcast(x, n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(y, n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    double end_bcast = MPI_Wtime();

    // Compute the local dot product
    double start_compute = MPI_Wtime();
    double local_result = 0.0;
    for (int i = rank; i < n; i += size) {
        local_result += x[i] * y[i];
    }

    // Compute the global dot product using MPI_Reduce
    double global_result;
    MPI_Reduce(&local_result, &global_result, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    double end_compute = MPI_Wtime();

    if (rank == 0) {
        cout << "The dot product is: " << global_result << endl;
    }

    MPI_Finalize();

    free(x);
    free(y);

    if (rank == 0) {
        double duration_bcast = end_bcast - start_bcast;
        double duration_compute = end_compute - start_compute;
        cout << "Broadcast time: " << duration_bcast << " seconds" << endl;
        cout << "Compute time: " << duration_compute << " seconds" << endl;
    }

    return 0;
}