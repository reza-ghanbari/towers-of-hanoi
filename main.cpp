#include <iostream>
#include <chrono>
#include "inc/Heuristic.h"

int main() {
    auto start = std::chrono::steady_clock::now();
    auto* heuristic = new Heuristic<Long>(ABSTRACT_SIZE);
    auto end = std::chrono::steady_clock::now();
    std::cout << "time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() / 1000.0 << "s" << std::endl;
    std::cout << "saving to file" << std::endl;
    start = std::chrono::steady_clock::now();
    heuristic->saveToFile("pdb1.txt");
    end = std::chrono::steady_clock::now();
    std::cout << "time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() / 1000.0 << "s" << std::endl;
    auto* smallHeuristic = new Heuristic<SmallInt>(TOWER_SIZE - ABSTRACT_SIZE);
    smallHeuristic->saveToFile("pdb2.txt");
    return 0;
}
