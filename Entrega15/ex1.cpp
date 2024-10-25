#include <mpi.h>
#include <iostream>
#include <cstring>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);  

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); 

    if (rank == 0) {
        const char* message = "Olá";
        MPI_Send(message, strlen(message) + 1, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
        std::cout << "Processo 0: Enviou mensagem '" << message << "' para o processo 1" << std::endl;

        char recv_message[10];
        MPI_Recv(recv_message, 10, MPI_CHAR, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        std::cout << "Processo 0: Recebeu mensagem '" << recv_message << "' do processo 1" << std::endl;
    } else if (rank == 1) {
        char recv_message[10];
        MPI_Recv(recv_message, 10, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        std::cout << "Processo 1: Recebeu mensagem '" << recv_message << "' do processo 0" << std::endl;

        const char* message = "Oi";
        MPI_Send(message, strlen(message) + 1, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
        std::cout << "Processo 1: Enviou mensagem '" << message << "' para o processo 0" << std::endl;
    }

    MPI_Finalize(); 
    return 0;
}
