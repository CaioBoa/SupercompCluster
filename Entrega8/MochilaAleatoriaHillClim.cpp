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
    std::vector<int> solucaoBinaria; // Representação binária da solução
};

// Função para calcular o valor e a capacidade restante de uma solução dada
Mochila calculaValor(const std::vector<Item>& items, const std::vector<int>& solucao, int capacidade) {
    Mochila mochilaAtual = {0, capacidade, solucao};

    for (size_t i = 0; i < items.size(); ++i) {
        if (solucao[i] == 1 && items[i].peso <= mochilaAtual.capacidade) {
            mochilaAtual.valor += items[i].valor;
            mochilaAtual.capacidade -= items[i].peso;
        }
    }

    return mochilaAtual;
}

// Função para gerar vizinhos ao modificar um único bit da solução atual
std::vector<std::vector<int>> geraVizinhos(const std::vector<int>& solucaoAtual) {
    std::vector<std::vector<int>> vizinhos;
    
    for (size_t i = 0; i < solucaoAtual.size(); ++i) {
        std::vector<int> vizinho = solucaoAtual;
        vizinho[i] = 1 - vizinho[i]; // Inverte o bit
        vizinhos.push_back(vizinho);
    }

    return vizinhos;
}

// Função que implementa a estratégia de Hill Climbing
Mochila calculaMochilaHillClimb(const std::vector<Item>& items, int capacidade, int iterations) {
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<> dis(0, 1);
    std::vector<int> solucaoAtual(items.size());
    for (auto& bit : solucaoAtual) {
        bit = dis(gen);
    }

    // Calcula o valor inicial
    Mochila mochilaAtual = calculaValor(items, solucaoAtual, capacidade);

    for (int i = 0; i < iterations; ++i) {
        // Gera todos os vizinhos da solução atual
        std::vector<std::vector<int>> vizinhos = geraVizinhos(solucaoAtual);

        // Avalia cada vizinho e encontra o melhor
        Mochila melhorVizinho = mochilaAtual;
        for (const auto& vizinho : vizinhos) {
            Mochila mochilaVizinho = calculaValor(items, vizinho, capacidade);

            if (mochilaVizinho.valor > melhorVizinho.valor) {
                melhorVizinho = mochilaVizinho;
            }
        }

        if (melhorVizinho.valor > mochilaAtual.valor) {
            mochilaAtual = melhorVizinho;
            solucaoAtual = mochilaAtual.solucaoBinaria;
        } else {
            break;
        }
    }

    return mochilaAtual;
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

    // Chama a função de Hill Climbing para encontrar a solução
    Mochila final = calculaMochilaHillClimb(items, capacidade, iterations);

    int capacidadeOcupada = capacidade - final.capacidade;

    std::cout << "Valor máximo: " << final.valor << std::endl;
    std::cout << "Capacidade Utilizada: " << capacidadeOcupada << std::endl;

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    std::cout << "Tempo de execução: " << duration.count() << "s" << std::endl;

    return 0;
}