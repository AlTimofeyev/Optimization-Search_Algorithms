/**
 * @file utilities.h
 * @author  Al Timofeyev
 * @date    April 15, 2019
 * @brief   This utilities file is used as a helper file for ProcessFunctions.h
 *          and SearchAlgorithms.h, and to create matricies using the Mersenne Twister.
 */

#ifndef BENCHMARKFUNCTIONS_UTILITIES_H
#define BENCHMARKFUNCTIONS_UTILITIES_H

#define _USE_MATH_DEFINES // Uncomment if cmath constants are desirable, like M_PI.

#include <iostream>
#include <string>
#include <string.h>
#include <vector>
#include <cmath>
#include <random>
#include "BenchmarkFunctions.h"

using namespace std;

/** Parses a string of numbers into a vector of doubles.*/
vector<double> parseStringDbl(string str, string delimiter);

/** Parses a string of numbers into a vector of integers.*/
vector<int> parseStringInt(string str, string delimiter);

/** Parses a string of characters into a vector of strings.*/
vector<string> parseStringStr(string str, string delimiter);

/** Preps the setup vector for the matrix of a function by resizing to size 3.*/
void prepForFunctionMatrix(vector<double> &setup);

/** Creates a matrix with the given min/max bound for the given number of rows/columns.*/
vector<vector<double>> createMatrix(int rows, int columns, double minBound, double maxBound);

/** Calculates the fitness of a single vector.*/
double calculateFitnessOfVector(vector<double> &vect, int functionID);
/** Calculates the fitness of all vectors in matrix.*/
vector<double> calculateFitnessOfMatrix(vector<vector<double>> matrix, int functionID);

/** Calculates the average value of a vector of doubles.*/
double calculateAverage(vector<double> vect);
/** Calculates the standard deviation value of a vector of doubles.*/
double calculateStandardDeviation(vector<double> vect);

/** Special Quicksort implementation for fitness/matrices.*/
void quicksort(vector<double> &fitnessList, vector<vector<double>> &matrix, int L, int R);
/** Swap function for the Quicksort.*/
void swap(vector<double> &fitnessList, vector<vector<double>> &matrix, int x, int y);

/** Normal Quicksort implementation for vector arrays.*/
void quicksort(vector<double> &vec, int L, int R);
void swap(vector<double> &v, int x, int y);

#endif //BENCHMARKFUNCTIONS_UTILITIES_H
