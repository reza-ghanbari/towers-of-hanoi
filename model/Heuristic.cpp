//
// Created by reza on 12/03/23.
//

#include "../inc/Heuristic.h"

template class Heuristic<Long>;
template class Heuristic<SmallInt>;

template <typename T>
T Heuristic<T>::getRank(const State* state) {
    Short * numberOfDisksInPegs = state->getNumberOfDisksInPegs();
    Short* topDiskInPegs = state->getTopDiskInPegs();
    Short mapping[NUMBER_OF_PEGS];
    for (int i = 0; i < NUMBER_OF_PEGS; ++i)
        mapping[i] = i;
    getMappingForSymmetry(mapping, numberOfDisksInPegs, topDiskInPegs);
    Long rank = convertStateToInt(state->getState(), mapping);
    return rank;
}

template <typename T>
State* Heuristic<T>::getUnrankedState(T rank) {
    std::bitset<64> bits(rank);
    auto* state = new Short[numberOfDisks];
    auto* numberOfDisksInPegs = new Short[NUMBER_OF_PEGS]{0};
    auto* topDiskInPegs = new Short[NUMBER_OF_PEGS]{0};
    for (int i = 0; i < NUMBER_OF_PEGS; ++i) {
        topDiskInPegs[i] = numberOfDisks;
    }
    for (int i = numberOfDisks - 1; i >= 0; --i) {
        state[i] = bits.to_ulong() & 7;
        bits >>= 3;
    }
    for (int i = 0; i <= numberOfDisks - 1; ++i) {
        numberOfDisksInPegs[state[i]]++;
        if (topDiskInPegs[state[i]] == numberOfDisks)
            topDiskInPegs[state[i]] = i;
    }
    return new State(state, numberOfDisksInPegs, topDiskInPegs, numberOfDisks);
}

template <typename T>
T Heuristic<T>::convertStateToInt(const Short state[], const Short mapping[]) {
    T bits = 0;
    for (int i = 0; i <= numberOfDisks - 1; i++) {
        bits <<= 3;
        bits |= mapping[state[i]];
    }
    return bits;
}

template <typename T>
void Heuristic<T>::createPDB() {
    std::queue<T> queue;
    auto* goalState = new Short[numberOfDisks]{0};
    for (int i = 0; i <= numberOfDisks - 1; ++i)
        goalState[i] = NUMBER_OF_PEGS - 1;
    auto* goalNumberOfDisksInPegs = new Short[NUMBER_OF_PEGS]{0};
    auto* topDiskInPegs = new Short[NUMBER_OF_PEGS]{0};
    for (int i = 0; i < NUMBER_OF_PEGS - 1; ++i)
        topDiskInPegs[i] = numberOfDisks;
    goalNumberOfDisksInPegs[NUMBER_OF_PEGS - 1] = numberOfDisks;
    auto *root = new State(goalState
            , goalNumberOfDisksInPegs
            , topDiskInPegs
            , numberOfDisks);
    T rootRank = getRank(root);
    queue.push(rootRank);
    PDB[rootRank] = 0;
    while (!queue.empty()) {
        T currentRank = queue.front();
        State *current = getUnrankedState(currentRank);
        queue.pop();
        for (auto &child: current->getChildren()) {
            T childRank = getRank(child);
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

template <typename T>
void Heuristic<T>::saveToFile(std::string fileName) {
    std::ofstream file;
    file.open(fileName);
    for (auto &i : PDB) {
        file << i.first << " " << unsigned(i.second) << std::endl;
    }
    file.close();
}

template <typename T>
Heuristic<T>::Heuristic(int numberOfDisks) {
    this->numberOfDisks = numberOfDisks;
    createPDB();
}

template <typename T>
Short Heuristic<T>::getHeuristicValue(T rank) {
    return this->PDB[rank];
}

template <typename T>
Short Heuristic<T>::getHeuristicValue(State *state) {
    return this->PDB[getRank(state)];
}

template <typename T>
void Heuristic<T>::getMappingForSymmetry(Short *mapping, const Short *numberOfDisksInPegs, const Short* topDiskInPegs) {
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
