#include <iostream>
#include <chrono>
#include "inc/Heuristic.h"
#include "inc/Solver.h"
#include "inc/Selections.h"

State* createRoot() {
    auto* state = new Short[TOWER_SIZE]{0};
    auto* root = new State(state, TOWER_SIZE);
    delete[] state;
    root->setGCost(0);
    return root;
}

int main() {
    std::cout << "pdb creating started..." << std::endl;
    auto start = std::chrono::steady_clock::now();
    Heuristic<Long>* heuristic;
    Heuristic<SmallInt>* smallHeuristic;
    if (READ_PDB) {
        heuristic = new Heuristic<Long>(ABSTRACT_SIZE, "pdb-long.txt");
        smallHeuristic = new Heuristic<SmallInt>(REMAINED_SIZE, "pdb-short.txt");
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
