//
// Created by reza on 13/03/23.
//

#ifndef TOWERS_OF_HANOI_SOLVER_SOLVER_H
#define TOWERS_OF_HANOI_SOLVER_SOLVER_H


#include "Heuristic.h"

class CompareStates {
public:
    bool operator()(const State *lhs, const State *rhs) const {
        return lhs->getCost() > rhs->getCost();
    }
};

class Solver {
private:
    State *root;
    Heuristic<Long> *longHeuristic;
    Heuristic<SmallInt> *shortHeuristic;
    State* generateState(Short* state, int numberOfDisks);
    Short getHCost(State *state);
public:
    Solver(State *root, Heuristic<Long> *longHeuristic, Heuristic<SmallInt> *shortHeuristic)
        : longHeuristic(longHeuristic), shortHeuristic(shortHeuristic), root(root) {};
    void solve();
};


#endif //TOWERS_OF_HANOI_SOLVER_SOLVER_H
