//
// Created by reza on 05/03/23.
//

#include <unordered_map>
#include "../inc/State.h"

int State::isGoal() {
    if (IS_MID_POINT_PDB)
        return state[totalNumberOfDisks - 1] != 0;
    else
        return numberOfDisksInPegs[NUMBER_OF_PEGS - 1] == totalNumberOfDisks;
}

State::~State() {
    delete[] state;
    delete[] numberOfDisksInPegs;
    delete[] topDiskInPegs;
}

Short *State::getState() const {
    return state;
}

bool State::isLegalMove(int diskNumber, int toPeg) {
    Short topDiskInTargetPeg = topDiskInPegs[toPeg];
    return topDiskInTargetPeg == totalNumberOfDisks || topDiskInTargetPeg >= diskNumber; // there is a condition diskNumber == topDiskInPegs[state[diskNumber]] but it is redundant because we check this in the for loop
}

void State::moveDisk(Short diskNumber, Short toPeg) {
    Short fromPeg = state[diskNumber];
    numberOfDisksInPegs[fromPeg]--;
    numberOfDisksInPegs[toPeg]++;
    topDiskInPegs[toPeg] = diskNumber;
    topDiskInPegs[fromPeg] = totalNumberOfDisks;
    for (int i = diskNumber + 1; i < totalNumberOfDisks; ++i) {
        if (state[i] == fromPeg) {
            topDiskInPegs[fromPeg] = i;
            break;
        }
    }
    state[diskNumber] = toPeg;
}

Short *State::getNumberOfDisksInPegs() const {
    return numberOfDisksInPegs;
}

std::vector<State*> State::getChildren(const std::unordered_map<Long, Long> & closeList) {
    std::vector<State*> children;
    for (int i = 0; i < NUMBER_OF_PEGS; ++i) {
        if (topDiskInPegs[i] == totalNumberOfDisks)
            continue;
        for (int j = 0; j < NUMBER_OF_PEGS; ++j) {
            if (i != j && isLegalMove(topDiskInPegs[i], j)) {
                Short diskNumber = topDiskInPegs[i];
                Short currentPeg = state[diskNumber];
                state[diskNumber] = j;
                if (closeList.find(getCompressedState()) == closeList.end()) {
                    state[diskNumber] = currentPeg;
                    children.push_back(generateChildState(diskNumber, j));
                } else {
                    state[diskNumber] = currentPeg;
                }
            }
        }
    }
    return children;
}

Long State::getCompressedState() {
    Long bits = 0;
    for (int i = 0; i < TOWER_SIZE; i++) {
        bits <<= 3;
        bits |= state[i];
    }
    return bits;
}

State* State::generateChildState(Short diskNumber, Short targetPeg) const {
    auto* newState = new Short[totalNumberOfDisks];
    auto* newNumberOfDisksInPegs = new Short[NUMBER_OF_PEGS];
    auto* newTopDiskInPegs = new Short[NUMBER_OF_PEGS];
    std::copy(state, state + totalNumberOfDisks, newState);
    std::copy(numberOfDisksInPegs, numberOfDisksInPegs + NUMBER_OF_PEGS, newNumberOfDisksInPegs);
    std::copy(topDiskInPegs, topDiskInPegs + NUMBER_OF_PEGS, newTopDiskInPegs);
    auto* child = new State(newState, newNumberOfDisksInPegs, newTopDiskInPegs, totalNumberOfDisks);
    child->setGCost(gCost + 1);
    child->moveDisk(diskNumber, targetPeg);
    return child;
}

void State::printState() const {
    std::cout << "State" << std::endl;
    for (int i = 0; i < totalNumberOfDisks; ++i) {
        std::cout << unsigned(state[i]) << " ";
    }
    std::cout << std::endl;
    std::cout << "Number of disks in pegs: ";
    for (int i = 0; i < NUMBER_OF_PEGS; ++i) {
        std::cout << unsigned(numberOfDisksInPegs[i]) << " ";
    }
    std::cout << std::endl;
    std::cout << "Top disks: ";
    for (int i = 0; i < NUMBER_OF_PEGS; ++i) {
        std::cout << ((topDiskInPegs[i] == totalNumberOfDisks) ? "-1" : std::to_string(unsigned(topDiskInPegs[i]))) << " ";
    }
    std::cout << "\n---------------" << std::endl;
}

Short *State::getTopDiskInPegs() const {
    return topDiskInPegs;
}

State::State(const Short *stateArray, int numberOfDisks) {
    totalNumberOfDisks = numberOfDisks;
    state = new Short[numberOfDisks];
    numberOfDisksInPegs = new Short[NUMBER_OF_PEGS]{0};
    topDiskInPegs = new Short[NUMBER_OF_PEGS]{0};
    std::copy(stateArray, stateArray + numberOfDisks, state);
    std::fill(topDiskInPegs, topDiskInPegs + NUMBER_OF_PEGS, numberOfDisks);
    for (int i = 0; i <= numberOfDisks - 1; ++i) {
        numberOfDisksInPegs[state[i]]++;
        if (topDiskInPegs[state[i]] == numberOfDisks)
            topDiskInPegs[state[i]] = i;
    }
}
