#include <mpi.h>
#include <iostream>
#include <cstring>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);  

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);  
    MPI_Comm_size(MPI_COMM_WORLD, &size); 

    if (rank == 0) {
        for (int i = 1; i < size; ++i) {
            std::string message = "Mensagem para o processo " + std::to_string(i);
            MPI_Send(message.c_str(), message.size() + 1, MPI_CHAR, i, i, MPI_COMM_WORLD);
            std::cout << "Processo 0 enviou: " << message << std::endl;
        }
    } else {
        char recv_message[100];  
        MPI_Recv(recv_message, 100, MPI_CHAR, 0, rank, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        std::cout << "Processo " << rank << " recebeu: " << recv_message << std::endl;
    }

    MPI_Finalize();  
    return 0;
}


