#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

// Função para ler o grafo de um arquivo e armazená-lo como uma matriz de adjacência
vector<vector<int>> LerGrafo(const string& nomeArquivo, int& numVertices) {
    ifstream arquivo(nomeArquivo);
    int numArestas;
    arquivo >> numVertices >> numArestas;

    // Inicializa a matriz de adjacência
    vector<vector<int>> grafo(numVertices, vector<int>(numVertices, 0));

    for (int i = 0; i < numArestas; ++i) {
        int u, v;
        arquivo >> u >> v;
        grafo[u - 1][v - 1] = 1;
        grafo[v - 1][u - 1] = 1;  // Grafo não direcionado
    }

    arquivo.close();
    return grafo;
}

// Função para verificar se um conjunto de vértices forma uma clique
bool EhClique(const vector<vector<int>>& grafo, const vector<int>& vertices) {
    for (size_t i = 0; i < vertices.size(); ++i) {
        for (size_t j = i + 1; j < vertices.size(); ++j) {
            if (grafo[vertices[i]][vertices[j]] == 0) {
                return false;
            }
        }
    }
    return true;
}

// Função para encontrar a clique máxima usando uma abordagem exaustiva
vector<int> EncontrarCliqueMaxima(const vector<vector<int>>& grafo, int numVertices) {
    vector<int> cliqueMaxima;
    vector<int> conjuntoAtual;

    // Para cada subconjunto de vértices
    for (int i = 1; i < (1 << numVertices); ++i) {
        conjuntoAtual.clear();
        for (int j = 0; j < numVertices; ++j) {
            if (i & (1 << j)) {
                conjuntoAtual.push_back(j);
            }
        }

        // Verifica se o conjunto atual forma uma clique
        if (EhClique(grafo, conjuntoAtual) && conjuntoAtual.size() > cliqueMaxima.size()) {
            cliqueMaxima = conjuntoAtual;
        }
    }

    return cliqueMaxima;
}

int main() {
    int numVertices;
    string nomeArquivo = "grafo.txt";

    // Lê o grafo do arquivo
    vector<vector<int>> grafo = LerGrafo(nomeArquivo, numVertices);

    // Encontra a clique máxima
    vector<int> cliqueMaxima = EncontrarCliqueMaxima(grafo, numVertices);

    // Exibe o resultado
    cout << "Clique máxima encontrada:\n";
    for (int v : cliqueMaxima) {
        cout << v + 1 << " ";  // Ajuste para exibir índices a partir de 1
    }
    cout << endl;

    return 0;
}
