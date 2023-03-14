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
}

Short *State::getState() const {
    return state;
}

bool State::isLegalMove(int diskNumber, int toPeg) {
    for (int i = 0; i < diskNumber; ++i)
        if (state[i] == toPeg)
            return false;
    return true;
}

void State::moveDisk(int diskNumber, int toPeg) {
    numberOfDisksInPegs[state[diskNumber]]--;
    numberOfDisksInPegs[toPeg]++;
    state[diskNumber] = toPeg;
}

Short *State::getNumberOfDisksInPegs() const {
    return numberOfDisksInPegs;
}

std::vector<State*> State::getChildren() {
    std::vector<State*> children;
    int* evaluatedPegs = new int(NUMBER_OF_PEGS);
    for (int i = 0; i < NUMBER_OF_PEGS; ++i)
        evaluatedPegs[i] = 0;
    int numberOfEvaluatedPegs = 0;
    int numberOfPegsWithDisks = getNumberOfPegsWithDisks();
    for (int i = 0; i < totalNumberOfDisks && numberOfEvaluatedPegs < numberOfPegsWithDisks; ++i) {
        if (evaluatedPegs[state[i]] == 0) {
            evaluatedPegs[state[i]] = 1;
            numberOfEvaluatedPegs++;
            for (int j = 0; j < NUMBER_OF_PEGS; ++j) {
                if (j != state[i] && isLegalMove(i, j)) {
                    children.push_back(generateChildState(i, j));
                }
            }
        }
    }
    delete evaluatedPegs;
    return children;
}

State* State::generateChildState(int diskNumber, int targetPeg) const {
    auto* newState = new Short[totalNumberOfDisks];
    auto* newNumberOfDisksInPegs = new Short[NUMBER_OF_PEGS];
    for (int k = 0; k < totalNumberOfDisks; ++k)
        newState[k] = state[k];
    for (int k = 0; k < NUMBER_OF_PEGS; ++k)
        newNumberOfDisksInPegs[k] = numberOfDisksInPegs[k];
    auto* child = new State(newState, newNumberOfDisksInPegs, totalNumberOfDisks);
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
    for (int i = 0; i < totalNumberOfDisks; ++i) {
        std::cout << unsigned(state[i]) << " ";
    }
    std::cout << std::endl;
    for (int i = 0; i < NUMBER_OF_PEGS; ++i) {
        std::cout << unsigned(numberOfDisksInPegs[i]) << " ";
    }
    std::cout << std::endl;
}