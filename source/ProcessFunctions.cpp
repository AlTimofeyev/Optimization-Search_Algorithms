/**
 * @file ProcessFunctions.cpp
 * @class ProcessFunctions ProcessFunctions.h "ProcessFunctions.h"
 * @author  Al Timofeyev
 * @date    April 4, 2019
 * @brief   A class used to process matrices against Benchmark Functions
 *          and analyze the results.
 */

#include "FilenameConstants.h"
#include "ProcessFunctions.h"

// ----------------------------------------------
// ---------------- CONSTRUCTORS ----------------
// ----------------------------------------------
/**
 * @brief The default constructor for the ProcessFunctions class.
 * The default constructor only initializes the numOfDimensions
 * variable to 0;
 */
ProcessFunctions::ProcessFunctions()
{
    numOfDimensions = 0;
}

// -------------------------------------------------------------------------------------------
// --------------------------------- PUBLIC FUNCTIONS BELOW ----------------------------------
// -------------------------------------------------------------------------------------------
/**
 * @brief Sets the number of dimensions for the ProcessFunctions object.
 *
 * After setting the new number of dimensions, the resultsOfFunctions vector
 * that held all the previous data, for the previous number of dimensions, is
 * also reset to 0, and a new FunctionAnalysis struct is assigned to analysis.
 *
 * @param dimensions    The number of dimensions in the matrix data
 *                      (dimensions = size of each vector in the matrix).
 */
void ProcessFunctions::setNumOfDimensions(int dimensions)
{
    numOfDimensions = dimensions;
    resultsOfFunctions.resize(0);
    analysis = FunctionAnalysis();

    searchAlgResults.resize(0);
    searchAlgAnalysis = SearchAlgorithmResultsAnalysis();
}

/**
 * @brief Returns the number of dimensions used for the matrix.
 * @return The value stored in the numOfDimensions variable.
 */
int ProcessFunctions::getNumOfDimensions()
{
    return numOfDimensions;
}

/**
 * @brief Generates a matrix using Mersenne Twister.
 *
 * A matrix is constructed using the default number of dimensions, or a
 * previously user-set number of dimensions, and the default minimum and
 * maximum bound. Saves the constructed matrix to variable resultsOfFunctions.
 */
void ProcessFunctions::constructMatrix()
{
    // If the number of dimensions is 0, set it to the default value.
    if(numOfDimensions == 0)
        setNumOfDimensions(DEFAULT_NUM_OF_DIMENSIONS);

    // Create a Mersenne Twister pseudo-random number generator.
    // Generate a random function ID.
    mt19937 randGenerator(time(NULL));
    uniform_int_distribution<int> dis(1, 18);
    int funcID = dis(randGenerator);

    // Construct a matrix with user-provided boundaries.
    FunctionData funcData = generateMatrix(BOUNDARY_MIN, BOUNDARY_MAX);
    funcData.functionID = funcID;

    // Save the constructed matrix to resultsOfFunctions vector.
    resultsOfFunctions.push_back(funcData);
}

/**
 * @brief Generates a matrix using Mersenne Twister.
 *
 * A matrix is constructed using the default value of 30 dimensions, or a
 * previously user-set number of dimensions, and a user-provided minimum
 * and maximum bound. Saves the constructed matrix to variable resultsOfFunctions.
 *
 * @param funcID The function ID for which Benchmark Function the matrix is generated for.
 * @param minBoundary, maxBoundary  The minimum and maximum boundaries for the values
 *                                  in the matrix.
 */
void ProcessFunctions::constructMatrix(int funcID, double minBoundary, double maxBoundary)
{
    // If the function ID is out of range, notify user of Function IDs and exit.
    if(funcID < 1 || funcID > 18)
    {
        cout <<"\n******* ";
        cout << "Cannot generate matrix for Function ID " << funcID;
        cout <<" *******";
        printAllFunctionIDs();
        return;
    }

    // If the number of dimensions is 0, set it to the default value.
    if(numOfDimensions == 0)
        setNumOfDimensions(DEFAULT_NUM_OF_DIMENSIONS);

    // Construct a matrix with user-provided boundaries.
    FunctionData funcData = generateMatrix(minBoundary, maxBoundary);
    funcData.functionID = funcID;
    funcData.minBound = minBoundary;
    funcData.maxBound = maxBoundary;

    // Save the constructed matrix to resultsOfFunctions vector.
    resultsOfFunctions.push_back(funcData);
}

/**
 * @brief Calculates the fitness of all Matrices in resultsOfFunctions vector.
 */
