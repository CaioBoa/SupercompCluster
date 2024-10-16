#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>

int main() {
    const int N = 100000000;  
    int pontosDentroDoCirculo = 0;
    double x, y;

    std::srand(static_cast<unsigned int>(std::time(0)));

    clock_t inicio = clock();

    for (int i = 0; i < N; ++i) {
        x = static_cast<double>(std::rand()) / RAND_MAX;
        y = static_cast<double>(std::rand()) / RAND_MAX;

        if (x * x + y * y <= 1.0) {
            pontosDentroDoCirculo++;
        }
    }

    double piEstimado = 4.0 * pontosDentroDoCirculo / N;

    clock_t fim = clock();
    double tempoDeExecucao = static_cast<double>(fim - inicio) / CLOCKS_PER_SEC;

    std::cout << "Valor estimado de pi: " << piEstimado << std::endl;
    std::cout << "Tempo de execução: " << tempoDeExecucao << " segundos" << std::endl;

    return 0;
}
