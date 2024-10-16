#include <iostream>
#include <random>
#include <ctime>
#include <cmath>
#include <chrono>
#include <omp.h>

int main() {
    const int N = 100000000;  // Número de pontos
    int pontosDentroDoCirculo = 0;

    // Gerador de números aleatórios global
    std::random_device rd;
    std::mt19937 gen(rd());  // Mersenne Twister como gerador
    std::uniform_real_distribution<> dis(0.0, 1.0); // Distribuição de 0 a 1

    // Início da medição de tempo usando std::chrono
    auto inicio = std::chrono::high_resolution_clock::now();

    // Paralelização com OpenMP
    #pragma omp parallel
    {
        int pontosDentro = 0;

        #pragma omp for reduction(+:pontosDentroDoCirculo)
        for (int i = 0; i < N; ++i) {
            double x, y;

            // Usar omp_critical para garantir que apenas uma thread gere números aleatórios por vez
            #pragma omp critical
            {
                x = dis(gen);
                y = dis(gen);
            }

            // Verificar se o ponto (x, y) está dentro do círculo
            if (x * x + y * y <= 1.0) {
                pontosDentro++;
            }
        }

        // Atualizar a soma total de pontos dentro do círculo
        pontosDentroDoCirculo += pontosDentro;
    }

    // Estimar o valor de pi
    double piEstimado = 4.0 * pontosDentroDoCirculo / N;

    // Fim da medição de tempo
    auto fim = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> tempoDeExecucao = fim - inicio;

    // Exibir resultados
    std::cout << "Valor estimado de pi (paralelo): " << piEstimado << std::endl;
    std::cout << "Tempo de execução (paralelo): " << tempoDeExecucao.count() << " segundos" << std::endl;

    return 0;
}
