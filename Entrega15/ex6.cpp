#include <mpi.h>
#include <iostream>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv); 

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);  
    MPI_Comm_size(MPI_COMM_WORLD, &size);  

    int message = 100;  
    int response;

    if (rank == 0) {
        for (int i = 1; i < size; ++i) {
            std::cout << "Processo 0 enviando mensagem para o processo " << i << std::endl;
            MPI_Send(&message, 1, MPI_INT, i, 0, MPI_COMM_WORLD);

            MPI_Recv(&response, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            std::cout << "Processo 0 recebeu resposta do processo " << i << ": " << response << std::endl;
        }
    } else {
        MPI_Recv(&message, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        std::cout << "Processo " << rank << " recebeu mensagem do processo 0" << std::endl;

        response = rank * 10; 
        MPI_Send(&response, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        std::cout << "Processo " << rank << " enviou resposta para o processo 0: " << response << std::endl;
    }

    MPI_Finalize(); 
    return 0;
}



