//
// Created by reza on 05/03/23.
//

#ifndef TOWERS_OF_HANOI_SOLVER_CONSTANTS_H
#define TOWERS_OF_HANOI_SOLVER_CONSTANTS_H

#include <cmath>
// Problem Configurations
const int TOWER_SIZE = 16;
const int NUMBER_OF_PEGS = 5;
const int ABSTRACT_SIZE = 12;
const int REMAINED_SIZE = TOWER_SIZE - ABSTRACT_SIZE;
// Implementation Cofigurations
const int NUMBER_OF_THREADS = 20;
const int NUMBER_OF_THREAD_ITERATIONS = 5;
const int NUMBER_OF_SPLITS = NUMBER_OF_THREAD_ITERATIONS * NUMBER_OF_THREADS;
const int INITIAL_NUMBER_OF_SPLITS = 1000;
const int CALC_ALL_HEURISTICS_LIMIT = 100; //maximum value of this constant is NUMBER_OF_SPLITS
const int IS_MID_POINT_PDB = 0;
const int SAVE_PDB = 0;
const int READ_PDB = 0;

#endif //TOWERS_OF_HANOI_SOLVER_CONSTANTS_H
