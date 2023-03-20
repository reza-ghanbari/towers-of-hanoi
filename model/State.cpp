//
// Created by reza on 05/03/23.
//

#include "../inc/State.h"

int State::isGoal() {
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

void State::moveDisk(int diskNumber, int toPeg) {
    Short fromPeg = state[diskNumber];
    numberOfDisksInPegs[fromPeg]--;
    numberOfDisksInPegs[toPeg]++;
    topDiskInPegs[toPeg] = diskNumber;
    if (diskNumber == totalNumberOfDisks - 1) {
        topDiskInPegs[fromPeg] = totalNumberOfDisks;
    }
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

std::vector<State*> State::getChildren() {
    std::vector<State*> children;
    for (int i = 0; i < NUMBER_OF_PEGS; ++i) {
        if (topDiskInPegs[i] == totalNumberOfDisks)
            continue;
        for (int j = 0; j < NUMBER_OF_PEGS; ++j) {
            if (i != j && isLegalMove(topDiskInPegs[i], j)) {
                children.push_back(generateChildState(topDiskInPegs[i], j));
            }
        }
    }
    return children;
}

State* State::generateChildState(int diskNumber, int targetPeg) const {
    auto* newState = new Short[totalNumberOfDisks];
    auto* newNumberOfDisksInPegs = new Short[NUMBER_OF_PEGS];
    auto* newTopDiskInPegs = new Short[NUMBER_OF_PEGS];
    std::copy(state, state + totalNumberOfDisks, newState);
    std::copy(numberOfDisksInPegs, numberOfDisksInPegs + NUMBER_OF_PEGS, newNumberOfDisksInPegs);
    std::copy(topDiskInPegs, topDiskInPegs + NUMBER_OF_PEGS, newTopDiskInPegs);
//    for (int k = 0; k < totalNumberOfDisks; ++k)
//        newState[k] = state[k];
//
//    for (int k = 0; k < NUMBER_OF_PEGS; ++k) {
//        newNumberOfDisksInPegs[k] = numberOfDisksInPegs[k];
//        newTopDiskInPegs[k] = topDiskInPegs[k];
//    }
    auto* child = new State(newState, newNumberOfDisksInPegs, newTopDiskInPegs, totalNumberOfDisks);
    child->moveDisk(diskNumber, targetPeg);
    return child;
}

int State::getNumberOfPegsWithDisks() const {
    int numberOfPegsWithDisks = 0;
    for (int i = 0; i < NUMBER_OF_PEGS; ++i)
        if (numberOfDisksInPegs[i] > 0)
            numberOfPegsWithDisks++;
    return numberOfPegsWithDisks;
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
