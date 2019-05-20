/**
 * @file DataStructs.h
 * @author  Al Timofeyev
 * @date    April 17, 2019
 * @brief   A library of data structures for storing BenchmarkFunction data.
 */

#ifndef BENCHMARKFUNCTIONS2_DATASTRUCTS_H
#define BENCHMARKFUNCTIONS2_DATASTRUCTS_H

#include <string>
#include <vector>

using namespace std;

/**
 * @brief Function Data
 * Function Data Structure, to keep track of all the data
 * used for the Benchmark Functions.
 */
struct FunctionData
{
    int functionID;                         /**< The ID used to determine which of the 18 Benchmark Functions to use.*/
    double minBound, maxBound;              /**< The max and min bound used for the matrix.*/
    vector<double> fitness;                 /**< The list of fitness for each vector in the matrix.*/
    vector<vector<double>> functionMatrix;  /**< The matrix of double vectors.*/
    double timeToExecute = -1.0;            /**< This is time in ms to process all 30 rows.*/
};

/**
 * @brief Function Analysis
 * Function Analysis Structure, to keep track of the analysis
 * performed on each FunctionData structure. Basically, it compiles
 * and holds the averages of the calculations performed for each function.
 */
struct FunctionAnalysis
{
    string header = "Function ID,Average Fitness,Standard Deviation,Range(min),Range(max),Median,Time(ms)\n"; /**< Header used when saving the data.*/
    vector<int> functionIDs;                /**< List of function IDs.*/
    vector<double> avgFunctionFitness;      /**< List of the average fitness per FunctionData structure.*/
    vector<double> standardDeviation;       /**< List of standard fitness deviations.*/
    vector<vector<double>> ranges;          /**< List of ranges for each fitness result in resultsOfFunctions.*/
    vector<double> medianFunctionFitness;   /**< List of the Median fitness from each FunctionData structure.*/
    vector<double> processTimes;            /**< List of process times in ms for all functions.*/
};

/**
 * @brief Results of Search Algorithms.
 * Search Algorithm Results Structure, to keep track of the search
 * algorithm output.
 */
struct SearchAlgorithmResults
{
    int functionID;             /**< The ID used to determine which of the 18 Benchmark Functions to use.*/
    double fitnessBS;           /**< The best fitness found in Blinnd Search Algorithm.*/
    double timeBS;              /**< The time it took to execute the Blind Search Algorithm in milliseconds.*/
    double fitnessLS;           /**< The best fitness found in Local Search Algorithm.*/
    double timeLS;              /**< The time it took to execute the Local Search Algorithm in milliseconds.*/
    vector<double> fitnessILS;  /**< List of the best fitness' found in Iterative Local Search Algorithm.*/
    double timeILS;             /**< The time it took to execute the Iterative Local Search Algorithm in milliseconds.*/
};

/**
 * @brief Search Algorithm Analysis
 * Search Algorithm Analysis Structure, to keep track of the analysis
 * performed on each SearchAlgorithmResults structure. Basically, it
 * compiles and holds the averages of the calculations performed for
 * each search algorithm.
 */
struct SearchAlgorithmResultsAnalysis
{
    string mainHeader = " ,Blind Search, , , , , ,Local Search, , , , , ,Iterative Local Search, , , , ,\n";
    string header = "Function ID,Average Fitness,Standard Deviation,Range(min),Range(max),Median,Time(ms),Average Fitness,Standard Deviation,Range(min),Range(max),Median,Time(ms),Average Fitness,Standard Deviation,Range(min),Range(max),Median,Time(ms)\n";
    vector<int> functionIDs;                /**< List of function IDs.*/

    vector<double> avgBSFitness;            /**< List of the average Blind Search fitness per SearchAlgorithmResults structure.*/
    vector<double> standardDeviationBS;     /**< List of standard Blind Search fitness deviations per SearchAlgorithmResults structure.*/
    vector<vector<double>> rangesBS;        /**< List of ranges for each Blind Search result per SearchAlgorithmResults structure.*/
    vector<double> medianBSFitness;         /**< List of the Median Blind Search fitness from each SearchAlgorithmResults structure.*/
    vector<double> processTimesBS;          /**< List of process times in ms for each Blind Search in SearchAlgorithmResults structure.*/

    vector<double> avgLSFitness;            /**< List of the average Local Search fitness per SearchAlgorithmResults structure.*/
    vector<double> standardDeviationLS;     /**< List of standard Local Search fitness deviations per SearchAlgorithmResults structure.*/
    vector<vector<double>> rangesLS;        /**< List of ranges for each Local Search result per SearchAlgorithmResults structure.*/
    vector<double> medianLSFitness;         /**< List of the Median Local Search fitness from each SearchAlgorithmResults structure.*/
    vector<double> processTimesLS;          /**< List of process times in ms for each Local Search in SearchAlgorithmResults structure.*/

    vector<double> avgILSFitness;           /**< List of the average Iterative Local Search fitness per SearchAlgorithmResults structure.*/
    vector<double> standardDeviationILS;    /**< List of standard Iterative Local Search fitness deviations per SearchAlgorithmResults structure.*/
    vector<vector<double>> rangesILS;       /**< List of ranges for each Iterative Local Search result per SearchAlgorithmResults structure.*/
    vector<double> medianILSFitness;        /**< List of the Median Iterative Local Search fitness from each SearchAlgorithmResults structure.*/
    vector<double> processTimesILS;         /**< List of process times in ms for each Iterative Local Search in SearchAlgorithmResults structure.*/
};


#endif //BENCHMARKFUNCTIONS2_DATASTRUCTS_H
