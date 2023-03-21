//
// Created by reza on 12/03/23.
//

#ifndef TOWERS_OF_HANOI_SOLVER_HEURISTIC_H
#define TOWERS_OF_HANOI_SOLVER_HEURISTIC_H

#include <bits/stdc++.h>

#include "../utils/Types.h"
#include "../utils/Constants.h"
#include "State.h"

template <typename T>
class Heuristic {
private:
    std::unordered_map<T, Short> PDB;
    int numberOfDisks;
    void createPDB();
    static void getMappingForSymmetry(Short* mapping, const Short* numberOfDisksInPegs, const Short* topDiskInPegs);
public:
    T convertStateToInt(const Short state[], const Short mapping[]);
    explicit Heuristic(int numberOfDisks, std::string fileName = "");
    T getRank(const State* state);
    void saveToFile(const std::string& fileName);
    Short getHeuristicValue(T rank);
    State *getUnrankedState(T rank);
    Short getHeuristicValue(State *state);
    void readFromFile(const std::string &filename);
};


#endif //TOWERS_OF_HANOI_SOLVER_HEURISTIC_H
