#include <mpi.h>
#include <iostream>
#include <vector>
#include <numeric> 
#include <cstdlib> 
#include <ctime>   

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv); 

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); 
    MPI_Comm_size(MPI_COMM_WORLD, &size); 

    const int array_size = 16; 
    const int elements_per_proc = array_size / size; 

    std::vector<int> array; 

    if (rank == 0) {
        array.resize(array_size);
        srand(time(0));  
        for (int i = 0; i < array_size; ++i) {
            array[i] = rand() % 100; 
        }

        std::cout << "Array inicial no processo raiz: ";
        for (int val : array) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }

    std::vector<int> sub_array(elements_per_proc);

    MPI_Scatter(array.data(), elements_per_proc, MPI_INT, sub_array.data(), elements_per_proc, MPI_INT, 0, MPI_COMM_WORLD);

    double local_sum = std::accumulate(sub_array.begin(), sub_array.end(), 0);
    double local_mean = local_sum / elements_per_proc;

    std::cout << "Processo " << rank << " recebeu os elementos: ";
    for (int val : sub_array) {
        std::cout << val << " ";
    }
    std::cout << "e calculou a média local: " << local_mean << std::endl;

    std::vector<double> local_means(size);

    MPI_Gather(&local_mean, 1, MPI_DOUBLE, local_means.data(), 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        double global_mean = std::accumulate(local_means.begin(), local_means.end(), 0.0) / size;
        std::cout << "A média global do array é: " << global_mean << std::endl;
    }

    MPI_Finalize();
    return 0;
}
