#include <mpi.h>
#include <omp.h>
#include <iostream>
#include <vector>
#include <numeric>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Obtém o rank do processo
    MPI_Comm_size(MPI_COMM_WORLD, &size); // Obtém o número total de processos

    const int VECTOR_SIZE = 100000; // Tamanho total do vetor
    const int elements_per_proc = VECTOR_SIZE / size; // Elementos por processo

    std::vector<int> vector; // Vetor global (apenas no processo raiz)
    std::vector<int> local_vector(elements_per_proc); // Subvetor local para cada processo

    if (rank == 0) {
        // Inicializa o vetor global no processo raiz
        vector.resize(VECTOR_SIZE);
        for (int i = 0; i < VECTOR_SIZE; ++i) {
            vector[i] = i + 1; // Preenche com valores de 1 a 100000
        }
    }

    // Distribui o vetor global para todos os processos
    MPI_Scatter(vector.data(), elements_per_proc, MPI_INT,
                local_vector.data(), elements_per_proc, MPI_INT, 0, MPI_COMM_WORLD);

    // Calcula a soma dos quadrados do subvetor local com OpenMP
    long long local_sum = 0;
    #pragma omp parallel for reduction(+:local_sum)
    for (int i = 0; i < elements_per_proc; ++i) {
        local_sum += static_cast<long long>(local_vector[i]) * local_vector[i];
    }

    // Reúne os resultados parciais no processo raiz
    long long global_sum = 0;
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD);

    // O processo raiz exibe o resultado total
    if (rank == 0) {
        std::cout << "Soma total dos quadrados: " << global_sum << std::endl;
    }

    MPI_Finalize();
    return 0;
}
