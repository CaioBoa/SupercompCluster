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

    const int N = 1000; // Tamanho total do vetor
    const int elements_per_proc = N / size; // Elementos por processo
    const int search_value = 42; // Valor a ser buscado
    std::vector<int> global_array; // Vetor global (somente no processo raiz)
    std::vector<int> local_array(elements_per_proc); // Subvetor de cada processo

    // Inicializar o vetor global no processo raiz
    if (rank == 0) {
        global_array.resize(N);
        srand(time(0)); // Semente para geração de números aleatórios
        for (int i = 0; i < N; ++i) {
            global_array[i] = rand() % 100; // Valores entre 0 e 99
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

    // Vetor para armazenar as posições locais onde o valor foi encontrado
    std::vector<int> local_positions;

    // Busca paralela com OpenMP
    #pragma omp parallel
    {
        std::vector<int> private_positions; // Posições encontradas pela thread

        #pragma omp for
        for (int i = 0; i < elements_per_proc; ++i) {
            if (local_array[i] == search_value) {
                private_positions.push_back(i); // Índice local da thread
            }
        }

        // Combinar resultados das threads
        #pragma omp critical
        local_positions.insert(local_positions.end(), private_positions.begin(), private_positions.end());
    }

    // Ajustar as posições locais para o índice global
    for (int& pos : local_positions) {
        pos += rank * elements_per_proc;
    }

    // Reunir todas as posições globais no processo raiz
    int max_positions = N; // Máximo de posições possíveis
    std::vector<int> global_positions(max_positions); // Buffer para receber todas as posições
    std::vector<int> counts(size); // Número de elementos encontrados por processo
    std::vector<int> displs(size); // Deslocamentos para MPI_Gatherv

    // Obter o número de elementos locais encontrados e deslocamentos
    int local_count = local_positions.size();
    MPI_Gather(&local_count, 1, MPI_INT, counts.data(), 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        displs[0] = 0;
        for (int i = 1; i < size; ++i) {
            displs[i] = displs[i - 1] + counts[i - 1];
        }
    }

    // Reunir as posições globais
    MPI_Gatherv(local_positions.data(), local_count, MPI_INT,
                global_positions.data(), counts.data(), displs.data(), MPI_INT, 0, MPI_COMM_WORLD);

    // Mostrar os resultados no processo raiz
    if (rank == 0) {
        std::cout << "Posições encontradas para o valor " << search_value << ": ";
        for (int i = 0; i < displs[size - 1] + counts[size - 1]; ++i) {
            std::cout << global_positions[i] << " ";
        }
        std::cout << std::endl;
    }

    MPI_Finalize();
    return 0;
}
