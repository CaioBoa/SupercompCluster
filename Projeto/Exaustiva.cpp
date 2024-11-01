#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <set>

using namespace std;
using namespace chrono;

// Função para ler o grafo de um arquivo e armazená-lo como uma matriz de adjacência
vector<vector<int>> LerGrafo(const string& nomeArquivo, int& numVertices) {
    ifstream arquivo(nomeArquivo);
    int numArestas;
    arquivo >> numVertices >> numArestas;

    vector<vector<int>> grafo(numVertices, vector<int>(numVertices, 0));

    for (int i = 0; i < numArestas; ++i) {
        int u, v;
        arquivo >> u >> v;
        grafo[u - 1][v - 1] = 1;
        grafo[v - 1][u - 1] = 1;
    }
    arquivo.close();
    
    // Imprimindo a matriz de adjacência para depuração
    cout << "Matriz de Adjacência:" << endl;
    for (int i = 0; i < numVertices; ++i) {
        for (int j = 0; j < numVertices; ++j) {
            cout << grafo[i][j] << " ";
        }
        cout << endl;
    }
    return grafo;
}

// Função para verificar se todos os vértices em um conjunto estão conectados (formam uma clique)
bool EhClique(const vector<vector<int>>& grafo, const vector<int>& vertices) {
    cout << "Verificando o conjunto: ";
    for (int v : vertices) cout << v + 1 << " ";
    cout << endl;

    for (size_t i = 0; i < vertices.size(); ++i) {
        for (size_t j = i + 1; j < vertices.size(); ++j) {
            int v1 = vertices[i];
            int v2 = vertices[j];
            if (grafo[v1][v2] == 0) {
                cout << "Aresta faltando entre " << v1 + 1 << " e " << v2 + 1 << endl;
                return false;
            }
        }
    }
    return true;
}

// Função para encontrar a clique máxima usando uma busca exaustiva com poda
vector<int> EncontrarCliqueMaxima(const vector<vector<int>>& grafo, int numVertices) {
    vector<int> cliqueMaxima;
    vector<int> conjuntoAtual;
    int maiorTamanho = 0;

    for (int i = 1; i < (1 << numVertices); ++i) {
        conjuntoAtual.clear();
        for (int j = 0; j < numVertices; ++j) {
            if (i & (1 << j)) {
                conjuntoAtual.push_back(j);
            }
        }

        if (conjuntoAtual.size() > maiorTamanho && EhClique(grafo, conjuntoAtual)) {
            cliqueMaxima = conjuntoAtual;
            maiorTamanho = conjuntoAtual.size();
        }
    }

    return cliqueMaxima;
}

int main() {
    int numVertices;
    string nomeArquivo = "grafo.txt";

    auto inicio = high_resolution_clock::now();

    vector<vector<int>> grafo = LerGrafo(nomeArquivo, numVertices);

    vector<int> cliqueMaxima = EncontrarCliqueMaxima(grafo, numVertices);

    auto fim = high_resolution_clock::now();
    auto duracao = duration_cast<milliseconds>(fim - inicio);

    if (!cliqueMaxima.empty()) {
        cout << "Clique máxima encontrada com tamanho " << cliqueMaxima.size() << ":\n";
        for (int v : cliqueMaxima) {
            cout << v + 1 << " ";
        }
        cout << endl;
    } else {
        cout << "Nenhuma clique encontrada.\n";
    }

    cout << "Tempo de execução: " << duracao.count() << " ms" << endl;

    return 0;
}