void ProcessFunctions::calculateFitnessOfAllMatrices()
{
    for(int numOfData = 0; numOfData < resultsOfFunctions.size(); numOfData++)
    {
        // Record the start and end time of executing the benchmark function on matrix.
        auto startTime = chrono::high_resolution_clock::now();
        calculateMatrixFitness(resultsOfFunctions[numOfData]);
        auto endTime = chrono::high_resolution_clock::now();

        // Calculate elapsed time in milliseconds it took to execute the benchmark function.
        auto elapsedTime = endTime - startTime;
        double elapsedTimeMS = chrono::duration_cast<chrono::milliseconds>(elapsedTime).count();

        // Save elapsed time to the FunctionData struct.
        resultsOfFunctions[numOfData].timeToExecute = elapsedTimeMS;

        // Sort the results by fitness.
        quicksortFuncData(resultsOfFunctions[numOfData], 0, resultsOfFunctions[numOfData].fitness.size()-1);
    }
}

/**
 * @brief Analyzes all the results from each FunctionData structure in resultsOfFunctions.
 */
void ProcessFunctions::analyzeAllFunctionResults()
{
    for(int numOfData = 0; numOfData < resultsOfFunctions.size(); numOfData++)
        analyzeFunctionResults(resultsOfFunctions[numOfData]);
}

/**
 * @brief Performs all search algorithms for each FunctionData structure in resultsOfFunctions.
 *
 * @note This function makes a call to utilities.h --> quicksort().
 */
void ProcessFunctions::performAllSearchAlgorithms()
{
    // Open the Search Algorithm configuration text file.
    ifstream configFile;
    configFile.open("SearchAlgorithmConfig.txt");
    if(configFile.fail())
    {
        cout << "Failed to open file: SearchAlgorithmConfig.txt" << endl;
        cout << "---------------------------------------\n";
        cout << "File is either not in the right directory\n";
        cout << "or does not exist.\n";
        cout << "---------------------------------------\n";
        cout << "Accepted File Formats: .txt" << endl;
        cout << "---------------------------------------\n";
        cout << "**** Terminating Program Execution ****\n\n";
        return;
    }
    // Retrieve all necessary variable from Search Algorithm configuration file.
    // Only two lines in file: number of iterations and alpha value.
    string line;
    getline(configFile, line);
    int iterations = stoi(line);
    getline(configFile, line);
    double alpha = stod(line);

    // Declare variables for search algorithms.
    vector<double> bestVect;
    double bestFitness;
    int functionID;
    double minBound, maxBound;
    int rows = DEFAULT_NUM_OF_VECTORS, columns = numOfDimensions;

    // Perform the search algorithms on all FunctionData structs in resultsOfFunctions.
    for(int numOfData = 0; numOfData < resultsOfFunctions.size(); numOfData++)
    {
        // Assign variables for search algorithms.
        functionID = resultsOfFunctions[numOfData].functionID;
        bestVect = resultsOfFunctions[numOfData].functionMatrix[0];
        bestFitness = resultsOfFunctions[numOfData].fitness[0];
        minBound = resultsOfFunctions[numOfData].minBound;
        maxBound = resultsOfFunctions[numOfData].maxBound;

        // Declare a SearchAlgorithmResults variable.
        SearchAlgorithmResults algResults;
        algResults.functionID = functionID;

        // ------------------------------------------------------------------------------
        // ---------- BLIND SEARCH ----------
        // Record the start and end time.
        auto startTime = chrono::high_resolution_clock::now();
        algResults.fitnessBS = blindSearch(iterations, bestVect, bestFitness, functionID, rows, columns, minBound, maxBound);
        auto endTime = chrono::high_resolution_clock::now();

        // Calculate elapsed time in milliseconds.
        auto elapsedTime = endTime - startTime;
        double elapsedTimeMS = chrono::duration_cast<chrono::milliseconds>(elapsedTime).count();
        algResults.timeBS = elapsedTimeMS;

        // ------------------------------------------------------------------------------
        // ---------- LOCAL SEARCH ----------
        // Record the start and end time.
        startTime = chrono::high_resolution_clock::now();
        algResults.fitnessLS = localSearch(bestVect, functionID, alpha);
        endTime = chrono::high_resolution_clock::now();

        // Calculate elapsed time in milliseconds.
        elapsedTime = endTime - startTime;
        elapsedTimeMS = chrono::duration_cast<chrono::milliseconds>(elapsedTime).count();
        algResults.timeLS = elapsedTimeMS;

        // ------------------------------------------------------------------------------
        // ----- ITERATIVE LOCAL SEARCH -----
        // Record the start and end time.
        startTime = chrono::high_resolution_clock::now();
        algResults.fitnessILS = iterativeLocalSearch(iterations, bestVect, functionID, alpha);
        endTime = chrono::high_resolution_clock::now();

        // Sort the resulting list of fitness values.
        quicksort(algResults.fitnessILS, 0, algResults.fitnessILS.size()-1);

        // Calculate elapsed time in milliseconds.
        elapsedTime = endTime - startTime;
        elapsedTimeMS = chrono::duration_cast<chrono::milliseconds>(elapsedTime).count();
        algResults.timeILS = elapsedTimeMS;

        // Add the search algorithm results to the searchAlgResults vector.
        searchAlgResults.push_back(algResults);
    }
}

