//
// Created by reza on 22/03/23.
//



#include "../inc/Selections.h"

Selections::Selections(Heuristic<Long> *longHeuristic, Heuristic<SmallInt> *shortHeuristic) {
    omp_set_num_threads(NUMBER_OF_THREADS);
    this->longHeuristic = longHeuristic;
    this->shortHeuristic = shortHeuristic;
    finalSelections = std::vector<SelectionPair>();
    numberOfSelectionCalls = 0;
//    std::uniform_int_distribution<> sampler(0, TOWER_SIZE - 1);
//    std::vector<Short> digits;
    std::vector<int> digits = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    std::random_device rd;
    std::mt19937 gen(rd());
    std::unordered_map<SmallInt, int> selections;
    while (selections.size() < INITIAL_NUMBER_OF_SPLITS) {
        shuffle(digits.begin(), digits.end(), gen);
//        while (digits.size() < 4) {
//            int sample = sampler(gen);
//            if (std::find(digits.begin(), digits.end(), sample) == digits.end()) {
//                digits.push_back(sample);
//            }
//        }
        int hexNum = digits[0] << 12 | digits[1] << 8 | digits[2] << 4 | digits[3];
//        digits.clear();
        if (selections.find(hexNum) == selections.end()) {
            selections[hexNum] = 0;
        }
    }
    int i = 0;
    for (auto selection : selections) {
        randomSelections[i++] = selection;
    }
    digits.clear();
}

std::vector<SelectionPair> Selections::getRandomSelections() {
    if (numberOfSelectionCalls < CALC_ALL_HEURISTICS_LIMIT) {
        std::vector<SelectionPair> selectionPairs;
        for (auto & randomSelection : randomSelections) {
            selectionPairs.push_back(generatePairFromNumber(randomSelection.second.first));
        }
        return selectionPairs;
    } else {
        std::vector<SelectionKeyValue> allSelections;
        for (auto & randomSelection : randomSelections) {
            allSelections.push_back(randomSelection.second);
        }
        std::sort(allSelections.begin(), allSelections.end(), [](const SelectionKeyValue &a, const SelectionKeyValue &b) {
            return a.second > b.second;
        });
        size_t selectedHeuristicsSize = selectedHeuristics.size();
        for (int i = 0; i < selectedHeuristicsSize; ++i) {
            finalSelections.push_back(generatePairFromNumber(selectedHeuristics[i]));
        }
        for (size_t i = selectedHeuristicsSize; i < NUMBER_OF_SPLITS; ++i) {
            finalSelections.push_back(generatePairFromNumber(allSelections[i - selectedHeuristicsSize].first));
        }
        allSelections.clear();
        randomSelections.clear();
        selectedHeuristics.clear();
        return finalSelections;
    }
}

SelectionPair Selections::generatePairFromNumber(SmallInt randomSelection) const {
    SelectionPair selectionPair;
    selectionPair.first = convertNumberToSelection(randomSelection);
    for (int i = 0; i < TOWER_SIZE; ++i) {
        if (std::find(selectionPair.first.begin(), selectionPair.first.end(), i) == selectionPair.first.end())
            selectionPair.second.push_back(i);
    }
    return selectionPair;
}

std::vector<Short> Selections::convertNumberToSelection(int number) const {
    std::vector<Short> selection;
    for (int i = 0; i < 4; ++i) {
        selection.push_back(number & 15);
        number >>= 4;
    }
    return selection;
}

State *Selections::generateState(const Short *state, int numberOfDisks) {
    auto* tempState = new Short[numberOfDisks];
    std::copy(state, state + numberOfDisks, tempState);
    auto* numberOfDisksInPegs = new Short[NUMBER_OF_PEGS]{0};
    auto* topDiskInPegs = new Short[NUMBER_OF_PEGS]{0};
    std::fill(topDiskInPegs, topDiskInPegs + NUMBER_OF_PEGS, numberOfDisks);
    for (int i = 0; i <= numberOfDisks - 1; ++i) {
        numberOfDisksInPegs[tempState[i]]++;
        if (topDiskInPegs[tempState[i]] == numberOfDisks)
            topDiskInPegs[tempState[i]] = i;
    }
    return new State(tempState
            , numberOfDisksInPegs
            , topDiskInPegs
            , numberOfDisks);
}

