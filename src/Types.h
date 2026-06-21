#pragma once
#include <cstdint>

/**
 * @brief Holds the running state for a single asset.
 * VWAP is calculated as: Sum(Price * Volume) / Sum(Volume)
 * We maintain the cumulative totals to allow O(1) mathematical updates
 * as new data streams in without needing to store individual trades.
 */
struct VWAPMetric {
    double cumulativeValue = 0.0;  // The numerator: Sum of (Price * Volume)
    uint64_t cumulativeVolume = 0; // The denominator: Total shares traded
    
    // [[nodiscard]] warns the compiler if the caller ignores the return value
    [[nodiscard]] double getVWAP() const {
        // Prevent division by zero if no trades occurred
        if (cumulativeVolume == 0) return 0.0;
        return cumulativeValue / static_cast<double>(cumulativeVolume);
    }
};