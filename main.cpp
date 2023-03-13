#include <iostream>
#include "inc/Heuristic.h"

int main() {
    Heuristic heuristic;
    Long number = heuristic.getRank(
            *(new State(
            new Short[ABSTRACT_SIZE]{0, 0, 4, 0, 4, 4, 4, 4,4, 4, 4, 4}
            , new Short[NUMBER_OF_PEGS]{3, 0, 0, 0, 9}
            , 12))
        );
    Long number2 = heuristic.getRank(
            *(new State(
                    new Short[ABSTRACT_SIZE]{2, 2, 4, 2, 4, 4, 4, 4,4, 4, 4, 4}
                    , new Short[NUMBER_OF_PEGS]{0, 0, 3, 0, 9}
                    , 12))
    );
    std::cout << number2 << std::endl;
    std::cout << number << std::endl;
    std::cout << (number == number2) << std::endl;
    return 0;
}