/**
 * @brief Analyzes all the results from the search algorithms.
 */
void ProcessFunctions::analyzeAllSearchAlgorithmResults()
{
    for(int numOfData = 0; numOfData < searchAlgResults.size(); numOfData++)
    {
        // Save the function ID.
        searchAlgAnalysis.functionIDs.push_back(searchAlgResults[numOfData].functionID);

        // Save Blind Search Data.
        searchAlgAnalysis.avgBSFitness.push_back(searchAlgResults[numOfData].fitnessBS);
        searchAlgAnalysis.standardDeviationBS.push_back(searchAlgResults[numOfData].fitnessBS);
        vector<double> rangeBS;
        rangeBS.push_back(searchAlgResults[numOfData].fitnessBS);
        rangeBS.push_back(searchAlgResults[numOfData].fitnessBS);
        searchAlgAnalysis.rangesBS.push_back(rangeBS);
        searchAlgAnalysis.medianBSFitness.push_back(searchAlgResults[numOfData].fitnessBS);
        searchAlgAnalysis.processTimesBS.push_back(searchAlgResults[numOfData].timeBS);

        // Save Local Search Data.
        searchAlgAnalysis.avgLSFitness.push_back(searchAlgResults[numOfData].fitnessLS);
        searchAlgAnalysis.standardDeviationLS.push_back(searchAlgResults[numOfData].fitnessLS);
        vector<double> rangeLS;
        rangeLS.push_back(searchAlgResults[numOfData].fitnessLS);
        rangeLS.push_back(searchAlgResults[numOfData].fitnessLS);
        searchAlgAnalysis.rangesLS.push_back(rangeLS);
        searchAlgAnalysis.medianLSFitness.push_back(searchAlgResults[numOfData].fitnessLS);
        searchAlgAnalysis.processTimesLS.push_back(searchAlgResults[numOfData].timeLS);

        // Save Iterative Local Search Data.
        double avgILSFitness = calculateAverage(searchAlgResults[numOfData].fitnessILS);
        searchAlgAnalysis.avgILSFitness.push_back(avgILSFitness);
        double stdDeviation = calculateStandardDeviation(searchAlgResults[numOfData].fitnessILS);
        searchAlgAnalysis.standardDeviationILS.push_back(stdDeviation);
        vector<double> rangeILS;
        rangeILS.push_back(searchAlgResults[numOfData].fitnessILS[0]);
        rangeILS.push_back(searchAlgResults[numOfData].fitnessILS[searchAlgResults[numOfData].fitnessILS.size()-1]);
        searchAlgAnalysis.rangesILS.push_back(rangeILS);
        searchAlgAnalysis.medianILSFitness.push_back(searchAlgResults[numOfData].fitnessILS[searchAlgResults[numOfData].fitnessILS.size()/2]);
        searchAlgAnalysis.processTimesILS.push_back(searchAlgResults[numOfData].timeILS);
    }
}

/**
 * @brief Saves all the matrices in resultsOfFunctions vector to files.
 *
 * @note This function makes a call to utilities.h --> parseStringStr().
 *
 * @param configFilename The configuration file from which data was generated.
 */
void ProcessFunctions::saveAllMatricesToFile(string configFilename)
{
    // Split the configuration filename based on the dot (.) delimiter.
    vector<string> splitConfig = parseStringStr(configFilename, ".");

    // Set the beginning of filename based on the number of dimensions.
    string filenameBegin = splitConfig[0] + "-";
    filenameBegin += to_string(numOfDimensions);
    filenameBegin += "DimensionalMatrix-";

    for(int numOfResults = 0; numOfResults < resultsOfFunctions.size(); numOfResults++)
    {
        // Initialize the filename with the beginning.
        string filename = filenameBegin;

        FunctionData data = resultsOfFunctions[numOfResults];
        int funcID = data.functionID;

        // Save with filename referenced by function IDs.
        switch(funcID)
        {
            case 1:
                filename += out_schefelsFilename;
                saveFunctionMatrixToFile(filename, data);
                break;
            case 2:
                filename += out_deJongsFilename;
                saveFunctionMatrixToFile(filename, data);
                break;
            case 3:
                filename += out_rosenbrockFilename;
                saveFunctionMatrixToFile(filename, data);
                break;
            case 4:
                filename += out_rastriginFilename;
                saveFunctionMatrixToFile(filename, data);
                break;
            case 5:
                filename += out_griewangkFilename;
                saveFunctionMatrixToFile(filename, data);
                break;
            case 6:
                filename += out_sEnvSWaveFilename;
                saveFunctionMatrixToFile(filename, data);
                break;
            case 7:
                filename += out_strchVSinWaveFilename;
                saveFunctionMatrixToFile(filename, data);
                break;
            case 8:
                filename += out_ackleys1Filename;
                saveFunctionMatrixToFile(filename, data);
                break;
            case 9:
                filename += out_ackleys2Filename;
                saveFunctionMatrixToFile(filename, data);
                break;
            case 10:
                filename += out_eggHolderFilename;
                saveFunctionMatrixToFile(filename, data);
                break;
            case 11:
                filename += out_ranaFilename;
                saveFunctionMatrixToFile(filename, data);
                break;
            case 12:
                filename += out_pathologicalFilename;
                saveFunctionMatrixToFile(filename, data);
                break;
            case 13:
                filename += out_michalewiczFilename;
                saveFunctionMatrixToFile(filename, data);
                break;
            case 14:
                filename += out_mastersCosWaveFilename;
                saveFunctionMatrixToFile(filename, data);
                break;
            case 15:
                filename += out_quarticFilename;
                saveFunctionMatrixToFile(filename, data);
                break;
            case 16:
                filename += out_levyFilename;
                saveFunctionMatrixToFile(filename, data);
                break;
            case 17:
                filename += out_stepFilename;
                saveFunctionMatrixToFile(filename, data);
                break;
            case 18:
                filename += out_alpineFilename;
                saveFunctionMatrixToFile(filename, data);
                break;
            default:
                cout << "Cannot Save Matrix for FunctionData->Function ID: " << funcID << endl;
                break;
        }
    }
}

