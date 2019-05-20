/**
 * @file SearchAlgorithms.cpp
 * @author  Al Timofeyev
 * @date    April 17, 2019
 * @brief   A library of Search Algorithms.
 */

#include "SearchAlgorithms.h"

/**
 * @brief Implementations of a Blind Search Algorithm.
 *
 * @note This function makes a call to utilities.h --> createMatrix().
 * @note This function makes a call to utilities.h --> calculateFitnessOfMatrix().
 * @note This function makes a call to utilities.h --> quicksort().
 *
 * @param iterations The number of times the Blind Search has to run.
 * @param argBest The initial vector of doubles that produced the initial best fitness.
 * @param fitness0 The initial best fitness produced by argBest.
 * @param functionID The ID of the function to use for calculating the fitness.
 * @param rows The number of vectors in the matrix.
 * @param columns The number of elements in each vector of the matrix.
 * @param minBound, maxBound The max/min boundaries are the range
 *                           in which to generate numbers.
 *
 * @return The best fitness found using Blind Search.
 */
double blindSearch(int iterations, vector<double> argBest, double fitness0, int functionID, int rows, int columns, double minBound, double maxBound)
{
    // Declare variables to hold best and current fitness from Blind Search.
    // Assign best fitness the initial best fitness value.
    double bestFitness = fitness0;
    double currFitness;

    // Declare a matrix and fitness vector (to hold a list of fitness values).
    vector<vector<double>> matrix;
    vector<double> fitnessList;

    // Start the Blind Search.
    for(int i = 0; i < iterations; i++)
    {
        // Construct a random matrix using Mersenne Twister and calculate fitness.
        matrix = createMatrix(rows, columns, minBound, maxBound);
        fitnessList = calculateFitnessOfMatrix(matrix, functionID);

        // Sort the fitness and matrix and set the best current fitness of this matrix.
        quicksort(fitnessList, matrix, 0, fitnessList.size()-1);
        currFitness = fitnessList[0];

        // If the current fitness is better than the best fitness.
        if(currFitness < bestFitness)
        {
            bestFitness = currFitness;
            argBest = matrix[0];
        }
    }

    // Return the best fitness.
    return bestFitness;
}

/**
 * @brief Implementations of a Local Search Algorithm.
 *
 * @note This function makes a call to utilities.h --> calculateFitnessOfVector().
 *
 * @param argBest The initial vector of doubles that produced the initial best fitness.
 * @param functionID The ID of the function to use for calculating the fitness.
 * @param alpha The value used to mutate the argBest vector.
 *
 * @return The best fitness found using Local Search.
 */
double localSearch(vector<double> argBest, int functionID, double alpha)
{
    // Initialize a best fitness variable for the Local Search.
    // Assign best fitness to be the fitness of argBest.
    double bestFitness = calculateFitnessOfVector(argBest, functionID);

    // Declare the neighborhood vector and its fitness variable.
    vector<double> neighborhood;
    double neighborhoodFitness;

    // Set the flag for the Local Search.
    bool isNotComplete = true;

    // Start the Local Search
    while(isNotComplete)
    {
        // Reset the Local Search flag to false.
        isNotComplete = false;

        // Generate the neighborhood.
        neighborhood = createNeighborhood(argBest, bestFitness, functionID, alpha);

        // Calculate fitness of neighborhood;
        neighborhoodFitness = calculateFitnessOfVector(neighborhood, functionID);

        // If the neighborhoodFitness is better than the best fitness,
        // continue the Local Search. Else, Local search is ended.
        if(neighborhoodFitness < bestFitness)
        {
            bestFitness = neighborhoodFitness;  // Set the new best fitness.
            argBest = neighborhood;             // Set the new best vector that produced best fitness.
            isNotComplete = true;               // Reset the loop flag to true to continue the loop.
        }
    }

    // Return the best fitness.
    return bestFitness;
}

