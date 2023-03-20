#include <iostream>
#include <chrono>
#include "inc/Heuristic.h"
#include "inc/Solver.h"

State* createRoot() {
    auto* state = new Short[TOWER_SIZE]{0};
    auto* numberOfDisksInPegs = new Short[NUMBER_OF_PEGS]{0};
    numberOfDisksInPegs[0] = TOWER_SIZE;
    auto* topDiskInPegs = new Short[NUMBER_OF_PEGS]{0};
    for (int i = 1; i < NUMBER_OF_PEGS; ++i)
        topDiskInPegs[i] = TOWER_SIZE;
    return new State(state
            , numberOfDisksInPegs
            , topDiskInPegs
            , TOWER_SIZE);
}

int main() {
    auto start = std::chrono::steady_clock::now();
    auto* heuristic = new Heuristic<Long>(ABSTRACT_SIZE);
    auto* smallHeuristic = new Heuristic<SmallInt>(TOWER_SIZE - ABSTRACT_SIZE);
    std::cout << "solving started..." << std::endl;
    auto* solver = new Solver(createRoot(), heuristic, smallHeuristic);
    solver->solve();
    auto end = std::chrono::steady_clock::now();
    std::cout << "time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() / 1000.0 << "s" << std::endl;
//    std::cout << "saving to file" << std::endl;
//    start = std::chrono::steady_clock::now();
//    heuristic->saveToFile("pdb-long.txt");
//    end = std::chrono::steady_clock::now();
//    std::cout << "time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() / 1000.0 << "s" << std::endl;
//    smallHeuristic->saveToFile("pdb-short.txt");
    return 0;
}
