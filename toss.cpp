#include <iostream>
#include <mpi.h>
#include <random>

using namespace std;

double calculate_pi(int number_of_tosses, int id)
{
    double pi_estimate = 0.0;
    if (id != 0) {
        double number_in_circle = 0;
        for (int toss = 0; toss < number_of_tosses; toss++) {
            const long max_rand = 1000000L;
            double x = -1 + (1 - (-1)) * (random() % max_rand) / max_rand;
            double y = -1 + (1 - (-1)) * (random() % max_rand) / max_rand;
            double distance_squared = x*x + y*y;
            if (distance_squared <= 1) number_in_circle++;
        }
        pi_estimate = 4 * number_in_circle / ((double) number_of_tosses);
    }
    return pi_estimate;
}

int main(int argc, char** argv)
{
    int id, np, number_of_tosses;
    double pi_estimate = 0.0;
    double start_time, end_time;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &np);

    if (id == 0) {
        cout << "Enter number of tosses: ";
        cin >> number_of_tosses;
    }

    MPI_Bcast(&number_of_tosses, 1, MPI_INT, 0, MPI_COMM_WORLD);

    start_time = MPI_Wtime();

    pi_estimate = calculate_pi(number_of_tosses, id);

    double global_pi_estimate;
    MPI_Reduce(&pi_estimate, &global_pi_estimate, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    end_time = MPI_Wtime();

    if (id == 0) {
        global_pi_estimate /= np;
        cout << "The estimated value of pi is: " << global_pi_estimate << endl;

        double duration = end_time - start_time;
        cout << "Elapsed time: " << duration << " seconds" << endl;
    }

    MPI_Finalize();

    return 0;
}