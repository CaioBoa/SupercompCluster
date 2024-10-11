#include <vector>
#include <iostream>
#include <omp.h>
#include <unistd.h>

// Função complexa que simula uma operação que consome recursos
double conta_complexa(int i) {
    return 2 * i;
}

int main() {
    int N = 10000; // Tamanho do vetor
    std::vector<double> vec; // Vetor de resultado

    // Alocação de memória previamente usando resize
    vec.resize(N); 

    // Variáveis para medir o tempo de execução
    double start_time, end_time;

    // Medindo o tempo de execução do loop paralelo
    start_time = omp_get_wtime(); // Início da medição do tempo

    // Paralelizando o loop de modificação do vetor com acesso direto
    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
        vec[i] = conta_complexa(i); // Cada thread escreve diretamente na posição correspondente
    }

    end_time = omp_get_wtime(); // Fim da medição do tempo

    // Exibindo os resultados do vetor para garantir que a paralelização foi bem-sucedida
    for (int i = 0; i < N; i++) {
        std::cout << vec[i] << " ";
    }

    std::cout << std::endl;

    // Exibindo o tempo de execução
    std::cout << "Tempo de execução paralelo com resize: " << (end_time - start_time) << " segundos" << std::endl;

    return 0;
}