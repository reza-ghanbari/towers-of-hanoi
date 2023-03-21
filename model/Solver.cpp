//
// Created by reza on 13/03/23.
//

#include "../inc/Solver.h"

void Solver::solve() {
    std::priority_queue<State*, std::vector<State*>, CompareStates> openList;
    std::unordered_map<Long, Long> closedList;
    const Short mapping[] = {0, 1, 2, 3, 4};
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
        }
        if (current->isGoal()) {
            current->printState();
            return;
        }
//        if (openList.size() == 1) {
//            std::cout << "Current State: ";
//            current->printState();
//        }
        for (State *child : current->getChildren()) {
//            if (openList.size() == 1) {
//                std::cout << "Child State: ";
//                child->printState();
//            }
            numberOfGeneratedStates++;
            Long childRank = longHeuristic->convertStateToInt(child->getState(), mapping);
            if (closedList.find(childRank) == closedList.end()) {
                closedList[childRank] = longHeuristic->getRank(current);
                child->setHCost(this->getHCost(child));
                openList.push(child);
            }
//            else if (openList.size() == 1) {
//                std::cout << "Child is in Closed List" << std::endl;
//            }
        }
    }
}

Short Solver::getHCost(State* state) {
    State* firstTwelve = this->generateState(state->getState(), ABSTRACT_SIZE);
    State* lastTwelve = this->generateState(state->getState() + REMAINED_SIZE, ABSTRACT_SIZE);
    State* firstFour = this->generateState(state->getState(), REMAINED_SIZE);
    State* lastFour = this->generateState(state->getState() + ABSTRACT_SIZE, REMAINED_SIZE);
    Short hCost = std::max(
            shortHeuristic->getHeuristicValue(firstFour) + shortHeuristic->getHeuristicValue(lastTwelve)
            , shortHeuristic->getHeuristicValue(firstTwelve) + shortHeuristic->getHeuristicValue(lastFour));
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
    for (int i = 0; i < NUMBER_OF_PEGS - 1; ++i)
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
