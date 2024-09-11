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

bool compararValor(const Item& a, const Item& b) {
    return a.valor > b.valor;
}

Mochila calculaMochila(const std::vector<Item>& items, int capacidade) {
    int maxValor = 0;
    int melhorCapacidadeRestante = capacidade;

    // Cria uma cópia da lista de itens
    std::vector<Item> itemsRestantes(items.begin(), items.end());
    
    // Ordena os itens pelo valor (maior valor primeiro)
    std::sort(itemsRestantes.begin(), itemsRestantes.end(), compararValor);
    
    Mochila mochilaAtual = {0, capacidade};
    
    // Adiciona os itens à mochila na ordem de maior valor primeiro
    for (const auto& item : itemsRestantes){
        if (item.peso <= mochilaAtual.capacidade){
            mochilaAtual.valor += item.valor;
            mochilaAtual.capacidade -= item.peso;
        }
    }
    
    // Atualiza o valor máximo encontrado e a melhor capacidade restante
    if (mochilaAtual.valor > maxValor){
        maxValor = mochilaAtual.valor;
        melhorCapacidadeRestante = mochilaAtual.capacidade;
    }

    return {maxValor, melhorCapacidadeRestante};
}

int main() {
    auto start = std::chrono::high_resolution_clock::now();

    std::ifstream infile("entradaLeve.txt");
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

    Mochila final = calculaMochila(items, capacidade);

    int capacidadeOcupada = capacidade - final.capacidade;

    std::cout << "Valor máximo: " << final.valor << std::endl;
    std::cout << "Capacidade Utilizada: " << capacidadeOcupada << std::endl;

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    std::cout << "Tempo de execução: " << duration.count() << "s" << std::endl;

    return 0;
}