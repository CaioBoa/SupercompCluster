#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <thrust/host_vector.h>
#include <thrust/device_vector.h>
#include <thrust/transform.h>
#include <thrust/functional.h>
#include <thrust/count.h>
#include <thrust/replace.h>
#include <thrust/reduce.h>

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

// Functor para identificar dias de aumento no preço (diferença positiva)
struct is_positive_gain {
    __host__ __device__
    bool operator()(const float& x) const {
        return x > 0.0f;  // Retorna verdadeiro se o ganho for positivo
    }
};

// Functor para substituir ganhos negativos por zero
struct is_negative_gain {
    __host__ __device__
    bool operator()(const float& x) const {
        return x < 0.0f;  // Retorna verdadeiro se o ganho for negativo
    }
};

int main() {
    thrust::host_vector<float> h_stocks;
    read_stock_data("google.txt", h_stocks);

    size_t total_size = h_stocks.size();
    const size_t block_size = 100000; // Tamanho do bloco a ser processado (ajustável)

    int total_positive_gains = 0;
    float total_sum_positive_gains = 0.0f;

    for (size_t i = 0; i < total_size - 1; i += block_size) {
        size_t current_block_size = std::min(block_size, total_size - i - 1);

        // Bloco atual de dados
        thrust::host_vector<float> h_stocks_block(h_stocks.begin() + i, h_stocks.begin() + i + current_block_size + 1);

        // Transferir o bloco para a GPU
        thrust::device_vector<float> d_stocks = h_stocks_block;

        if (d_stocks.size() > 1) {
            // Calcular as diferenças diárias
            thrust::device_vector<float> d_daily_gain(d_stocks.size() - 1);
            thrust::transform(d_stocks.begin() + 1, d_stocks.end(), d_stocks.begin(), d_daily_gain.begin(), daily_gain_functor());

            // Contar quantos dias tiveram aumento no preço nesse bloco
            int num_positive_gains = thrust::count_if(d_daily_gain.begin(), d_daily_gain.end(), is_positive_gain());
            total_positive_gains += num_positive_gains;

            // Substituir valores negativos por zero
            thrust::replace_if(d_daily_gain.begin(), d_daily_gain.end(), is_negative_gain(), 0.0f);

            // Somar os valores restantes (apenas os positivos)
            float sum_positive_gains = thrust::reduce(d_daily_gain.begin(), d_daily_gain.end(), 0.0f);
            total_sum_positive_gains += sum_positive_gains;
        }
    }

    // Calcular a média dos ganhos positivos
    float average_gain = (total_positive_gains > 0) ? total_sum_positive_gains / total_positive_gains : 0.0f;

    // Exibir os resultados
    std::cout << "Número total de dias com aumento no preço: " << total_positive_gains << std::endl;
    std::cout << "Aumento médio nos dias com ganho: " << average_gain << std::endl;

    return 0;
}
