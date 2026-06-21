cmake_minimum_required(VERSION 3.15)
project(FastVWAP_Engine CXX)

# C++17 is strictly required for std::string_view and std::from_chars
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Aggressive compiler optimizations:
# -O3: Highest level of optimization
# -march=native: Allows the compiler to use CPU-specific vector instructions
# -Wall -Wextra: Enforces strict coding standards
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -march=native -Wall -Wextra")

# Main engine executable
add_executable(vwap_engine src/main.cpp src/VWAPEngine.cpp)

# Utility script to generate test data
add_executable(data_generator src/DataGenerator.cpp)