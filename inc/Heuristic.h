//
// Created by reza on 12/03/23.
//

#ifndef TOWERS_OF_HANOI_SOLVER_HEURISTIC_H
#define TOWERS_OF_HANOI_SOLVER_HEURISTIC_H

#include <bitset>
#include <bits/stdc++.h>

#include "../utils/Types.h"
#include "../utils/Constants.h"
#include "State.h"


class Heuristic {
private:
    std::unordered_map<Long, Short> PDB;
    void createPDB();
    static void getMappingForSymmetry(uint8_t *mapping, const uint8_t *numberOfDisksInPegs);
    Long convertStateToInt(Short state[], Short mapping[]);
public:
    Heuristic();
    Long getRank(const State* state);
    void saveToFile();
    Long getHeuristicValue(const State *state);
};


#endif //TOWERS_OF_HANOI_SOLVER_HEURISTIC_H
