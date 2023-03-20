//
// Created by reza on 12/03/23.
//

#include "../inc/Heuristic.h"

Long Heuristic::getRank(const State* state) {
    Short * numberOfDisksInPegs = state->getNumberOfDisksInPegs();
    Short* topDiskInPegs = state->getTopDiskInPegs();
    Short mapping[NUMBER_OF_PEGS];
    for (int i = 0; i < NUMBER_OF_PEGS; ++i)
        mapping[i] = i;
    getMappingForSymmetry(mapping, numberOfDisksInPegs, topDiskInPegs);
    Long rank = convertStateToInt(state->getState(), mapping);
    return rank;
}

State* Heuristic::getUnrankedState(Long rank) {
    std::bitset<64> bits(rank);
    auto* state = new Short[ABSTRACT_SIZE];
    auto* numberOfDisksInPegs = new Short[NUMBER_OF_PEGS]{0};
    auto* topDiskInPegs = new Short[NUMBER_OF_PEGS]{0};
    for (int i = 0; i < NUMBER_OF_PEGS; ++i) {
        topDiskInPegs[i] = ABSTRACT_SIZE;
    }
    for (int i = ABSTRACT_SIZE - 1; i >= 0; --i) {
        state[i] = bits.to_ulong() & 7;
        bits >>= 3;
    }
    for (int i = 0; i < ABSTRACT_SIZE; ++i) {
        numberOfDisksInPegs[state[i]]++;
        if (topDiskInPegs[state[i]] == ABSTRACT_SIZE)
            topDiskInPegs[state[i]] = i;
    }
    return new State(state, numberOfDisksInPegs, topDiskInPegs, ABSTRACT_SIZE);
}


Long Heuristic::convertStateToInt(Short state[], Short mapping[]) {
    std::bitset<64> bits(0);
    for (int i = 0; i < ABSTRACT_SIZE; i++) {
        std::bitset<64> b(mapping[state[i]]);
        bits <<= 3;
        bits |= b;
    }
    return bits.to_ulong();
}

void Heuristic::createPDB() {
    std::queue<Long> queue;
    Short* goalState = new Short[ABSTRACT_SIZE]{0};
    for (int i = 0; i < ABSTRACT_SIZE; ++i)
        goalState[i] = NUMBER_OF_PEGS - 1;
    Short* goalNumberOfDisksInPegs = new Short[NUMBER_OF_PEGS]{0};
    Short* topDiskInPegs = new Short[NUMBER_OF_PEGS]{0};
    for (int i = 0; i < NUMBER_OF_PEGS - 1; ++i)
        topDiskInPegs[i] = ABSTRACT_SIZE;
    goalNumberOfDisksInPegs[NUMBER_OF_PEGS - 1] = ABSTRACT_SIZE;
    auto *root = new State(goalState
            , goalNumberOfDisksInPegs
            , topDiskInPegs
            , ABSTRACT_SIZE);
    Long rootRank = getRank(root);
    queue.push(rootRank);
    PDB[rootRank] = 0;
    while (!queue.empty()) {
        Long currentRank = queue.front();
        State *current = getUnrankedState(currentRank);
        queue.pop();
        for (auto &child: current->getChildren()) {
            Long childRank = getRank(child);
            auto entry = PDB.find(childRank);
            if (entry == PDB.end() || (entry->second > PDB[currentRank] + 1)) {
                PDB[childRank] = PDB[currentRank] + 1;
                queue.push(childRank);
            }
            delete child;
        }
        delete current;
    }
}

void Heuristic::saveToFile() {
    std::ofstream file;
    file.open("pdb.txt");
    for (auto &i : PDB) {
        file << i.first << " " << unsigned(i.second) << std::endl;
    }
    file.close();
}

Heuristic::Heuristic() {
    createPDB();
}

Long Heuristic::getHeuristicValue(const State* state) {
    return this->PDB[getRank(state)];
}

void Heuristic::getMappingForSymmetry(Short *mapping, const Short *numberOfDisksInPegs, const Short* topDiskInPegs) {
    auto *temp = new Short[NUMBER_OF_PEGS - 1];
    auto *tempTopDiskInPegs = new Short[NUMBER_OF_PEGS - 1];
    for (int i = 0; i < NUMBER_OF_PEGS - 1; ++i) {
        temp[i] = numberOfDisksInPegs[i];
        tempTopDiskInPegs[i] = topDiskInPegs[i];
    }
    if (temp[0] < temp[1] || (temp[0] == temp[1] && tempTopDiskInPegs[0] < tempTopDiskInPegs[1])) {
        std::swap(tempTopDiskInPegs[0], tempTopDiskInPegs[1]);
        std::swap(temp[0], temp[1]);
        std::swap(mapping[0], mapping[1]);
    }
    if (temp[2] < temp[3] || (temp[2] == temp[3] && tempTopDiskInPegs[2] < tempTopDiskInPegs[3])) {
        std::swap(tempTopDiskInPegs[2], tempTopDiskInPegs[3]);
        std::swap(temp[2], temp[3]);
        std::swap(mapping[2], mapping[3]);
    }
    if (temp[0] < temp[2] || (temp[0] == temp[2] && tempTopDiskInPegs[0] < tempTopDiskInPegs[2])) {
        std::swap(tempTopDiskInPegs[0], tempTopDiskInPegs[2]);
        std::swap(temp[0], temp[2]);
        std::swap(mapping[0], mapping[2]);
    }
    if (temp[1] < temp[3] || (temp[1] == temp[3] && tempTopDiskInPegs[1] < tempTopDiskInPegs[3])) {
        std::swap(tempTopDiskInPegs[1], tempTopDiskInPegs[3]);
        std::swap(temp[1], temp[3]);
        std::swap(mapping[1], mapping[3]);
    }
    if (temp[1] < temp[2] || (temp[1] == temp[2] && tempTopDiskInPegs[1] < tempTopDiskInPegs[2])) {
        std::swap(tempTopDiskInPegs[1], tempTopDiskInPegs[2]);
        std::swap(temp[1], temp[2]);
        std::swap(mapping[1], mapping[2]);
    }
    auto* indices = new Short[NUMBER_OF_PEGS]{0};
    for (int i = 0; i < NUMBER_OF_PEGS - 1; ++i) {
        indices[mapping[i]] = i;
    }
    for (int i = 0; i < NUMBER_OF_PEGS - 1; ++i) {
        mapping[i] = indices[i];
    }
    delete[] indices;
    delete[] temp;
    delete[] tempTopDiskInPegs;
}
