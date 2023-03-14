#include <iostream>
#include <chrono>
#include "inc/Heuristic.h"

int main() {
    auto start = std::chrono::steady_clock::now();
    auto* heuristic = new Heuristic();
    auto end = std::chrono::steady_clock::now();
    std::cout << "time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() / 1000.0 << "s" << std::endl;
    std::cout << "saving to file" << std::endl;
    start = std::chrono::steady_clock::now();
    heuristic->saveToFile();
    end = std::chrono::steady_clock::now();
    std::cout << "time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() / 1000.0 << "s" << std::endl;
    return 0;
}
