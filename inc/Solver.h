//
// Created by reza on 13/03/23.
//

#ifndef TOWERS_OF_HANOI_SOLVER_SOLVER_H
#define TOWERS_OF_HANOI_SOLVER_SOLVER_H


#include "Heuristic.h"

class Solver {
private:
    State* root;
    Heuristic *heuristic;
public:
    Solver(State* root, Heuristic *heuristic): heuristic(heuristic), root(root) {};
    void solve();
};


#endif //TOWERS_OF_HANOI_SOLVER_SOLVER_H
