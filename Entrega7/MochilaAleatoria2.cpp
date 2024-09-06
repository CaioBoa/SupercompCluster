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
    int restoCapacidade = 0;
    std::random_device rd;  
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, 1);

    // Gera um número aleatório entre 0 e 1
    int random_number = distrib(gen);
    for(int i = 0; i < iterations; i++){
        Mochila MochilaNo = {0, capacidade};
        //gera variavel aleatoria entre 0 e 1
        for (int j = 0; j < items.size(); j++){
            std::mt19937 gen(rd()); 
            int random_number = distrib(gen);
            if (random_number >= 0.5){
                if (items[j].peso <= MochilaNo.capacidade){
                    MochilaNo.valor += items[j].valor;
                    MochilaNo.capacidade -= items[j].peso;
                } else {
                    break;
                }
            }
        }
        if (MochilaNo.valor > maxValor){
            maxValor = MochilaNo.valor;
            restoCapacidade = MochilaNo.capacidade;
        }
    }
    return {maxValor, restoCapacidade};
}

int main() {
    auto start = std::chrono::high_resolution_clock::now();

    std::ifstream infile("Entrada4.txt");
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
    int iterations = 5;

    Mochila final = calculaMochila(items, capacidade, iterations);

    int CapacidadeOcupada = capacidade - final.capacidade;

    std::cout << "Valor máximo: " << final.valor << std::endl;
    std::cout << "Capacidade Utilizada: " << CapacidadeOcupada << std::endl;

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    std::cout << "Tempo de execução: " << duration.count() << "s" << std::endl;

    return 0;
}