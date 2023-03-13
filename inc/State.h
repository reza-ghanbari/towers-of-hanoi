//
// Created by reza on 05/03/23.
//

#ifndef TOWERS_OF_HANOI_SOLVER_STATE_H
#define TOWERS_OF_HANOI_SOLVER_STATE_H

#include "../utils/Types.h"
#include "../utils/Constants.h"

class State {
private:
    Short* state; // state[i] = peg number of disk i, the bigger the disk number the bigger the disk
    Short* numberOfDisksInPegs;
    int totalNumberOfDisks;
    bool isLegalMove(int diskNumber, int toPeg);
public:
    State(Short* state, Short* numberOfDisksInPegs, int totalNumberOfDisks)
        : state(state)
        , numberOfDisksInPegs(numberOfDisksInPegs)
        , totalNumberOfDisks(totalNumberOfDisks) {}
    ~State();
    Short* getState();
    int isGoal();
    void moveDisk(int diskNumber, int toPeg);
    Short* getNumberOfDisksInPegs();
};


#endif //TOWERS_OF_HANOI_SOLVER_STATE_H
