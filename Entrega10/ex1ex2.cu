#include <iostream>
#include <vector>
#include <thrust/host_vector.h>
#include <thrust/device_vector.h>
#include <thrust/reduce.h>
#include <thrust/extrema.h>
#include <chrono>
#include <fstream>

thrust::host_vector<float> read_stock_data(const std::string& file_name) {
    std::ifstream file(file_name);
    thrust::host_vector<float> stock_prices;
    float price;

    while (file >> price) {
        stock_prices.push_back(price);
    }

    return stock_prices;
}

int main() {
    auto start = std::chrono::high_resolution_clock::now();

    thrust::host_vector<float> h_stock_prices = read_stock_data("google.txt");

    thrust::device_vector<float> d_stock_prices = h_stock_prices;

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Tempo de alocação e cópia: " << elapsed.count() << " segundos" << std::endl;

    float total_sum = thrust::reduce(d_stock_prices.begin(), d_stock_prices.end(), 0.0f);
    float average_price = total_sum / d_stock_prices.size();
    std::cout << "Preço médio dos últimos 10 anos: " << average_price << std::endl;

    size_t days_in_year = 365;
    thrust::device_vector<float>::iterator start_year = d_stock_prices.end() - days_in_year;
    float year_sum = thrust::reduce(start_year, d_stock_prices.end(), 0.0f);
    float average_last_year = year_sum / days_in_year;
    std::cout << "Preço médio dos últimos 365 dias: " << average_last_year << std::endl;

    auto max_price = thrust::max_element(d_stock_prices.begin(), d_stock_prices.end());
    auto min_price = thrust::min_element(d_stock_prices.begin(), d_stock_prices.end());
    std::cout << "Maior preço nos últimos 10 anos: " << *max_price << std::endl;
    std::cout << "Menor preço nos últimos 10 anos: " << *min_price << std::endl;

    auto max_last_year = thrust::max_element(start_year, d_stock_prices.end());
    auto min_last_year = thrust::min_element(start_year, d_stock_prices.end());
    std::cout << "Maior preço no último ano: " << *max_last_year << std::endl;
    std::cout << "Menor preço no último ano: " << *min_last_year << std::endl;

    return 0;
}
