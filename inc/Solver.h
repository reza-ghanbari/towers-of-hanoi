//
// Created by reza on 13/03/23.
//

#ifndef TOWERS_OF_HANOI_SOLVER_SOLVER_H
#define TOWERS_OF_HANOI_SOLVER_SOLVER_H

#include <algorithm>
#include <cstdlib>
#include <random>
#include "Heuristic.h"
#include "Selections.h"

typedef std::pair<Long, std::pair<Short, Short>> Item; // first = state, second.first = gCost, second.second = hCost

class CompareStates {
public:
    bool operator()(const Item lhs, const Item rhs) const {
        Short lhsCost = lhs.second.first + lhs.second.second;
        Short rhsCost = rhs.second.first + rhs.second.second;
        return (lhsCost > rhsCost)
            || (lhsCost == rhsCost && lhs.second.first > rhs.second.first);
    }
};

class Solver {
private:
    State *root;
    int numberOfExpandedStates;
    int numberOfGeneratedStates;
    Selections *selections;
    std::vector<std::pair<std::vector<Short>, std::vector<Short>>> randomSelections;
    State* generateState(Short* state, int numberOfDisks);
    State *getStateFromItem(const Item &currentItem);
public:
    Solver(State *root, Selections* selections)
        : selections(selections)
        , root(root)
        , numberOfExpandedStates(0)
        , numberOfGeneratedStates(1) {
    };
    void solve();
    void printPath(Long goalRank, std::unordered_map<Long, Long> &closedList);
};


#endif //TOWERS_OF_HANOI_SOLVER_SOLVER_H
