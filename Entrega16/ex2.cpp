#include <mpi.h>
#include <iostream>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv); 

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);  
    MPI_Comm_size(MPI_COMM_WORLD, &size);  

    int num_iterations = 0;

    if (rank == 0) {
        num_iterations = 100; 
        std::cout << "Processo raiz define o número de iterações: " << num_iterations << std::endl;
    }

    MPI_Bcast(&num_iterations, 1, MPI_INT, 0, MPI_COMM_WORLD);

    std::cout << "Processo " << rank << " recebeu o número de iterações: " << num_iterations << std::endl;

    int local_sum = 0;
    for (int i = 1; i <= num_iterations; ++i) {
        local_sum += i;
    }

    std::cout << "Processo " << rank << " calculou a soma de 1 até " << num_iterations 
              << ": " << local_sum << std::endl;

    MPI_Finalize();  
    return 0;
}
