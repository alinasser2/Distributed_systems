#include <mpi.h>
#include <iostream>
#include <cmath>

using namespace std;

int main(int argc, char** argv)
{
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int parent = (rank - 1) / 2;
    int left_child = 2 * rank + 1;
    int right_child = 2 * rank + 2;

    double start_time = MPI_Wtime();


    if (left_child < size) {
        MPI_Send(&rank, 1, MPI_INT, left_child, 0, MPI_COMM_WORLD);
    }
    if (right_child < size) {
        MPI_Send(&rank, 1, MPI_INT, right_child, 0, MPI_COMM_WORLD);
    }

    int received_from_parent = -1;
    if (rank != 0) {
        MPI_Recv(&received_from_parent, 1, MPI_INT, parent, 0, MPI_COMM_WORLD);
    }

    double end_time = MPI_Wtime();

    cout << "Process " << rank << " received message from parent " << received_from_parent << endl;

    MPI_Finalize();

    if (rank == 0) {
        double duration = end_time - start_time;
        cout << "Time taken: " << duration << " seconds" << endl;
    }

    return 0;
}
