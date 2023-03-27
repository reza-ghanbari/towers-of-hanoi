#include <iostream>
#include <chrono>
#include "inc/Heuristic.h"
#include "inc/Solver.h"
#include "inc/Selections.h"

State* createRoot() {
    auto* state = new Short[TOWER_SIZE]{0};
    auto* numberOfDisksInPegs = new Short[NUMBER_OF_PEGS]{0};
    numberOfDisksInPegs[0] = TOWER_SIZE;
    auto* topDiskInPegs = new Short[NUMBER_OF_PEGS]{0};
    for (int i = 1; i < NUMBER_OF_PEGS; ++i)
        topDiskInPegs[i] = TOWER_SIZE;
    State* root = new State(state
            , numberOfDisksInPegs
            , topDiskInPegs
            , TOWER_SIZE);
    root->setGCost(0);
    return root;
}

int main() {
    std::cout << "pdb creating started..." << std::endl;
    auto start = std::chrono::steady_clock::now();
    Heuristic<Long>* heuristic;
    Heuristic<SmallInt>* smallHeuristic;
    if (READ_PDB) {
        heuristic = new Heuristic<Long>(ABSTRACT_SIZE, "pdb-long-all-center.txt");
        smallHeuristic = new Heuristic<SmallInt>(REMAINED_SIZE, "pdb-short-all-center.txt");
    } else {
        heuristic = new Heuristic<Long>(ABSTRACT_SIZE);
        smallHeuristic = new Heuristic<SmallInt>(REMAINED_SIZE);
    }
    auto end = std::chrono::steady_clock::now();
    std::cout << "time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() / 1000.0 << "s" << std::endl;
    Selections* selections = new Selections(heuristic, smallHeuristic);
    std::cout << "solving started..." << std::endl;
    auto* solver = new Solver(createRoot(), selections);
    start = std::chrono::steady_clock::now();
    solver->solve();
    end = std::chrono::steady_clock::now();
    std::cout << "time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() / 1000.0 << "s" << std::endl;
    if (SAVE_PDB) {
        heuristic->saveToFile("pdb-long.txt");
        smallHeuristic->saveToFile("pdb-short.txt");
    }
    return 0;
}
