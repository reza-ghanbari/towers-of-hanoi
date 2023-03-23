//
// Created by reza on 22/03/23.
//

#ifndef TOWERS_OF_HANOI_SOLVER_SELECTIONS_H
#define TOWERS_OF_HANOI_SOLVER_SELECTIONS_H

#include <vector>
#include <algorithm>
#include <random>
#include <omp.h>
#include <unordered_set>
#include "../utils/Types.h"
#include "../utils/Constants.h"
#include "State.h"
#include "Heuristic.h"

typedef std::pair<SmallInt, int> SelectionKeyValue;

class Selections {
private:
    std::unordered_map<SmallInt, SelectionKeyValue> randomSelections;
    int numberOfSelectionCalls;
    std::vector<SelectionPair> finalSelections;
    Heuristic<Long> *longHeuristic;
    Heuristic<SmallInt> *shortHeuristic;
    std::vector<SmallInt> selectedHeuristics;
    SelectionPair generatePairFromNumber(SmallInt randomSelection) const;
    std::vector<SelectionPair> getRandomSelections();
    Short getHCostOfSelection(const Short *stateArray, const SelectionPair& randomSelection) const;
    State *generateState(const Short *state, int numberOfDisks);
    std::vector<Short> convertNumberToSelection(int number) const;
public:
    Selections(Heuristic<Long> *longHeuristic, Heuristic<SmallInt> *shortHeuristic);
    Short getHCost(const Short *stateArray);
};


#endif //TOWERS_OF_HANOI_SOLVER_SELECTIONS_H
