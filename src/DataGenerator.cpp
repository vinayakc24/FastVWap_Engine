#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <random>

/**
 * @brief Generates millions of rows of mock CSV data to benchmark the engine.
 */
int main() {
    const std::string filename = "market_data.csv";
    const int numRows = 5'000'000; // Generate 5 Million ticks

    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file for writing.\n";
        return 1;
    }

    std::cout << "Generating " << numRows << " rows of test data...\n";
    file << "Timestamp,Ticker,Price,Volume\n";

    std::vector<std::string> tickers = {"AAPL", "MSFT", "GOOGL", "AMZN", "TSLA", "META", "NVDA"};
    
    // Set up standard C++ random number generation
    std::mt19937 rng(42); // Fixed seed for reproducible benchmarks
    std::uniform_int_distribution<int> tickerDist(0, tickers.size() - 1);
    std::uniform_real_distribution<double> priceDist(100.0, 500.0);
    std::uniform_int_distribution<int> volDist(1, 10);

    for (int i = 0; i < numRows; ++i) {
        // Example output: 09:30:00.000,AAPL,150.25,100
        file << "09:30:00.000," 
             << tickers[tickerDist(rng)] << ","
             << priceDist(rng) << ","
             << volDist(rng) * 100 << "\n";
    }

    file.close();
    std::cout << "Data generation complete. File saved to: " << filename << "\n";
    return 0;
}