/**
 * @brief Saves all the data in resultsOfFunctions to files.
 *
 * @note This function makes a call to utilities.h --> parseStringStr().
 *
 * @param configFilename The configuration file from which data was generated.
 */
void ProcessFunctions::saveAllProcessedFunctionDataToFile(string configFilename)
{
    // Split the configuration filename based on the dot (.) delimiter.
    vector<string> splitConfig = parseStringStr(configFilename, ".");

    // Set the beginning of filename based on the number of dimensions.
    string filenameBegin = splitConfig[0] + "-";
    filenameBegin += to_string(numOfDimensions);
    filenameBegin += "Dimensional-";

    for(int numOfResults = 0; numOfResults < resultsOfFunctions.size(); numOfResults++)
    {
        // Initialize the filename with the beginning.
        string filename = filenameBegin;

        FunctionData data = resultsOfFunctions[numOfResults];
        int funcID = data.functionID;

        // Save with filename referenced by function IDs.
        switch(funcID)
        {
            case 1:
                filename += out_schefelsFilename;
                saveAllFunctionDataToFile(filename, data);
                break;
            case 2:
                filename += out_deJongsFilename;
                saveAllFunctionDataToFile(filename, data);
                break;
            case 3:
                filename += out_rosenbrockFilename;
                saveAllFunctionDataToFile(filename, data);
                break;
            case 4:
                filename += out_rastriginFilename;
                saveAllFunctionDataToFile(filename, data);
                break;
            case 5:
                filename += out_griewangkFilename;
                saveAllFunctionDataToFile(filename, data);
                break;
            case 6:
                filename += out_sEnvSWaveFilename;
                saveAllFunctionDataToFile(filename, data);
                break;
            case 7:
                filename += out_strchVSinWaveFilename;
                saveAllFunctionDataToFile(filename, data);
                break;
            case 8:
                filename += out_ackleys1Filename;
                saveAllFunctionDataToFile(filename, data);
                break;
            case 9:
                filename += out_ackleys2Filename;
                saveAllFunctionDataToFile(filename, data);
                break;
            case 10:
                filename += out_eggHolderFilename;
                saveAllFunctionDataToFile(filename, data);
                break;
            case 11:
                filename += out_ranaFilename;
                saveAllFunctionDataToFile(filename, data);
                break;
            case 12:
                filename += out_pathologicalFilename;
                saveAllFunctionDataToFile(filename, data);
                break;
            case 13:
                filename += out_michalewiczFilename;
                saveAllFunctionDataToFile(filename, data);
                break;
            case 14:
                filename += out_mastersCosWaveFilename;
                saveAllFunctionDataToFile(filename, data);
                break;
            case 15:
                filename += out_quarticFilename;
                saveAllFunctionDataToFile(filename, data);
                break;
            case 16:
                filename += out_levyFilename;
                saveAllFunctionDataToFile(filename, data);
                break;
            case 17:
                filename += out_stepFilename;
                saveAllFunctionDataToFile(filename, data);
                break;
            case 18:
                filename += out_alpineFilename;
                saveAllFunctionDataToFile(filename, data);
                break;
            default:
                cout << "Cannot Save Function Results for FunctionData->Function ID: " << funcID << endl;
                break;
        }
    }
}

/**
 * @brief Saves all analyzed data in analysis to file.
 *
 * @note This function makes a call to utilities.h --> parseStringStr().
 *
 * @param configFilename The configuration file from which data was generated.
 */
