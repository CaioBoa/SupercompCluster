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
#include <thrust/iterator/constant_iterator.h>

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

    // Transfer data to device
    thrust::device_vector<float> d_aapl_prices = h_aapl_prices;
    thrust::device_vector<float> d_msft_prices = h_msft_prices;

    // Calculate differences between AAPL and MSFT prices
    thrust::device_vector<float> d_diff(d_aapl_prices.size());
    thrust::transform(d_aapl_prices.begin(), d_aapl_prices.end(), d_msft_prices.begin(), d_diff.begin(), thrust::minus<float>());

    // Calculate the mean of differences
    float total_diff = thrust::reduce(d_diff.begin(), d_diff.end(), 0.0f);
    float average_diff = total_diff / d_diff.size();
    std::cout << "Diferença média entre os preços da Apple e Microsoft: " << average_diff << std::endl;

    // Calculate the variance
    thrust::device_vector<float> d_variance(d_diff.size());
    thrust::transform(d_diff.begin(), d_diff.end(),
                      thrust::constant_iterator<float>(average_diff),
                      d_variance.begin(),
                      thrust::minus<float>());  // (xi - μ)

    thrust::transform(d_variance.begin(), d_variance.end(), d_variance.begin(),
                      thrust::square<float>());  // (xi - μ)²

    float variance_sum = thrust::reduce(d_variance.begin(), d_variance.end(), 0.0f);
    float variance = variance_sum / d_diff.size();
    std::cout << "Variância das diferenças: " << variance << std::endl;

    return 0;
}
