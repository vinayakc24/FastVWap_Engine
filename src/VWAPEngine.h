#pragma once
#include "Types.h"
#include <string>
#include <string_view>
#include <unordered_map>
#include <deque>
#include <utility>

class VWAPEngine {
private:
    // Hot-path index: Maps a zero-copy string_view directly to the storage index.
    std::unordered_map<std::string_view, size_t> tickerIndex;

    // Stable storage: Owns the actual std::string keys and VWAP math.
    // std::deque is required here because push_back/emplace_back guarantees
    // that memory addresses of existing elements are never invalidated/moved.
    std::deque<std::pair<std::string, VWAPMetric>> tickerStore;

public:
    VWAPEngine() = default;

    bool processCSV(const std::string& filepath);
    void printResults() const;

private:
    void processLine(std::string_view line);
};