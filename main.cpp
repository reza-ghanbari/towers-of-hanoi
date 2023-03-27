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
    auto* heuristic = new Heuristic<Long>(ABSTRACT_SIZE, "pdb-long-mid-goal.txt");
    auto* smallHeuristic = new Heuristic<SmallInt>(REMAINED_SIZE, "pdb-short-mid-goal.txt");
    auto end = std::chrono::steady_clock::now();
    std::cout << "time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() / 1000.0 << "s" << std::endl;
    Selections* selections = new Selections(heuristic, smallHeuristic);
    std::cout << "solving started..." << std::endl;
    auto* solver = new Solver(createRoot(), selections);
    start = std::chrono::steady_clock::now();
    solver->solve();
    end = std::chrono::steady_clock::now();
    std::cout << "time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() / 1000.0 << "s" << std::endl;
//    heuristic->saveToFile("pdb-long-mid-goal.txt");
//    smallHeuristic->saveToFile("pdb-short-mid-goal.txt");
    return 0;
}
