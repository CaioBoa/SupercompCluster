#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono> 

struct Item {
    int peso;
    int valor;
};

struct Mochila {
    int valor;
    int capacidade;
};

Mochila calculaMochila(const std::vector<Item>& items, int capacidade, int iterations) {
    int maxValor = 0;
    int melhorCapacidadeRestante = capacidade;
    std::random_device rd;
    std::mt19937 gen(rd());

    for(int i = 0; i < iterations; i++){
        std::vector<Item> itemsRestantes(items.begin(), items.end());
        std::shuffle(itemsRestantes.begin(), itemsRestantes.end(), gen);
        
        Mochila mochilaAtual = {0, capacidade};
        
        for (const auto& item : itemsRestantes){
            if (item.peso <= mochilaAtual.capacidade){
                mochilaAtual.valor += item.valor;
                mochilaAtual.capacidade -= item.peso;
            }
        }
        
        if (mochilaAtual.valor > maxValor){
            maxValor = mochilaAtual.valor;
            melhorCapacidadeRestante = mochilaAtual.capacidade;
        }
    }

    return {maxValor, melhorCapacidadeRestante};
}

int main() {
    auto start = std::chrono::high_resolution_clock::now();

    std::ifstream infile("Entrada3.txt");
    if (!infile.is_open()) {
        std::cerr << "Erro ao abrir o arquivo" << std::endl;
        return 1;
    }

    int capacidade, n;
    infile >> n >> capacidade;

    std::vector<Item> items(n);
    for (int i = 0; i < n; ++i) {
        infile >> items[i].peso >> items[i].valor;
    }

    infile.close();
    
    int iterations = 10;

    Mochila final = calculaMochila(items, capacidade, iterations);

    int capacidadeOcupada = capacidade - final.capacidade;

    std::cout << "Valor máximo: " << final.valor << std::endl;
    std::cout << "Capacidade Utilizada: " << capacidadeOcupada << std::endl;

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    std::cout << "Tempo de execução: " << duration.count() << "s" << std::endl;

    return 0;
}