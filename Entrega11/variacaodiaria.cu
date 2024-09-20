#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <thrust/host_vector.h>
#include <thrust/device_vector.h>
#include <thrust/transform.h>
#include <thrust/iterator/counting_iterator.h>

// Função para ler os preços de ações do arquivo
void read_stock_data(const std::string& file_name, thrust::host_vector<float>& stocks) {
    std::ifstream file(file_name);
    std::string line;

    while (std::getline(file, line)) {
        float price = std::stof(line); // Converte a string para float
        stocks.push_back(price);       // Armazena no vetor
    }
}

// Functor para calcular a diferença entre dias consecutivos
struct daily_gain_functor {
    __host__ __device__
    float operator()(const float& current, const float& previous) const {
        return current - previous;  // Calcula ganho diário
    }
};

int main() {
    thrust::host_vector<float> h_stocks;
    read_stock_data("google.txt", h_stocks);

    size_t total_size = h_stocks.size();
    const size_t block_size = 100000; // Processar 100.000 elementos por vez (ajustável)

    for (size_t i = 0; i < total_size; i += block_size) {
        // Define o tamanho do bloco atual
        size_t current_block_size = std::min(block_size, total_size - i);

        // Vetores no host para o bloco atual
        thrust::host_vector<float> h_stocks_block(h_stocks.begin() + i, h_stocks.begin() + i + current_block_size);

        // Transferir o bloco para a GPU
        thrust::device_vector<float> d_stocks = h_stocks_block;

        if (d_stocks.size() > 1) {
            thrust::device_vector<float> d_daily_gain(d_stocks.size() - 1);
            thrust::transform(d_stocks.begin() + 1, d_stocks.end(), d_stocks.begin(), d_daily_gain.begin(), daily_gain_functor());

            thrust::host_vector<float> h_daily_gain = d_daily_gain;
            for (size_t j = 0; j < h_daily_gain.size(); ++j) {
                std::cout << h_daily_gain[j] << std::endl;
            }
        }
    }

    return 0;
}
