//
// Created by reza on 05/03/23.
//

#ifndef TOWERS_OF_HANOI_SOLVER_STATE_H
#define TOWERS_OF_HANOI_SOLVER_STATE_H

#include <vector>
#include <string>
#include <iostream>
#include <unordered_set>
#include "../utils/Types.h"
#include "../utils/Constants.h"


class State {
private:
    Short* state; // state[i] = peg number of disk i, the bigger the index the bigger the disk
    Short* numberOfDisksInPegs;
    Short* topDiskInPegs; // topDiskInPegs[i] = disk number of the top disk in peg i, totalNumberOfDisks if peg is empty
    int totalNumberOfDisks;
    int gCost;
    int hCost;
    bool isLegalMove(int diskNumber, int toPeg);
public:
    State(Short* state, Short* numberOfDisksInPegs, Short* topDiskInPegs, int totalNumberOfDisks)
        : state(state)
        , numberOfDisksInPegs(numberOfDisksInPegs)
        , topDiskInPegs(topDiskInPegs)
        , totalNumberOfDisks(totalNumberOfDisks) {}
    State(const Short *state, int numberOfDisks);
    ~State();
    Short* getState() const;
    int isGoal();
    void setGCost(int cost) { this->gCost = cost; };
    void setHCost(int cost) { this->hCost = cost; };
    int getGCost() const { return gCost; };
    int getCost() const { return gCost + hCost; };
    void moveDisk(Short diskNumber, Short toPeg);
    Short* getNumberOfDisksInPegs() const;
    Short* getTopDiskInPegs() const;
    std::vector<State *> getChildren(const std::unordered_map<Long, Long> & closeList = std::unordered_map<Long, Long>());
    State *generateChildState(Short diskNumber, Short targetPeg) const;
    void printState() const;
    Long getCompressedState();
};


#endif //TOWERS_OF_HANOI_SOLVER_STATE_H
