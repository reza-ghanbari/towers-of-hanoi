//
// Created by reza on 13/03/23.
//

#ifndef TOWERS_OF_HANOI_SOLVER_SOLVER_H
#define TOWERS_OF_HANOI_SOLVER_SOLVER_H

#include <algorithm>
#include <cstdlib>
#include <random>
#include <omp.h>
#include "Heuristic.h"

class CompareStates {
public:
    bool operator()(const State *lhs, const State *rhs) const {
        return (lhs->getCost() > rhs->getCost()) || (lhs->getCost() == rhs->getCost() && lhs->getGCost() > rhs->getGCost());
    }
};

class Solver {
private:
    State *root;
    int numberOfExpandedStates;
    int numberOfGeneratedStates;
    Heuristic<Long> *longHeuristic;
    Heuristic<SmallInt> *shortHeuristic;
    std::vector<std::pair<std::vector<Short>, std::vector<Short>>> randomSelections;
    State* generateState(Short* state, int numberOfDisks);
    Short getHCost(State *state);
    Long getCompressedState(const Short state[]);
    Short *getDecompressedState(Long state);
    Short *selectFromArray(Short *array, int size, std::vector<Short> selection);
    std::pair<std::vector<Short>, std::vector<Short>> generateRandomSelection();
public:
    Solver(State *root, Heuristic<Long> *longHeuristic, Heuristic<SmallInt> *shortHeuristic)
        : longHeuristic(longHeuristic)
        , shortHeuristic(shortHeuristic)
        , root(root)
        , numberOfExpandedStates(0)
        , numberOfGeneratedStates(1) {
        omp_set_num_threads(NUMBER_OF_THREADS);
        for (int i = 0; i < NUMBER_OF_SPLITS - 2; ++i) {
            randomSelections.push_back(generateRandomSelection());
        }
    };
    void solve();
};


#endif //TOWERS_OF_HANOI_SOLVER_SOLVER_H
