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

    std::cout << "Processos MPI : " << size << std::endl;

    const int N = 1000; // Tamanho total do vetor
    const int elements_per_proc = N / size;

    std::vector<std::vector<int>> A(N, std::vector<int>(N));
    std::vector<std::vector<int>> B(elements_per_proc, std::vector<int>(N));
    std::vector<int> displs(size);

    // Inicializar o vetor global no processo raiz
    if (rank == 0) {
        srand(time(NULL));
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                A[i][j] = rand() % 100;
            }
        }
    }

    // Distribuir o vetor global para todos os processos
    MPI_Scatter(A.data(), N, MPI_INT,
                B.data(), elements_per_proc, MPI_INT, 0, MPI_COMM_WORLD);

    std::vector<int> LocalMeans(elements_per_proc);

    for (int i = 0; i < elements_per_proc; ++i) {
        LocalMeans[i] = 0;
        #pragma omp parallel for
        for (int j = 0; j < N; ++j){
            LocalMeans[i] += B[i][j]; 
        }
        LocalMeans[i] = LocalMeans[i] / N;
    }
    
    float LocalMean = 0.0;

    #pragma omp parallel for reduction(+:LocalMean)
    for (int i = 0; i < elements_per_proc; ++i){
        LocalMean += LocalMeans[i]/elements_per_proc;
    }

    std::vector<float> GlobalMeans(size);

    MPI_Gather(&LocalMean, 1, MPI_FLOAT, GlobalMeans.data(), 1, MPI_FLOAT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        float GlobalMean = 0;
        for (int i = 1; i < size; ++i) {
            GlobalMean += GlobalMeans[i] / size;
        }
        std::cout << "Media Global : " << GlobalMean << std::endl;
    }

    MPI_Finalize();
    return 0;
}