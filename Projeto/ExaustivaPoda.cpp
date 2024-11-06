#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <string>
#include <fstream>
#include <iomanip>
#include <numeric> // Para std::iota

// Função para ler o grafo a partir do arquivo de entrada
std::vector<std::vector<int>> LerGrafo(const std::string& nomeArquivo, int& numVertices) {
    std::ifstream arquivo(nomeArquivo);
    if (!arquivo.is_open()) {
        std::cerr << "Erro ao abrir o arquivo " << nomeArquivo << std::endl;
        return {};
    }

    int numArestas;
    arquivo >> numVertices >> numArestas;

    std::vector<std::vector<int>> grafo(numVertices, std::vector<int>(numVertices, 0));

    for (int i = 0; i < numArestas; ++i) {
        int u, v;
        arquivo >> u >> v;
        grafo[u - 1][v - 1] = 1;
        grafo[v - 1][u - 1] = 1;
    }

    arquivo.close();
    return grafo;
}

// Função que verifica se um vértice é conectado a todos os vértices da clique atual
bool ConectadoATodos(const std::vector<std::vector<int>>& grafo, const std::vector<int>& cliqueAtual, int v) {
    for (int u : cliqueAtual) {
        if (grafo[u][v] == 0) {
            return false;
        }
    }
    return true;
}

// Função recursiva para encontrar a clique máxima com poda
void ExplorarClique(const std::vector<std::vector<int>>& grafo, std::vector<int>& cliqueAtual,
                    std::vector<int>& melhorClique, std::vector<int>& candidatos) {
    if (cliqueAtual.size() + candidatos.size() <= melhorClique.size()) {
        return; // Poda: Não pode superar a melhor clique conhecida
    }

    if (cliqueAtual.size() > melhorClique.size()) {
        melhorClique = cliqueAtual;
    }

    for (size_t i = 0; i < candidatos.size(); ++i) {
        int v = candidatos[i];
        if (ConectadoATodos(grafo, cliqueAtual, v)) {
            cliqueAtual.push_back(v);

            std::vector<int> novosCandidatos(candidatos.begin() + i + 1, candidatos.end());
            ExplorarClique(grafo, cliqueAtual, melhorClique, novosCandidatos);

            cliqueAtual.pop_back();
        }
    }
}

// Função principal para encontrar a clique máxima
std::vector<int> EncontrarCliqueMaximaHeuristica(const std::vector<std::vector<int>>& grafo, int numVertices) {
    std::vector<int> melhorClique;
    std::vector<int> cliqueAtual;
    std::vector<int> candidatos(numVertices);
    std::iota(candidatos.begin(), candidatos.end(), 0); // Preenche com 0, 1, ..., numVertices-1

    ExplorarClique(grafo, cliqueAtual, melhorClique, candidatos);
    return melhorClique;
}

int main() {
    int numVertices;
    std::string nomeArquivo = "grafo.txt";

    std::vector<std::vector<int>> grafo = LerGrafo(nomeArquivo, numVertices);

    auto inicioTotal = std::chrono::high_resolution_clock::now();

    std::vector<int> cliqueMaxima = EncontrarCliqueMaximaHeuristica(grafo, numVertices);

    auto duracaoTotal = std::chrono::duration_cast<std::chrono::duration<double>>(
                            std::chrono::high_resolution_clock::now() - inicioTotal).count();

    std::cout << "Clique máxima encontrada -> ";
    for (int v : cliqueMaxima) {
        std::cout << v + 1 << " ";
    }
    std::cout << "\nTamanho da clique máxima -> " << cliqueMaxima.size() << std::endl;

    std::cout << std::fixed << std::setprecision(6);
    std::cout << "Tempo total de execução -> " << duracaoTotal << " segundos" << std::endl;

    return 0;
}