void ProcessFunctions::saveAllAnalyzedDataToFile(string configFilename)
{
    // Split the configuration filename based on the dot (.) delimiter.
    vector<string> splitConfig = parseStringStr(configFilename, ".");

    // Set the filename based on the number of dimensions.
    string filename = splitConfig[0] + "-Analysis-";
    filename += to_string(numOfDimensions);
    filename += "DimensionResults.csv";

    // Rows and Columns.
    int rows = analysis.functionIDs.size(); // Fitness IDs dictates the number of rows.

    // Create the file to where the matrix is saved.
    ofstream outputFile;
    outputFile.open (filename);

    // If there are more than 0 fitness IDs, save the header line first.
    if(analysis.functionIDs.size() > 0)
        outputFile << analysis.header;

    // Save data to file.
    string line = "";
    for(int row = 0; row < rows; row++)
    {
        // Save the fitness ID.
        line += to_string(analysis.functionIDs[row]) + ",";

        // Save the average fitness.
        line += to_string(analysis.avgFunctionFitness[row]) + ",";

        // Save the standard deviation.
        line += to_string(analysis.standardDeviation[row]) + ",";

        // Save the range.
        line += to_string(analysis.ranges[row][0]) + ",";
        line += to_string(analysis.ranges[row][1]) + ",";

        // Save the median.
        line += to_string(analysis.medianFunctionFitness[row]) + ",";

        // Save the execution time.
        line += to_string(analysis.processTimes[row]) + "\n";

        // Save the row to file and clear the line string.
        outputFile << line;
        line = "";
    }

    // Close the file.
    outputFile.close();
}

/**
 * @brief Saves all analyzed data in analysis to user-specified file.
 *
 * @param filename  The name of the file where to save the analysis,
 *                  preferably an Excel (.csv) file.
 */
void ProcessFunctions::saveAllAnalyzedDataToSpecificFile(string filename)
{
    // Rows and Columns.
    int rows = analysis.functionIDs.size(); // Fitness IDs dictates the number of rows.

    // Create the file to where the matrix is saved.
    ofstream outputFile;
    outputFile.open (filename);

    // If there are more than 0 fitness IDs, save the header line first.
    if(analysis.functionIDs.size() > 0)
        outputFile << analysis.header;

    // Save data to file.
    string line = "";
    for(int row = 0; row < rows; row++)
    {
        // Save the fitness ID.
        line += to_string(analysis.functionIDs[row]) + ",";

        // Save the average fitness.
        line += to_string(analysis.avgFunctionFitness[row]) + ",";

        // Save the standard deviation.
        line += to_string(analysis.standardDeviation[row]) + ",";

        // Save the range.
        line += to_string(analysis.ranges[row][0]) + ",";
        line += to_string(analysis.ranges[row][1]) + ",";

        // Save the median.
        line += to_string(analysis.medianFunctionFitness[row]) + ",";

        // Save the execution time.
        line += to_string(analysis.processTimes[row]) + "\n";

        // Save the row to file and clear the line string.
        outputFile << line;
        line = "";
    }

    // Close the file.
    outputFile.close();
}

/**
 * @brief Saves all analyzed data in searchAlgAnalysis to file.
 *
 * @note This function makes a call to utilities.h --> parseStringStr().
 *
 * @param configFilename Configuration file from which data was generated.
 */
void ProcessFunctions::saveAllAnalyzedSearchAlgDataToFile(string configFilename)
{
    // If there is nothing to save.
    if(searchAlgResults.size() == 0)
    {
        cout << "There is nothing to save from the Search Algorithms.\n";
        cout << "**** Terminating Saving Process Execution ****\n\n";
        return;
    }

    // Split the configuration filename based on the dot (.) delimiter.
    vector<string> splitConfig = parseStringStr(configFilename, ".");

    // Set the filename based on the number of dimensions.
    string filename = splitConfig[0] + "-SearchAlgorithmAnalysis-";
    filename += to_string(numOfDimensions);
    filename += "DimensionResults.csv";

    // Rows and Columns.
    int rows = searchAlgAnalysis.functionIDs.size(); // Fitness IDs dictates the number of rows.

    // Create the file to where the matrix is saved.
    ofstream outputFile;
    outputFile.open (filename);

    // If there are more than 0 fitness IDs, save the header line first.
    if(analysis.functionIDs.size() > 0)
    {
        outputFile << searchAlgAnalysis.mainHeader;
        outputFile << searchAlgAnalysis.header;
    }

    // Save data to file.
    string line = "";
    for(int row = 0; row < rows; row++)
    {
        // Save the fitness ID.
        line += to_string(searchAlgAnalysis.functionIDs[row]) + ",";

        // Save Blind Search Data.
        line += to_string(searchAlgAnalysis.avgBSFitness[row]) + ",";
        line += to_string(searchAlgAnalysis.standardDeviationBS[row]) + ",";
        line += to_string(searchAlgAnalysis.rangesBS[row][0]) + ",";
        line += to_string(searchAlgAnalysis.rangesBS[row][1]) + ",";
        line += to_string(searchAlgAnalysis.medianBSFitness[row]) + ",";
        line += to_string(searchAlgAnalysis.processTimesBS[row]) + ",";

        // Save Local Search Data.
        line += to_string(searchAlgAnalysis.avgLSFitness[row]) + ",";
        line += to_string(searchAlgAnalysis.standardDeviationLS[row]) + ",";
        line += to_string(searchAlgAnalysis.rangesLS[row][0]) + ",";
        line += to_string(searchAlgAnalysis.rangesLS[row][1]) + ",";
        line += to_string(searchAlgAnalysis.medianLSFitness[row]) + ",";
        line += to_string(searchAlgAnalysis.processTimesLS[row]) + ",";

        // Save Iterative Local Search Data.
        line += to_string(searchAlgAnalysis.avgILSFitness[row]) + ",";
        line += to_string(searchAlgAnalysis.standardDeviationILS[row]) + ",";
        line += to_string(searchAlgAnalysis.rangesILS[row][0]) + ",";
        line += to_string(searchAlgAnalysis.rangesILS[row][1]) + ",";
        line += to_string(searchAlgAnalysis.medianILSFitness[row]) + ",";
        line += to_string(searchAlgAnalysis.processTimesILS[row]) + "\n";

        // Save the row to file and clear the line string.
        outputFile << line;
        line = "";
    }

    // Close the file.
    outputFile.close();
}

