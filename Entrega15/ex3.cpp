#include <mpi.h>
#include <iostream>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv); 

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); 
    MPI_Comm_size(MPI_COMM_WORLD, &size);  

    int message;  

    if (rank == 0) {
        message = 100;  
        std::cout << "Processo 0 enviou a mensagem: " << message << " para o processo 1" << std::endl;
        MPI_Send(&message, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
    } else {
        MPI_Recv(&message, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        std::cout << "Processo " << rank << " recebeu a mensagem: " << message
                  << " do processo " << rank - 1 << std::endl;

        if (rank < size - 1) {
            MPI_Send(&message, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
        } else {
            std::cout << "Processo " << rank << " é o último e exibiu a mensagem: " << message << std::endl;
        }
    }

    MPI_Finalize();  
    return 0;
}

