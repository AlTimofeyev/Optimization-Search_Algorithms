/**
 * @file SearchAlgorithms.h
 * @author  Al Timofeyev
 * @date    April 17, 2019
 * @brief   A library of Search Algorithms.
 */

#ifndef BENCHMARKFUNCTIONS2_SEARCHALGORITHMS_H
#define BENCHMARKFUNCTIONS2_SEARCHALGORITHMS_H

#include <vector>
#include "utilities.h"

using namespace std;

/** Uses Blind Search algorithm and returns the best fitness found. */
double blindSearch(int iterations, vector<double> argBest, double fitness0, int functionID, int rows, int columns, double minBound, double maxBound);

/** Uses Local Search algorithm and returns the best fitness found. */
double localSearch(vector<double> argBest, int functionID, double alpha);

/** Uses Iterative Local Search algorithm and returns a list of the best fitness found. */
vector<double> iterativeLocalSearch(int iterations, vector<double> argBest, int functionID, double alpha);

/** Creates a neighborhood of a vector using an alpha value and original vector.*/
vector<double> createNeighborhood(vector<double> origVect, double origFitness, int functionID, double alpha);



#endif //BENCHMARKFUNCTIONS2_SEARCHALGORITHMS_H