/**
 * @brief Prints all the possible Function IDs to the screen.
 *
 * Prints all possible Function ID, as well as the funtions they
 * reference, to the screen.
 */
void ProcessFunctions::printAllFunctionIDs()
{
    cout << "\n********************************************************\n";
    cout << "All Possible Function IDs and Their Respective Functions";
    cout << "\n--------------------------------------------------------\n";
    cout << "Function ID: 1\tFunction Name: Schwefels function\n";
    cout << "Function ID: 2\tFunction Name: 1st De Jongs function\n";
    cout << "Function ID: 3\tFunction Name: Rosenbrock function\n";
    cout << "Function ID: 4\tFunction Name: Rastrigin function\n";
    cout << "Function ID: 5\tFunction Name: Griewangk function\n";
    cout << "Function ID: 6\tFunction Name: Sine Envelope Sine Wave function\n";
    cout << "Function ID: 7\tFunction Name: Stretched V Sine Wave function\n";
    cout << "Function ID: 8\tFunction Name: Ackleys One function\n";
    cout << "Function ID: 9\tFunction Name: Ackleys Two function\n";
    cout << "Function ID: 10\tFunction Name: Egg Holder function\n";
    cout << "Function ID: 11\tFunction Name: Rana function\n";
    cout << "Function ID: 12\tFunction Name: Pathological function\n";
    cout << "Function ID: 13\tFunction Name: Michalewicz function\n";
    cout << "Function ID: 14\tFunction Name: Masters Cosine Wave function\n";
    cout << "Function ID: 15\tFunction Name: Quartic function\n";
    cout << "Function ID: 16\tFunction Name: Levy function\n";
    cout << "Function ID: 17\tFunction Name: Step function\n";
    cout << "Function ID: 18\tFunction Name: Alpine function\n";
    cout << "********************************************************\n\n";
}

/**
 * @brief Prints all the FunctionData structures in resultsOfFunctions.
 */
void ProcessFunctions::printFunctionResults()
{
    cout << "\n\n********************************************************\n";
    cout << "******* Printing All Data in resultsOfFunctions ********\n";
    cout << "--------------------------------------------------------\n";
    cout << "**** Number Of Dimensions: " << numOfDimensions << "\n";
    for(int numOfData = 0; numOfData < resultsOfFunctions.size(); numOfData++)
    {
        FunctionData temp = resultsOfFunctions[numOfData];
        cout << "Function ID: " << temp.functionID << "\t\t";

        cout << "Total Time To Calculate Fitness: ";
        if(temp.timeToExecute != -1.0)
            cout << temp.timeToExecute << "\n";
        else
            cout << "N/A (FITNESS NOT CALCULATED YET)\n";

        cout.precision(10);
        cout << "Fitness:\t";
        for(int row = 0; row < temp.functionMatrix.size(); row++)
        {
            if(temp.fitness.size() > 0)
            {
                if(row > 0)
                {
                    cout << "\t\t\t";
                    if(temp.fitness[row] >= 0.0)
                        cout << " ";
                    cout << temp.fitness[row] << "\t\t\t\t\t";
                }

                else
                {
                    if(temp.fitness[row] >= 0.0)
                        cout << " ";
                    cout << temp.fitness[row] << "\t\t\t";
                }
            }
            else
            {
                if(row > 0)
                    cout << "\t\t\t\t\t";
                cout << "\t\t\t\t\t";
            }

            if(row == 0)
                cout << "Matrix:\t";

            for(int col = 0; col < temp.functionMatrix[row].size(); col++)
            {
                if(temp.functionMatrix[row][col] >= 0.0)
                    cout << " ";
                cout << temp.functionMatrix[row][col] << ",\t";
            }

            cout << endl;
        }
        cout << "--------------------------------------------------------------------------\n";
        cout << "--------------------------------------------------------------------------\n\n";
    }
    cout << "********************************************************\n\n";
}

