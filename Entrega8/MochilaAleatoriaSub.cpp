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
        std::vector<Item> itemsMochila;

        std::shuffle(itemsRestantes.begin(), itemsRestantes.end(), gen);
        
        Mochila mochilaAtual = {0, capacidade};

        loop:
        for (auto it = itemsRestantes.begin(); it != itemsRestantes.end(); ){
            if (it->peso <= mochilaAtual.capacidade){
                mochilaAtual.valor += it->valor;
                mochilaAtual.capacidade -= it->peso;

                // Adiciona o item à mochila
                itemsMochila.push_back(*it);
                
                // Remove o item da lista de itens restantes
                it = itemsRestantes.erase(it);  // O `erase` retorna o próximo iterador válido
            } else {
                ++it;
            }
        }

        // Tenta substituir itens na mochila por itens restantes
        for (const auto& itemRestante : itemsRestantes){
            for (auto itMochila = itemsMochila.begin(); itMochila != itemsMochila.end(); ++itMochila){
                if (itemRestante.peso <= mochilaAtual.capacidade + itMochila->peso){
                    if (itemRestante.valor > itMochila->valor){
                        // Substitui o item menos valioso da mochila por um item mais valioso
                        mochilaAtual.valor += itemRestante.valor - itMochila->valor;
                        mochilaAtual.capacidade += itMochila->peso;
                        mochilaAtual.capacidade -= itemRestante.peso;

                        itemsMochila.erase(itMochila);  // Remove o item antigo
                        itemsMochila.push_back(itemRestante);  // Adiciona o item novo
                        goto loop;
                    }
                }
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