//
// Created by reza on 13/03/23.
//

#include "../inc/Solver.h"
#include <iomanip>

void Solver::solve() {
    std::priority_queue<Item, std::vector<Item>, CompareStates> openList;
    std::unordered_map<Long, Long> closedList;
    openList.emplace(root->getCompressedState(), std::make_pair(0, selections->getHCost(root->getState())));
    int globalCost = 0;
    Short spaceSize = 12;
    delete root;
    while (!openList.empty()) {
        Long currentRank = openList.top().first;
        State *current = getStateFromItem(openList.top());
        openList.pop();
        numberOfExpandedStates++;
        if (current->getCost() != globalCost) {
            globalCost = current->getCost();
            std::cout << std::left << "cost: " << globalCost
                << "   expanded: " << std::setw(spaceSize) << numberOfExpandedStates
                << " generated: " << std::setw(spaceSize) << numberOfGeneratedStates
                << std::endl;
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
            openList.emplace(childRank, std::make_pair(child->getGCost(), selections->getHCost(child->getState())));
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
    auto *current = new State(stateArray, TOWER_SIZE);
    current->setGCost(currentItem.second.first);
    current->setHCost(currentItem.second.second);
    return current;
}

void Solver::printPath(Long goalRank, std::unordered_map<Long, Long> &closedList) {
    std::vector<Long> path;
    auto rank = goalRank;
    while ((rank = closedList[rank]) != 0) {
        path.push_back(rank);
    }
    path.push_back(0);
    std::reverse(path.begin(), path.end());
    std::cout << "Solution found in length: " << path.size() << std::endl;
    std::cout << "Path: " << std::endl;
    for (Long state : path) {
        State *currentState = getStateFromItem(std::make_pair(state, std::make_pair(0, 0)));
        currentState->printState();
        delete currentState;
    }
}
