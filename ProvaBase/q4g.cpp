// *************************************************************************
//         IMPORTS DA THRUST
// *************************************************************************

#include <thrust/host_vector.h>
#include <thrust/device_vector.h>
#include <thrust/transform.h>
#include <thrust/reduce.h>
#include <thrust/functional.h>
#include <thrust/iterator/constant_iterator.h>
#include <thrust/iterator/counting_iterator.h>
#include <thrust/random.h>
#include <iostream>
#include <cmath>

// Função para gerar números aleatórios
struct prg
{
    float a, b;

    __host__ __device__
    prg(float _a=0.f, float _b=1.f) : a(_a), b(_b) {};

    __host__ __device__
    float operator()(const unsigned int n) const
    {
        thrust::default_random_engine rng;
        thrust::uniform_real_distribution<float> dist(a, b);
        rng.discard(n);

        return dist(rng);
    }
};

int main() {
    const int N = 1000000; // Tamanho do vetor
    thrust::counting_iterator<unsigned int> index_sequence_begin(0);
    thrust::host_vector<float> h_vec(N);

    // Preencher o vetor com números aleatórios entre 1.0 e 2.0
    thrust::transform(index_sequence_begin,
                      index_sequence_begin + N,
                      h_vec.begin(),
                      prg(1.f, 2.f));

    // Imprime alguns valores do vetor criado para verificação
    for (int i = 0; i < 20; i++) {
        std::cout << "Elemento original " << i << ": " << h_vec[i] << std::endl;
    }

    // Copiar o vetor do host para o dispositivo
    thrust::device_vector<float> d_vec = h_vec;

    // Calcular o quadrado de cada elemento
    thrust::device_vector<float> squared_d_vec(N);
    thrust::transform(d_vec.begin(), d_vec.end(), squared_d_vec.begin(),
                      thrust::multiplies<float>());

    // Calcular a norma L2 (raiz quadrada da soma dos quadrados)
    float norm = std::sqrt(thrust::reduce(
        squared_d_vec.begin(), squared_d_vec.end(), 0.0f, thrust::plus<float>()));

    // Imprime a norma calculada
    std::cout << "Norma calculada: " << norm << std::endl;

    // Normalize o vetor dividindo cada elemento pela norma L2
    thrust::transform(d_vec.begin(), d_vec.end(),
                      thrust::make_constant_iterator(norm),
                      d_vec.begin(),
                      thrust::divides<float>());

    // Imprime alguns valores do vetor normalizado para verificação
    for (int i = 0; i < 20; i++) {
        std::cout << "Elemento normalizado " << i << ": " << d_vec[i] << std::endl;
    }

    return 0;
}