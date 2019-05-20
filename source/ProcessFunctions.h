/**
 * @file ProcessFunctions.h
 * @author  Al Timofeyev
 * @date    April 17, 2019
 * @brief   A class used to process matrices against Benchmark Functions
 *          and analyze the results.
 */

#ifndef BENCHMARKFUNCTIONS2_PROCESSFUNCTIONS_H
#define BENCHMARKFUNCTIONS2_PROCESSFUNCTIONS_H


#include <iostream>
#include <fstream>
#include <random>
#include <chrono>
#include "utilities.h"
#include "DataStructs.h"
#include "SearchAlgorithms.h"

using namespace std;

// -------------- CONSTANTS --------------
/** The default minimum number of dimensions. */
#define DEFAULT_NUM_OF_DIMENSIONS 30
/** The default number of vectors per matrix. */
#define DEFAULT_NUM_OF_VECTORS 30
/** The default minimum boundary for the elements generated. */
#define BOUNDARY_MIN -500.0
/** The default maximum boundary for the elements generated. */
#define BOUNDARY_MAX 500.0

class ProcessFunctions{
public:
    // --------------------- Constructor Declarations ---------------------
    ProcessFunctions(); // Sets the number of dimensions to 0;

    // --------------------- Functions Declarations ---------------------
    void setNumOfDimensions(int dimensions);    /**< Sets the number of dimensions.*/
    int getNumOfDimensions();                   /**< Returns the number of dimensions.*/

    void constructMatrix();                                                      /**< Uses all default constants, or previously user-set dimensions.*/
    void constructMatrix(int funcID, double minBoundary, double maxBoundary);    /**< Uses default number of dimensions.*/

    void calculateFitnessOfAllMatrices();   /**< Calculates Fitness for all matrices in resultsOfFunctions.*/
    void analyzeAllFunctionResults();       /**< Analyzes all the results from resultsOfFunctions.*/

    void performAllSearchAlgorithms();          /**< Executes all the search algorithms.*/
    void analyzeAllSearchAlgorithmResults();    /**< Analyzes all the results from the search algorithms.*/

    void saveAllMatricesToFile(string configFilename);                      /**< Saves all the matrices in resultsOfFunctions to files.*/
    void saveAllProcessedFunctionDataToFile(string configFilename);    /**< Saves all the data in resultsOfFunctions to files.*/
    void saveAllAnalyzedDataToFile(string configFilename);             /**< Saves all analyzed data in analysis to file.*/
    void saveAllAnalyzedDataToSpecificFile(string filename);           /**< Saves all analyzed data in analysis to user-specified file.*/
    void saveAllAnalyzedSearchAlgDataToFile(string configFilename);    /**< Saves all analyzed data in searchAlgAnalysis to file.*/

    void printAllFunctionIDs();             /**< Prints all the possible Function IDs to the screen.*/
    void printFunctionResults();            /**< Prints all the FunctionData structures in resultsOfFunctions.*/
    void printFunctionResultsAnalysis();    /**< Prints all the Analysis Results in analysis.*/

private:
    // --------------------------- Variables ----------------------------
    int numOfDimensions;
    vector<FunctionData> resultsOfFunctions;
    FunctionAnalysis analysis;

    vector<SearchAlgorithmResults> searchAlgResults;
    SearchAlgorithmResultsAnalysis searchAlgAnalysis;

    // --------------------- Functions Declarations ---------------------
    FunctionData generateMatrix(double minBoundary, double maxBoundary);            /**< Generates a matrix using min/max boundaries.*/

    void calculateMatrixFitness(FunctionData &data);                                /**< Calculates the fitness of all vectors in matrix.*/
    void analyzeFunctionResults(FunctionData &data);                                /**< Analyzes the results of the functions.*/
    double getMinFitness(FunctionData &data);                                       /**< Returns the minimum fitness of data.*/
    double getMaxFitness(FunctionData &data);                                       /**< Returns the maximum fitness of data.*/

    void saveFunctionMatrixToFile(string filename, FunctionData &data);    /**< Saves the matrix to file.*/
    void saveAllFunctionDataToFile(string filename, FunctionData &data);   /**< Saves the results of the function and it's data to file.*/

    void quicksortFuncData(FunctionData &data, int L, int R);   /**< Special quicksort implementation.*/
    void swapFuncData(FunctionData &data, int x, int y);
};


#endif //BENCHMARKFUNCTIONS2_PROCESSFUNCTIONS_H
