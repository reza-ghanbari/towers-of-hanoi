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
    std::fill(topDiskInPegs, topDiskInPegs + NUMBER_OF_PEGS, numberOfDisks);
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
    auto* topDiskInPegs = new Short[NUMBER_OF_PEGS];
    auto* goalNumberOfDisksInPegs = new Short[NUMBER_OF_PEGS]{0};
    std::fill(goalState, goalState + numberOfDisks, NUMBER_OF_PEGS - 1);
    std::fill(topDiskInPegs, topDiskInPegs + NUMBER_OF_PEGS, numberOfDisks);
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
            if (PDB.find(childRank) == PDB.end()) {
                PDB[childRank] = PDB[currentRank] + 1;
                queue.push(childRank);
            }
            delete child;
        }
        delete current;
    }
    delete root;
}

template <typename T>
void Heuristic<T>::saveToFile(const std::string& fileName) {
    std::ofstream file;
    file.open(fileName);
    for (auto &i : PDB) {
        file << i.first << " " << unsigned(i.second) << std::endl;
    }
    file.close();
}

template <typename T>
void Heuristic<T>::readFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file " << filename << std::endl;
        return;
    }
    std::string line;
    while (std::getline(file, line)) {
        uint64_t key;
        uint8_t value;
        size_t spacePos = line.find(' ');
        if (spacePos != std::string::npos) {
            key = std::stoull(line.substr(0, spacePos));
            value = std::stoi(line.substr(spacePos + 1));
            PDB[key] = value;
        } else {
            std::cerr << "Failed to parse line: " << line << std::endl;
            break;
        }
    }
    file.close();
}

template <typename T>
Heuristic<T>::Heuristic(int numberOfDisks, std::string fileName) {
    this->numberOfDisks = numberOfDisks;
    if (fileName.empty()) {
        createPDB();
    } else {
        readFromFile(fileName);
    }
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
    Short zero = 0, one = 1, two = 2, three = 3;
    Short first = numberOfDisksInPegs[zero];
    Short second = numberOfDisksInPegs[one];
    if (first < second || (first == second && topDiskInPegs[zero] < topDiskInPegs[one])) {
        std::swap(zero, one);
    }
    first = numberOfDisksInPegs[two];
    second = numberOfDisksInPegs[three];
    if (first < second || (first == second && topDiskInPegs[two] < topDiskInPegs[three])) {
        std::swap(two, three);
    }
    first = numberOfDisksInPegs[zero];
    second = numberOfDisksInPegs[two];
    if (first < second || (first == second && topDiskInPegs[zero] < topDiskInPegs[two])) {
        std::swap(zero, two);
    }
    first = numberOfDisksInPegs[one];
    second = numberOfDisksInPegs[three];
    if (first < second || (first == second && topDiskInPegs[one] < topDiskInPegs[three])) {
        std::swap(one, three);
    }
    first = numberOfDisksInPegs[one];
    second = numberOfDisksInPegs[two];
    if (first < second || (first == second && topDiskInPegs[one] < topDiskInPegs[two])) {
        std::swap(one, two);
    }
    mapping[zero] = 0;
    mapping[one] = 1;
    mapping[two] = 2;
    mapping[three] = 3;
}
