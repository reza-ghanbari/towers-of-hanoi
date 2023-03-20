//
// Created by reza on 05/03/23.
//

#ifndef TOWERS_OF_HANOI_SOLVER_STATE_H
#define TOWERS_OF_HANOI_SOLVER_STATE_H

#include <vector>
#include <string>
#include <iostream>
#include "../utils/Types.h"
#include "../utils/Constants.h"


class State {
private:
    Short* state; // state[i] = peg number of disk i, the bigger the index the bigger the disk
    Short* numberOfDisksInPegs;
    Short* topDiskInPegs; // topDiskInPegs[i] = disk number of the top disk in peg i, totalNumberOfDisks if peg is empty
    int totalNumberOfDisks;
    bool isLegalMove(int diskNumber, int toPeg);
public:
    State(Short* state, Short* numberOfDisksInPegs, Short* topDiskInPegs, int totalNumberOfDisks)
        : state(state)
        , numberOfDisksInPegs(numberOfDisksInPegs)
        , topDiskInPegs(topDiskInPegs)
        , totalNumberOfDisks(totalNumberOfDisks) {}
    ~State();
    Short* getState() const;
    int isGoal();
    void moveDisk(int diskNumber, int toPeg);
    Short* getNumberOfDisksInPegs() const;
    Short* getTopDiskInPegs() const;
    std::vector<State *> getChildren();
    [[nodiscard]] int getNumberOfPegsWithDisks() const;
    [[nodiscard]] State * generateChildState(int diskNumber, int targetPeg) const;
    void printState() const;
};


#endif //TOWERS_OF_HANOI_SOLVER_STATE_H
