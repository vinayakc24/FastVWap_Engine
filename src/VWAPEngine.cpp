#include "VWAPEngine.h"
#include <iostream>
#include <fstream>
#include <charconv> // Essential for std::from_chars
#include <iomanip>

bool VWAPEngine::processCSV(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filepath << "\n";
        return false;
    }

    // We allocate this string buffer exactly once. 
    // std::getline will overwrite the contents of this buffer on every loop
    // iteration rather than allocating new memory on the heap.
    std::string line;
    
    // Skip the CSV header row
    std::getline(file, line);

    while (std::getline(file, line)) {
        if (line.empty()) continue;
        processLine(line); // Pass the buffer as a lightweight view
    }

    return true;
}

void VWAPEngine::processLine(std::string_view line) {
    // Expected Data Format: Timestamp,Ticker,Price,Volume
    // Example: "09:30:01.123,AAPL,150.25,100"

    // Step 1: Manual string splitting. 
    // Using std::stringstream or regex is too slow because it allocates memory.
    // We find the indices of the commas directly.
    size_t pos1 = line.find(',');
    size_t pos2 = line.find(',', pos1 + 1);
    size_t pos3 = line.find(',', pos2 + 1);

    // Safety check for malformed rows
    if (pos1 == std::string_view::npos || pos2 == std::string_view::npos || pos3 == std::string_view::npos) {
        return; 
    }

    // Step 2: Zero-copy substring extraction.
    // string_view merely points to the character array and specifies a length.
    std::string_view tickerView = line.substr(pos1 + 1, pos2 - pos1 - 1);
    std::string_view priceView  = line.substr(pos2 + 1, pos3 - pos2 - 1);
    std::string_view volumeView = line.substr(pos3 + 1);

    // Step 3: Fast Numeric Parsing
    // std::stod creates a std::string under the hood. std::from_chars operates 
    // directly on the raw memory buffer, bypassing locale checks and heap allocation.
    double price = 0.0;
    std::from_chars(priceView.data(), priceView.data() + priceView.size(), price);

    uint64_t volume = 0;
    std::from_chars(volumeView.data(), volumeView.data() + volumeView.size(), volume);

    // Step 4: State Aggregation
    // We instantiate a std::string from the view to use as the hash map key.
    std::string ticker(tickerView);
    
    // Fetch the metric struct (or default initialize it if it doesn't exist yet)
    auto& metric = metrics[ticker];
    
    // VWAP Math: Add the dollar value of this specific trade, and add the volume
    metric.cumulativeValue += (price * volume);
    metric.cumulativeVolume += volume;
}

void VWAPEngine::printResults() const {
    std::cout << "\n=== FINAL VWAP CALCULATIONS ===\n";
    std::cout << std::left << std::setw(10) << "TICKER" 
              << std::setw(15) << "TOTAL VOL" 
              << "VWAP\n";
    std::cout << "--------------------------------------\n";
    
    for (const auto& [ticker, metric] : metrics) {
        std::cout << std::left << std::setw(10) << ticker 
                  << std::setw(15) << metric.cumulativeVolume 
                  << "$" << std::fixed << std::setprecision(4) << metric.getVWAP() << "\n";
    }
}