#include <omp.h>
#include <iostream>
#include <vector>
#include <numeric>
#include <chrono>

const int VECTOR_SIZE = 20;

float meanV(const std::vector<int> vector){
    long sum = 0;
    for(int i = 0; i < VECTOR_SIZE; i++){
        sum += vector[i];
    }
    float mean = sum / VECTOR_SIZE;
    return mean;
}

int maxV(const std::vector<int> vector){
    int max = vector[0];
    for(int i = 1; i < VECTOR_SIZE; i++){
        if (vector[i] > max){
            max = vector[i];
        }
    }
    return max;
}

long multV(const std::vector<int> vector){
    long prod = vector[0];
    for(int i = 1; i < VECTOR_SIZE; i++){
        prod *= vector[i];
    }
    return prod;
}

int main(int argc, char** argv) {
    
    auto inicio = std::chrono::high_resolution_clock::now();
    std::vector<int> vector;

    vector.resize(VECTOR_SIZE);
    for (int i = 0; i < VECTOR_SIZE; ++i) {
        vector[i] = i + 1; 
    }

    float mean;
    int max;
    long prod;

    mean = meanV(vector);
    max = maxV(vector);
    prod = multV(vector);

    auto fim = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> tempoDeExecucao = fim - inicio;

    std::cout << "Mean : " << mean << std::endl;
    std::cout << "Max : " << max << std::endl;
    std::cout << "Prod : " << prod << std::endl;
    std::cout << "Tempo de Execução : " << tempoDeExecucao.count() << std::endl;

}