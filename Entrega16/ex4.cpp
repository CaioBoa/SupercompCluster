#include <mpi.h>
#include <iostream>
#include <vector>
#include <numeric>
#include <cmath>  
#include <cstdlib>  
#include <ctime>    

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv); 

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);  
    MPI_Comm_size(MPI_COMM_WORLD, &size);  

    const int array_size = 16;  
    const int elements_per_proc = array_size / size;  

    std::vector<float> array; 
    if (rank == 0) {
        array.resize(array_size);
        srand(time(0));  
        for (int i = 0; i < array_size; ++i) {
            array[i] = static_cast<float>(rand() % 100);  
        }

        std::cout << "Array inicial no processo raiz: ";
        for (float val : array) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }

    std::vector<float> sub_array(elements_per_proc);

    MPI_Scatter(array.data(), elements_per_proc, MPI_FLOAT, sub_array.data(), elements_per_proc, MPI_FLOAT, 0, MPI_COMM_WORLD);

    float local_sum = std::accumulate(sub_array.begin(), sub_array.end(), 0.0f);
    float local_mean = local_sum / elements_per_proc;

    float local_variance = 0.0f;
    for (float val : sub_array) {
        local_variance += (val - local_mean) * (val - local_mean);
    }

    std::vector<float> local_means(size);
    std::vector<float> local_variances(size);

    MPI_Gather(&local_mean, 1, MPI_FLOAT, local_means.data(), 1, MPI_FLOAT, 0, MPI_COMM_WORLD);
    MPI_Gather(&local_variance, 1, MPI_FLOAT, local_variances.data(), 1, MPI_FLOAT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        float global_mean = std::accumulate(local_means.begin(), local_means.end(), 0.0f) / size;

        float global_variance = 0.0f;
        for (int i = 0; i < size; ++i) {
            global_variance += local_variances[i];  
            for (int j = 0; j < elements_per_proc; ++j) {
                float val = array[i * elements_per_proc + j];
                global_variance += (val - global_mean) * (val - global_mean);
            }
        }

        float global_std_dev = std::sqrt(global_variance / array_size);

        std::cout << "Desvio padrão global: " << global_std_dev << std::endl;
    }

    MPI_Finalize(); 
    return 0;
}

