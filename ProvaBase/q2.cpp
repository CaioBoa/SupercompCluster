#include <mpi.h>
#include <omp.h>
#include <iostream>
#include <vector>
#include <cstdlib> // Para rand() e srand()
#include <ctime>   // Para time()

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Obtém o rank do processo
    MPI_Comm_size(MPI_COMM_WORLD, &size); // Obtém o número de processos

    const int N = 100000; // Tamanho total do vetor
    const int elements_per_proc = N / size; // Elementos por processo
    std::vector<int> global_array; // Vetor global (somente no processo raiz)
    std::vector<int> local_array(elements_per_proc); // Subvetor de cada processo

    // Inicializar o vetor global no processo raiz
    if (rank == 0) {
        global_array.resize(N);
        srand(time(0)); // Semente para geração de números aleatórios
        for (int i = 0; i < N; ++i) {
            global_array[i] = rand() % 1000; // Valores entre 0 e 99
        }

        std::cout << "Vetor global: ";
        for (int val : global_array) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }

    // Distribuir o vetor global para todos os processos
    MPI_Scatter(global_array.data(), elements_per_proc, MPI_INT,
                local_array.data(), elements_per_proc, MPI_INT, 0, MPI_COMM_WORLD);

    // Busca paralela com OpenMP
    {
        int squared_summ = 0; // Posições encontradas pela thread

        #pragma omp parallel for reduction(+:squared_summ)
        for (int i = 0; i < elements_per_proc; ++i) {
            squared_summ += local_array[i] * local_array[i];
        }

    }

    std::vector<int> counts(size); // Contagem de elementos por processo
    MPI_Gather(&squared_summ, 1, MPI_INT, counts.data(), 1, MPI_INT, 0, MPI_COMM_WORLD);
    int total_summ = 0; // Soma total dos quadrados
    if (rank == 0) {
        for (int i = 0; i < size; ++i) {
            total_summ += counts[i];
        }
        std::cout << total_summ << std::endl;
    }

    MPI_Finalize();
    return 0;
}