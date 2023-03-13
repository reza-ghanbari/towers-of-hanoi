//
// Created by reza on 05/03/23.
//

#include "../inc/State.h"

int State::isGoal() {
    return numberOfDisksInPegs[NUMBER_OF_PEGS - 1] == totalNumberOfDisks;
}

State::~State() {
    delete[] state;
}

Short *State::getState() {
    return state;
}

bool State::isLegalMove(int diskNumber, int toPeg) {
    for (int i = 0; i < diskNumber; ++i)
        if (state[i] == toPeg)
            return false;
    return true;
}

void State::moveDisk(int diskNumber, int toPeg) {
    if (isLegalMove(diskNumber, toPeg)) {
        numberOfDisksInPegs[state[diskNumber]]--;
        numberOfDisksInPegs[toPeg]++;
        state[diskNumber] = toPeg;
    }

}

Short *State::getNumberOfDisksInPegs() {
    return numberOfDisksInPegs;
}
