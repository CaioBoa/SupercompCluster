#include <mpi.h>
#include <iostream>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv); 

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); 
    MPI_Comm_size(MPI_COMM_WORLD, &size);  

    if (size <= 2) {
        if (rank == 0) {
            std::cerr << "Erro: O número de processos deve ser maior que 2!" << std::endl;
        }
        MPI_Finalize();
        return 1;
    }

    int send_value = rank;
    int recv_value;       

    int next = (rank == size) ? 0 : rank + 1;
    int prev = (rank == 0) ? size - 1 : rank - 1;

    if (rank != 0) {
        MPI_Recv(&recv_value, 1, MPI_INT, prev, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        std::cout << "Processo " << rank << " recebeu mensagem do processo " << prev
                  << ": " << recv_value << std::endl;
    }

    MPI_Send(&send_value, 1, MPI_INT, next, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        MPI_Recv(&recv_value, 1, MPI_INT, prev, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        std::cout << "Processo 0 recebeu mensagem do processo " << prev
                  << ": " << recv_value << std::endl;
    }

    MPI_Finalize();  
    return 0;
}
