//
// Created by reza on 13/03/23.
//

#include "../inc/Solver.h"

void Solver::solve() {
    std::priority_queue<Item, std::vector<Item>, CompareStates> openList;
    std::unordered_map<Long, Long> closedList;
    openList.emplace(root->getCompressedState(), std::make_pair(0, getHCost(root)));
    int globalCost = 0;
    delete root;
    while (!openList.empty()) {
        Long currentRank = openList.top().first;
        State *current = getStateFromItem(openList.top());
        openList.pop();
        numberOfExpandedStates++;
        if (current->getCost() != globalCost) {
            globalCost = current->getCost();
            std::cout << "Cost: " << globalCost
                << ", Number of Expanded States: " << numberOfExpandedStates
                << ", Number of Generated States: " << numberOfGeneratedStates
                << ", open list size: " << openList.size() << std::endl;
            if (globalCost >= 70) break;
        }
        if (current->isGoal()) {
            printPath(currentRank, closedList);
            return;
        }
        for (State *child : current->getChildren(closedList)) {
            numberOfGeneratedStates++;
            Long childRank = child->getCompressedState();
            closedList[childRank] = currentRank;
            openList.emplace(childRank, std::make_pair(child->getGCost(), getHCost(child)));
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
    Short currentElement;
    Short smallerStateArray[REMAINED_SIZE];
    Short biggerStateArray[ABSTRACT_SIZE];
    std::vector<Short> shorterSelection = randomSelection.second;
    std::vector<Short> longerSelection = randomSelection.first;
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

State *Solver::generateState(Short *state, int numberOfDisks) {
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

void Solver::printPath(Long goalRank, std::unordered_map<Long, Long> &closedList) {
    std::vector<Long> path;
    auto rank = goalRank;
    while ((rank = closedList[rank]) != 0) {
        path.push_back(goalRank);
    }
    path.push_back(0);
    std::reverse(path.begin(), path.end());
    for (Long state : path) {
        State *currentState = getStateFromItem(std::make_pair(state, std::make_pair(0, 0)));
        currentState->printState();
        delete currentState;
    }
}
