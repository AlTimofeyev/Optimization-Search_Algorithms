#include <iostream>
#include <fstream>
#include "ProcessFunctions.h"


using namespace std;

int main(int argc, char** argv)
{
    // Default configuration filename.
    string configFilename = "config.txt";

    // If a filename was provided, reassign the variable to user input.
    if (argc > 1)
        configFilename = argv[1];

    // Open the text file.
    ifstream configFile;
    configFile.open(configFilename);
    if(configFile.fail())
    {
        cout << "Failed to open file: " << configFilename << endl;
        cout << "---------------------------------------\n";
        cout << "File is either not in the right directory,\n";
        cout << "does not exist, or was not provided as\n";
        cout << "a command line argument." << endl;
        cout << "---------------------------------------\n";
        cout << "Accepted File Formats: .txt" << endl;
        cout << "---------------------------------------\n";
        cout << "**** Terminating Program Execution ****\n\n";
        return 1;
    }

    // Set the number of dimensions to test, from file.
    string line, dummyLine;
    getline(configFile, line);
    vector<int> dimensions = parseStringInt(line, ",");

    // Reset the file pointer to the beginning of file.
    configFile.clear();
    configFile.seekg(0, ios::beg);

    // Create a ProcessFunctions object.
    ProcessFunctions procFuncs;

    // Print all Function IDs to user.
    //procFuncs.printAllFunctionIDs();

    // For every dimension that was listed in the configuration file.
    for(int i = 0; i < dimensions.size(); i++)
    {
        // Consume the first line (skip it).
        getline(configFile, dummyLine);

        // Set the number of dimensions.
        procFuncs.setNumOfDimensions(dimensions[i]);

        // Create matrices with the current number of dimensions.
        while(configFile.good())
        {
            // Get the function parameters from file and prep them for matrix.
            getline(configFile, line);
            vector<double> vals = parseStringDbl(line, ",");
            prepForFunctionMatrix(vals);

            // Construct the matrix.
            procFuncs.constructMatrix(vals[0], vals[1], vals[2]);
        }

        // Calculate the fitness for all matrices and save them to file.
        procFuncs.calculateFitnessOfAllMatrices();
        //procFuncs.saveAllProcessedFunctionDataToFile(configFilename);

        // Analyze all function results and save them to file.
        procFuncs.analyzeAllFunctionResults();
        procFuncs.saveAllAnalyzedDataToFile(configFilename);

        // Run all the search algorithms and save them to file.
        procFuncs.performAllSearchAlgorithms();
        procFuncs.analyzeAllSearchAlgorithmResults();
        procFuncs.saveAllAnalyzedSearchAlgDataToFile(configFilename);

        // Print the results of the analysis.
        procFuncs.printFunctionResultsAnalysis();

        // Reset the file pointer to the beginning of file.
        configFile.clear();
        configFile.seekg(0, ios::beg);
    }

    // CLose the text file.
    configFile.close();

    return 0;
}