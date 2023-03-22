//
// Created by reza on 13/03/23.
//

#include "../inc/Solver.h"

void Solver::solve() {
    std::priority_queue<Item, std::vector<Item>, CompareStates> openList;
    std::unordered_map<Long, Long> closedList;
    openList.emplace(getCompressedState(root->getState()), std::make_pair(0, getHCost(root)));
    int globalCost = 0;
    delete root;
    while (!openList.empty()) {
        State *current = getStateFromItem(openList.top());
        openList.pop();
        numberOfExpandedStates++;
        if (current->getCost() != globalCost) {
            globalCost = current->getCost();
            std::cout << "Cost: " << globalCost
                << ", Number of Expanded States: " << numberOfExpandedStates
                << ", Number of Generated States: " << numberOfGeneratedStates
                << ", closed list size: " << closedList.size()
                << ", open list size: " << openList.size() << std::endl;
            if (globalCost > 70) break;
        }
        if (current->isGoal()) {
            current->printState();
            return;
        }
        Long currentRank = getCompressedState(current->getState());
        for (State *child : current->getChildren()) {
            numberOfGeneratedStates++;
            Long childRank = getCompressedState(child->getState());
            if (closedList.find(childRank) == closedList.end()) {
                closedList[childRank] = currentRank;
                openList.emplace(getCompressedState(child->getState()), std::make_pair(child->getGCost(), getHCost(child)));
            }
            delete child;
        }
        delete current;
    }
}

State *Solver::getStateFromItem(const Item &currentItem) {
    Long state = currentItem.first;
    Short stateArray[TOWER_SIZE];
    for (int i = TOWER_SIZE - 1; i >= 0; i--) {
        stateArray[i] = state & 7;
        state >>= 3;
    }
    State *current = generateState(stateArray, TOWER_SIZE);
    current->setGCost(currentItem.second.first);
    current->setHCost(currentItem.second.second);
    return current;
}

Long Solver::getCompressedState(const Short state[]) {
    Long bits = 0;
    for (int i = 0; i < TOWER_SIZE; i++) {
        bits <<= 3;
        bits |= state[i];
    }
    return bits;
}

Short* Solver::selectFromArray(const Short* array, int size, std::vector<Short> selection) {
    auto* newArray = new Short[size];
    for (int i = 0; i < size; ++i) {
        newArray[i] = array[selection[i]];
    }
    return newArray;
}

Short Solver::getHCost(State* state) {
    Short* stateArray = state->getState();
    State* firstTwelve = this->generateState(stateArray, ABSTRACT_SIZE);
    State* lastTwelve = this->generateState(stateArray + REMAINED_SIZE, ABSTRACT_SIZE);
    State* firstFour = this->generateState(stateArray, REMAINED_SIZE);
    State* lastFour = this->generateState(stateArray + ABSTRACT_SIZE, REMAINED_SIZE);
    Short hCost = std::max(
            shortHeuristic->getHeuristicValue(firstFour) + longHeuristic->getHeuristicValue(lastTwelve)
            , longHeuristic->getHeuristicValue(firstTwelve) + shortHeuristic->getHeuristicValue(lastFour));
    #pragma omp parallel for reduction(max:hCost)
    for (auto & randomSelection : randomSelections) {
        hCost = std::max(hCost, getHCostOfSelection(stateArray, randomSelection));
    }
    delete firstTwelve;
    delete lastTwelve;
    delete firstFour;
    delete lastFour;
    return hCost;
}

inline Short
Solver::getHCostOfSelection(const Short *stateArray, const std::pair<std::vector<Short>, std::vector<Short>> &randomSelection) {
    Short biggerStateSArray[ABSTRACT_SIZE];
    Short smallerStateArray[REMAINED_SIZE];
    std::vector<Short> longerSelection = randomSelection.first;
    std::vector<Short> shorterSelection = randomSelection.second;
    for (int i = 0; i < ABSTRACT_SIZE; ++i) {
        biggerStateSArray[i] = stateArray[longerSelection[i]];
    }
    for (int i = 0; i < REMAINED_SIZE; ++i) {
        smallerStateArray[i] = stateArray[shorterSelection[i]];
    }
    State* biggerState = generateState(biggerStateSArray, ABSTRACT_SIZE);
    State* smallerState = generateState(smallerStateArray, REMAINED_SIZE);
    Short hCost = longHeuristic->getHeuristicValue(biggerState) + shortHeuristic->getHeuristicValue(smallerState);
    delete biggerState;
    delete smallerState;
    return hCost;
}

State *Solver::generateState(Short *state, int numberOfDisks) {
    auto* tempState = new Short[numberOfDisks];
    std::copy(state, state + numberOfDisks, tempState);
    auto* numberOfDisksInPegs = new Short[NUMBER_OF_PEGS]{0};
    auto* topDiskInPegs = new Short[NUMBER_OF_PEGS]{0};
    for (int i = 0; i < NUMBER_OF_PEGS; ++i)
        topDiskInPegs[i] = numberOfDisks;
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

std::pair<std::vector<Short>, std::vector<Short>> Solver::generateRandomSelection() {
    std::vector<Short> pool(TOWER_SIZE);
    std::vector<Short> longArray;
    std::vector<Short> shortArray;
    for (int i = 0; i < TOWER_SIZE; i++)
        pool[i] = i;
    std::sample(pool.begin(), pool.end(), std::back_inserter(longArray), ABSTRACT_SIZE, std::mt19937(std::random_device()()));
    for (int number : pool) {
        if (std::find(longArray.begin(), longArray.end(), number) == longArray.end()) {
            shortArray.push_back(number);
        }
    }
    return std::make_pair(longArray, shortArray);
}