/**
 * @brief Prints all the Analysis Results in analysis.
 */
void ProcessFunctions::printFunctionResultsAnalysis()
{
    cout << "\n\n********************************************************\n";
    cout << "************** Printing Analysis Results ***************\n";
    cout << "--------------------------------------------------------\n";

    cout << "**** Number Of Dimensions: " << numOfDimensions << "\n";
    cout << "Function ID\t\tAverage Fitness\t\t\tStandard Deviation\t\t\tRange(min)\t\t\tRange(max)\t\t\t\tMedian\t\t\t\tTime(ms)\n";
    cout.precision(12);
    for(int row = 0; row < analysis.functionIDs.size(); row++)
    {
        // Print function ID.
        cout << analysis.functionIDs[row] << "\t\t\t\t";

        // Print average fitness.
        if(analysis.avgFunctionFitness[row] >= 0.0)
            cout << " ";
        cout << analysis.avgFunctionFitness[row] << "\t\t\t";

        // Print the standard deviation.
        if(analysis.standardDeviation[row] >= 0.0)
            cout << " ";
        cout << analysis.standardDeviation[row] << "\t\t\t";

        // Print the range.
        if(analysis.ranges[row][0] >= 0.0)
            cout << " ";
        cout << analysis.ranges[row][0] << "\t\t\t";
        if(analysis.ranges[row][1] >= 0.0)
            cout << " ";
        cout << analysis.ranges[row][1] << "\t\t\t";

        // Print the median.
        if(analysis.medianFunctionFitness[row] >= 0.0)
            cout << " ";
        cout << analysis.medianFunctionFitness[row] << "\t\t\t";

        // Print the Time in milliseconds.
        cout << analysis.processTimes[row] << "\n";
    }

    cout << "********************************************************\n\n";
}


// -------------------------------------------------------------------------------------------
// --------------------------------- PRIVATE FUNCTIONS BELOW ---------------------------------
// -------------------------------------------------------------------------------------------
/**
 * @brief Generates a DEFAULT_NUM_OF_VECTORS by numOfDimensions matrix using Mersenne Twister.
 *
 * A matrix is constructed using the specified number of dimensions
 * stored in numOfDimensions and a user-provided minimum and maximum bound.
 *
 * @note DEFAULT_NUM_OF_VECTORS is currently set to 30 (as of April 4, 2019).
 * @note This function makes a call to utilities.h --> createMatrix().
 *
 * @param minBoundary, maxBoundary The max/min boundaries are the range
 *                                 in which to generate numbers.
 * @return  The struct that contains the constructed matrix and an empty
 *          list of function fitness results.
 */
FunctionData ProcessFunctions::generateMatrix(double minBoundary, double maxBoundary)
{
    FunctionData generatedData;

    // Create the Matrix.
    generatedData.functionMatrix = createMatrix(DEFAULT_NUM_OF_VECTORS, numOfDimensions, minBoundary, maxBoundary);

    // Return the generated data.
    return generatedData;
}

/**
 * @brief Calculates the fitness of all vectors of a matrix.
 *
 * Calculates the fitness of all the vectors of the matrix stored
 * in a FunctionData structure. All the fitness results are stored
 * in the fitness vector variable of the same FunctionData structure.
 *
 * @note This function makes a call to utilities.h --> calculateFitnessOfMatrix().
 *
 * @param data The FunctionData structure that contains the matrix.
 */
void ProcessFunctions::calculateMatrixFitness(FunctionData &data)
{
    // Variables to hold function ID and fitness of each vector.
    int funcID = data.functionID;
    double fit;

    // Calculate the fitness of all rows in matrix.
    data.fitness = calculateFitnessOfMatrix(data.functionMatrix, funcID);
}

void ProcessFunctions::analyzeFunctionResults(FunctionData &data) /**< Analyzes the results of the functions.*/
{
    int fitnessSize = data.fitness.size();

    // Save the function ID.
    analysis.functionIDs.push_back(data.functionID);

    // Save the average fitness of data.
    double averageFitness = calculateAverage(data.fitness);
    analysis.avgFunctionFitness.push_back(averageFitness);

    // Save the standard deviation fitness of data
    double stdDeviationFitness = calculateStandardDeviation(data.fitness);
    analysis.standardDeviation.push_back(stdDeviationFitness);

    // Save the fitness ranges.
    vector<double> range;
    range.push_back(getMinFitness(data));
    range.push_back(getMaxFitness(data));
    analysis.ranges.push_back(range);

    // Save the median fitness of data.
    analysis.medianFunctionFitness.push_back(data.fitness[fitnessSize/2]);

    // Save the execution time of data.
    analysis.processTimes.push_back(data.timeToExecute);
}

/**
 * @brief Returns the minimum fitness of the data in FunctionData struct.
 * @param data The FunctionData structure that contains a list of fitness values.
 * @return The Minimum fitness in FunctionaData data structure.
 */
