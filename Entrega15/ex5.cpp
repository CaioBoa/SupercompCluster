#include <mpi.h>
#include <iostream>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);  

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);  
    MPI_Comm_size(MPI_COMM_WORLD, &size);  

    int value = rank + 1;  
    int received_sum = 0; 

    if (rank == 0) {
        std::cout << "Processo 0 inicia com valor: " << value << std::endl;
        MPI_Send(&value, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
    } else {
        MPI_Recv(&received_sum, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        received_sum += value;
        std::cout << "Processo " << rank << " recebeu soma " << received_sum - value
                  << " e somou seu valor " << value << " = " << received_sum << std::endl;

        if (rank < size - 1) {
            MPI_Send(&received_sum, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
        } else {
            std::cout << "Processo " << rank << " é o último e a soma total é: " << received_sum << std::endl;
        }
    }

    MPI_Finalize(); 
    return 0;
}



