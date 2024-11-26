#include <mpi.h>
#include <omp.h>
#include <iostream>
#include <vector>
#include <numeric> // Para std::accumulate
#include <cstdlib> // Para rand e srand
#include <ctime>   // Para time

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Obtém o rank do processo
    MPI_Comm_size(MPI_COMM_WORLD, &size); // Obtém o número total de processos

    const int N = 1000000; // Tamanho do vetor total
    const int elements_per_proc = N / size; // Elementos por processo
    std::vector<double> local_array(elements_per_proc);

    // Inicialização do vetor (apenas no processo raiz ou localmente)
    if (rank == 0) {
        std::vector<double> global_array(N);
        srand(time(0)); // Inicializa o gerador de números aleatórios
        for (int i = 0; i < N; ++i) {
            global_array[i] = rand() % 100; // Valores aleatórios de 0 a 99
        }

        // Distribui o vetor para os outros processos
        MPI_Scatter(global_array.data(), elements_per_proc, MPI_DOUBLE,
                    local_array.data(), elements_per_proc, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    } else {
        // Recebe a parte do vetor do processo raiz
        MPI_Scatter(nullptr, elements_per_proc, MPI_DOUBLE,
                    local_array.data(), elements_per_proc, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    }

    // Soma parcial usando OpenMP
    double local_sum = 0.0;
    #pragma omp parallel for reduction(+:local_sum)
    for (int i = 0; i < elements_per_proc; ++i) {
        local_sum += local_array[i];
    }

    // Rank 0 recebe as somas parciais e calcula a soma total
    double global_sum = 0.0;
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    // Calcula e exibe a média global no processo raiz
    if (rank == 0) {
        double global_mean = global_sum / N;
        std::cout << "A média global do vetor é: " << global_mean << std::endl;
    }

    MPI_Finalize();
    return 0;
}