double ProcessFunctions::getMinFitness(FunctionData &data)
{
    return data.fitness[0];
}

/**
 * @brief Returns the maximum fitness of the data in FunctionData struct.
 * @param data The FunctionData structure that contains a list of fitness values.
 * @return The Maximum fitness in FunctionaData data structure.
 */
double ProcessFunctions::getMaxFitness(FunctionData &data)
{
    return data.fitness[data.fitness.size() - 1];
}

/**
 * @brief Saves the matrix of the FunctionData to file.
 *
 * @param filename  The filename where to store the matrix. Should be
 *                  a Excel file (.csv).
 * @param data  A FunctionData struct that contains all the data of the function,
 *              including the matrix that was used as well as the fitness
 *              result of that function.
 */
void ProcessFunctions::saveFunctionMatrixToFile(string filename, FunctionData &data)
{
    // Rows and Columns of matrix.
    int rows = data.functionMatrix.size();
    int columns = data.functionMatrix[0].size();

    // Create the file to where the matrix is saved.
    ofstream outputFile;
    outputFile.open (filename);

    // Save data to file.
    string line = "";
    for(int row = 0; row < rows; row++)
    {
        for (int col = 0; col < columns; col++)
        {
            // Add value to line.
            line += to_string(data.functionMatrix[row][col]);

            // If the end of the row has been reached.
            if(col == columns-1)
                line += "\n"; // Add a newline.
            else
                line += ","; // Else, add a comma.
        }

        // Save the row to file and clear the line string.
        outputFile << line;
        line = "";
    }

    // Close the file.
    outputFile.close();
}

/**
 * @brief Saves all the data of the function to file.
 *
 * @param filename  The filename where to store the matrix. Should be
 *                  a Excel file (.csv).
 * @param data  A FunctionData struct that contains all the data of the function,
 *              including the matrix that was used as well as the fitness
 *              result of that function.
 */
void ProcessFunctions::saveAllFunctionDataToFile(string filename, FunctionData &data)
{
    // Rows and Columns.
    int rows = data.fitness.size();                 // Fitness dictates the number of rows.
    int columns = data.functionMatrix[0].size();    // Matrix dictates the number of dimensions.

    // Create the file to where the matrix is saved.
    ofstream outputFile;
    outputFile.open (filename);

    // Save the header to file First.
    string header = "Fitness,Vector\n";
    outputFile << header;

    // Save data to file.
    string line = "";
    for(int row = 0; row < rows; row++)
    {
        // Save the fitness.
        line += to_string(data.fitness[row]) + ",";

        for (int col = 0; col < columns; col++)
        {
            // Add value to line.
            line += to_string(data.functionMatrix[row][col]);

            // If the end of the row has been reached.
            if(col == columns-1)
                line += "\n"; // Add a newline.
            else
                line += ","; // Else, add a comma.
        }

        // Save the row to file and clear the line string.
        outputFile << line;
        line = "";
    }

    // Close the file.
    outputFile.close();
}


//**********************************************************************************************
//**********************************************************************************************
/**
 * @brief Quicksort Algorithm for sorting the fitness and matrix using the fitness.
 *
 * @note Smallest (minimum) fitness gets moved to index 0, along with its vector.
 * @note Largest (maximum) fitness gets moved to the last index, along with its vector.
 *
 * @param data The FunctionData struct that hold the fitness vector and matrix.
 * @param L The starting index for the quicksort (inclusive).
 * @param R The ending index for the quicksort (inclusive).
 */
void ProcessFunctions::quicksortFuncData(FunctionData &data, int L, int R) {
    int i, j, mid;
    double piv;
    i = L;
    j = R;
    mid = L + (R - L) / 2;
    piv = data.fitness[mid];

    while (i<R || j>L)
    {
        while (data.fitness[i] < piv)
            i++;
        while (data.fitness[j] > piv)
            j--;

        if (i <= j)
        {
            swapFuncData(data, i, j); //error=swap function doesnt take 3 arguments
            i++;
            j--;
        }
        else
        {
            if (i < R)
                quicksortFuncData(data, i, R);
            if (j > L)
                quicksortFuncData(data, L, j);
            return;
        }
    }
}

/**
 * @brief Specialized swap function for the quicksort algorithm.
 *
 * @param data The FunctionData struct that hold the fitness vector and matrix.
 * @param x The 1st index of the fitness/vector for the swap.
 * @param y The 2nd index of the fitness/vector for the swap.
 */
void ProcessFunctions::swapFuncData(FunctionData &data, int x, int y)
{
    // Swap fitness values.
    double fitTemp = data.fitness[x];
    data.fitness[x] = data.fitness[y];
    data.fitness[y] = fitTemp;

    // Swap vector values.
    vector<double> vectTemp = data.functionMatrix[x];
    data.functionMatrix[x] = data.functionMatrix[y];
    data.functionMatrix[y] = vectTemp;
}