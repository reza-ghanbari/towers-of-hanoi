#include <iostream>
#include "inc/Heuristic.h"

int main() {
    Heuristic heuristic;
    auto* root = new State(new Short[ABSTRACT_SIZE]{0}
        , new Short[NUMBER_OF_PEGS]{ABSTRACT_SIZE}
        , ABSTRACT_SIZE);
    for (auto &i : root->getChildren()) {
        for (auto &c: i->getChildren()) {
            Short* s = c->getState();
            for (int j = 0; j < ABSTRACT_SIZE; ++j) {
                std::cout << unsigned(s[j]) << " ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
    return 0;
}
