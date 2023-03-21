//
// Created by reza on 13/03/23.
//

#include "../inc/Solver.h"

void Solver::solve() {
    std::priority_queue<State*, std::vector<State*>, CompareStates> openList;
    std::unordered_map<Long, Long> closedList;
    root->setHCost(getHCost(root));
    std::cout << "Initial State Cost: " << unsigned(getHCost(root)) << std::endl;
    std::cout << root->getCost() << std::endl;
    openList.push(root);
    int globalCost = 0;
    while (!openList.empty()) {
        State *current = openList.top();
        openList.pop();
        numberOfExpandedStates++;
        if (current->getCost() != globalCost) {
            globalCost = current->getCost();
            std::cout << "Cost: " << globalCost << ", ";
            std::cout << "Number of Expanded States: " << numberOfExpandedStates << ", ";
            std::cout << "Number of Generated States: " << numberOfGeneratedStates << std::endl;
            if (globalCost >= 125)
                current->printState();
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
                child->setHCost(getHCost(child));
                openList.push(child);
            } else {
                delete child;
            }
        }
    }
}

Short* Solver::getDecompressedState(Long state) {
    auto* bits = new Short[TOWER_SIZE];
    for (int i = TOWER_SIZE - 1; i >= 0; i--) {
        bits[i] = state & 7;
        state >>= 3;
    }
    return bits;
}

Long Solver::getCompressedState(const Short state[]) {
    Long bits = 0;
    for (int i = 0; i < TOWER_SIZE; i++) {
        bits <<= 3;
        bits |= state[i];
    }
    return bits;
}

Short Solver::getHCost(State* state) {
    State* firstTwelve = this->generateState(state->getState(), ABSTRACT_SIZE);
    State* lastTwelve = this->generateState(state->getState() + REMAINED_SIZE, ABSTRACT_SIZE);
    State* firstFour = this->generateState(state->getState(), REMAINED_SIZE);
    State* lastFour = this->generateState(state->getState() + ABSTRACT_SIZE, REMAINED_SIZE);
    Short hCost = std::max(
            shortHeuristic->getHeuristicValue(firstFour) + longHeuristic->getHeuristicValue(lastTwelve)
            , longHeuristic->getHeuristicValue(firstTwelve) + shortHeuristic->getHeuristicValue(lastFour));
    delete firstTwelve;
    delete lastTwelve;
    delete firstFour;
    delete lastFour;
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
