#pragma once
#include "Types.h"
#include <string>
#include <string_view>
#include <unordered_map>

class VWAPEngine {
private:
    // Hash map providing O(1) average time complexity for updating metrics.
    // The key is the Ticker Symbol (e.g., "AAPL"), the value is the running math.
    std::unordered_map<std::string, VWAPMetric> metrics;

public:
    VWAPEngine() = default;

    /**
     * @brief Opens the CSV and orchestrates the parsing loop.
     * @param filepath Path to the historical tick data.
     * @return true if successful, false if the file cannot be opened.
     */
    bool processCSV(const std::string& filepath);
    
    /**
     * @brief Outputs the final aggregated VWAP for all processed symbols.
     */
    void printResults() const;

private:
    /**
     * @brief The critical path function. Parses a single line of CSV.
     * Takes a string_view to prevent copying the string payload.
     */
    void processLine(std::string_view line);
};