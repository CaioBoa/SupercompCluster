#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <thrust/host_vector.h>
#include <thrust/device_vector.h>
#include <thrust/reduce.h>
#include <thrust/extrema.h>
#include <thrust/transform.h>

void read_dual_stock_data(const std::string& file_name, thrust::host_vector<float>& aapl, thrust::host_vector<float>& msft) {
    std::ifstream file(file_name);
    std::string line;

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string apple_str, microsoft_str;

        if (std::getline(ss, apple_str, ',') && std::getline(ss, microsoft_str)) {

            float apple_price = std::stof(apple_str);
            float microsoft_price = std::stof(microsoft_str);

            aapl.push_back(apple_price);
            msft.push_back(microsoft_price);
        }
    }
}

int main() {
    thrust::host_vector<float> h_aapl_prices, h_msft_prices;
    read_dual_stock_data("stocks.txt", h_aapl_prices, h_msft_prices);

    thrust::device_vector<float> d_aapl_prices = h_aapl_prices;
    thrust::device_vector<float> d_msft_prices = h_msft_prices;

    thrust::device_vector<float> d_diff(d_aapl_prices.size());

    thrust::transform(d_aapl_prices.begin(), d_aapl_prices.end(), d_msft_prices.begin(), d_diff.begin(), thrust::minus<float>());

    float total_diff = thrust::reduce(d_diff.begin(), d_diff.end(), 0.0f);
    float average_diff = total_diff / d_diff.size();
    std::cout << "Diferença média entre os preços da Apple e Microsoft: " << average_diff << std::endl;

    return 0;
}
