#include <mpi.h>
#include <iostream>
#include <vector>
#include <cstdlib>  
#include <ctime>    
#include <algorithm> 

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);  

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);  
    MPI_Comm_size(MPI_COMM_WORLD, &size);  

    const int array_size = 16; 
    const int elements_per_proc = array_size / size;  
    std::vector<float> array;  
    float max_value = 0.0f;  

    if (rank == 0) {
        array.resize(array_size);
        srand(time(0));  
        for (int i = 0; i < array_size; ++i) {
            array[i] = static_cast<float>(rand() % 100);  
        }

        std::cout << "Array original no processo raiz: ";
        for (float val : array) {
            std::cout << val << " ";
        }
        std::cout << std::endl;

        max_value = *std::max_element(array.begin(), array.end());
        std::cout << "Valor máximo encontrado no processo raiz: " << max_value << std::endl;
    }

    MPI_Bcast(&max_value, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);

    std::vector<float> sub_array(elements_per_proc);

    MPI_Scatter(array.data(), elements_per_proc, MPI_FLOAT, sub_array.data(), elements_per_proc, MPI_FLOAT, 0, MPI_COMM_WORLD);

    for (int i = 0; i < elements_per_proc; ++i) {
        sub_array[i] /= max_value; 
    }

    std::vector<float> normalized_array;
    if (rank == 0) {
        normalized_array.resize(array_size);
    }

    MPI_Gather(sub_array.data(), elements_per_proc, MPI_FLOAT, normalized_array.data(), elements_per_proc, MPI_FLOAT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        std::cout << "Array normalizado no processo raiz: ";
        for (float val : normalized_array) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }

    MPI_Finalize(); 
    return 0;
}