/**
 * @brief Implementations of a Iterative Local Search Algorithm.
 *
 * @note This function makes a call to utilities.h --> calculateFitnessOfVector().
 *
 * @param iterations The maximum number of times the Iterative Local Search can run.
 * @param argBest The initial vector of doubles that produced the initial best fitness.
 * @param functionID The ID of the function to use for calculating the fitness.
 * @param alpha The value used to mutate the argBest vector.
 *
 * @return A vector of best fitness' found using Iterative Local Search.
 */
vector<double> iterativeLocalSearch(int iterations, vector<double> argBest, int functionID, double alpha)
{
    // Declare vector that will hold best fitness from each iteration.
    vector<double> bestFitnessList;

    // Initialize best global/iterative fitness variables for the Iterative Local Search.
    // Assign their initial values to be the fitness of argBest.
    double bestGlobFitness = calculateFitnessOfVector(argBest, functionID);
    double bestIterFitness = calculateFitnessOfVector(argBest, functionID);

    // Initialize a variable to hold the best iterative arg.
    // Assign its initial value to be argBest.
    vector<double> argIterBest = argBest;

    // Declare the neighborhood vector and its fitness variable.
    vector<double> neighborhood;
    double neighborhoodFitness;

    // Set the flag for the Local Search.
    bool isNotComplete = true;

    // Start the Iterative Local Search.
    for(int i = 0; i < iterations; i++)
    {
        // Start the Local Search.
        while(isNotComplete)
        {
            // Reset the Local Search flag to false.
            isNotComplete = false;

            // Generate the neighborhood.
            neighborhood = createNeighborhood(argBest, bestGlobFitness, functionID, alpha);

            // Calculate fitness of neighborhood;
            neighborhoodFitness = calculateFitnessOfVector(neighborhood, functionID);

            // If the neighborhoodFitness is better than the best fitness,
            // continue the Local Search. Else, Local search is ended.
            if(neighborhoodFitness < bestIterFitness)
            {
                bestIterFitness = neighborhoodFitness;  // Set the new best iterative fitness.
                argIterBest = neighborhood;             // Set the new best iterative vector that produced best iterative fitness.
                isNotComplete = true;                   // Reset the loop flag to true to continue the loop.
            }
        }

        // If the iterative solution is better than the best global solution.
        if(bestIterFitness < bestGlobFitness)
        {
            bestGlobFitness = bestIterFitness;  // Set the new best global fitness.
            argBest = argIterBest;              // Set the new best global vector that produced best global fitness.
        }

        // Add the global best solution to the list of best fitness values.
        bestFitnessList.push_back(bestGlobFitness);

        // Reset the Local Search flag back to true.
        isNotComplete = true;
    }

    // Return the list of best fitness values.
    return bestFitnessList;
}



/**
 * @brief Create a neighborhood of the original vector.
 *
 * @note This is used in Local Search and Iterative Local Search algorithms.
 *
 * @param origVect The original vector.
 * @param origFitness The fitness or origVect.
 * @param functionID The ID of the function to use for calculating the fitness.
 * @param alpha The value used to mutate the original vector.
 *
 * @return The neighborhood of the original vector.
 */
vector<double> createNeighborhood(vector<double> origVect, double origFitness, int functionID, double alpha)
{
    // Declare the neighborhood and neighbor variables.
    vector<double> neighborhood;
    double neighbor;

    // Make a temp variable to hold the temp fitness values.
    double tempfitness;

    // Create the neighborhood.
    for(int i = 0; i < origVect.size(); i++)
    {
        // Copy the original vector to a temporary vector.
        vector<double> tempVect = origVect;

        // Mutate the temporary vector using alpha.
        tempVect[i] = tempVect[i] + alpha;

        // Get the fitness of tempVect.
        tempfitness = calculateFitnessOfVector(tempVect, functionID);

        // Calculate the neighbor value.
        neighbor = origVect[i] - (alpha * (tempfitness - origFitness));

        // Add the neighbor to neighborhood.
        neighborhood.push_back(neighbor);
    }

    // Return the neighborhood.
    return neighborhood;
}