Short Selections::getHCost(const Short* stateArray) {
    State* firstTwelve = this->generateState(stateArray, ABSTRACT_SIZE);
    State* lastTwelve = this->generateState(stateArray + REMAINED_SIZE, ABSTRACT_SIZE);
    State* firstFour = this->generateState(stateArray, REMAINED_SIZE);
    State* lastFour = this->generateState(stateArray + ABSTRACT_SIZE, REMAINED_SIZE);
    Short hCost = std::max(
            shortHeuristic->getHeuristicValue(firstFour) + longHeuristic->getHeuristicValue(lastTwelve)
            , longHeuristic->getHeuristicValue(firstTwelve) + shortHeuristic->getHeuristicValue(lastFour));
    if (numberOfSelectionCalls == 0) {
        numberOfSelectionCalls++;
        return hCost;
    }
    if (numberOfSelectionCalls < CALC_ALL_HEURISTICS_LIMIT) {
        numberOfSelectionCalls++;
        int i = 0;
        int selected = -1;
        for (auto & randomSelection : getRandomSelections()) {
            Short currentHCost = getHCostOfSelection(stateArray, randomSelection);
            randomSelections[i].second += currentHCost;
            if (currentHCost > hCost) {
                selected = randomSelections[i].first;
                hCost = currentHCost;
            }
            i++;
        }
        if (selected != -1 && std::find(selectedHeuristics.begin(), selectedHeuristics.end(), selected) == selectedHeuristics.end())
            selectedHeuristics.push_back(selected);
    }
    else {
        #pragma omp parallel for reduction(max:hCost)
        for (auto &randomSelection: finalSelections) {
            hCost = std::max(hCost, getHCostOfSelection(stateArray, randomSelection));
        }
    }
    delete firstTwelve;
    delete lastTwelve;
    delete firstFour;
    delete lastFour;
    return hCost;
}

inline Short
Selections::getHCostOfSelection(const Short *stateArray, const SelectionPair& randomSelection) const {
    Short currentElement;
    Short smallerStateArray[REMAINED_SIZE];
    Short biggerStateArray[ABSTRACT_SIZE];
    std::vector<Short> shorterSelection = randomSelection.first;
    std::vector<Short> longerSelection = randomSelection.second;
    Short numberOfDisksInPegs[NUMBER_OF_PEGS]{0};
    Short topDiskInPegs[NUMBER_OF_PEGS]{0};
    std::fill(topDiskInPegs, topDiskInPegs + NUMBER_OF_PEGS, ABSTRACT_SIZE);
    for (int i = 0; i < ABSTRACT_SIZE; ++i) {
        currentElement = stateArray[longerSelection[i]];
        biggerStateArray[i] = currentElement;
        numberOfDisksInPegs[currentElement]++;
        if (topDiskInPegs[currentElement] == ABSTRACT_SIZE)
            topDiskInPegs[currentElement] = i;
    }
    Short biggerStateHCost = longHeuristic->getHeuristicValue(biggerStateArray, numberOfDisksInPegs, topDiskInPegs);
    std::fill(topDiskInPegs, topDiskInPegs + NUMBER_OF_PEGS, REMAINED_SIZE);
    std::fill(numberOfDisksInPegs, numberOfDisksInPegs + NUMBER_OF_PEGS, 0);
    for (int i = 0; i < REMAINED_SIZE; ++i) {
        currentElement = stateArray[shorterSelection[i]];
        smallerStateArray[i] = currentElement;
        numberOfDisksInPegs[currentElement]++;
        if (topDiskInPegs[currentElement] == REMAINED_SIZE)
            topDiskInPegs[currentElement] = i;
    }
    return biggerStateHCost + shortHeuristic->getHeuristicValue(smallerStateArray, numberOfDisksInPegs, topDiskInPegs);
}