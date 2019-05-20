/**
 * @file FilenameConstants.h
 * @author  Al Timofeyev
 * @date    March 28, 2019
 * @brief   A list of input and output filenames. The input files
 *          are where the matrices are stored. The output files
 *          are where the results from the benchmark functions
 *          are stored.
 */

#ifndef BENCHMARKFUNCTIONS2_FILENAMECONSTANTS_H
#define BENCHMARKFUNCTIONS2_FILENAMECONSTANTS_H

#include <string>

using namespace std;

// ---------- FUNCTION OUTPUT FILES ----------
string out_schefelsFilename = "Schefels.csv";
string out_deJongsFilename = "DeJongs.csv";
string out_rosenbrockFilename = "Rosenbrock.csv";
string out_rastriginFilename = "Rastrigin.csv";
string out_griewangkFilename = "Griewangk.csv";
string out_sEnvSWaveFilename = "SEnvSWave.csv";
string out_strchVSinWaveFilename = "StrchVSinWave.csv";
string out_ackleys1Filename = "Ackleys1.csv";
string out_ackleys2Filename = "Ackleys2.csv";
string out_eggHolderFilename = "EggHolder.csv";
string out_ranaFilename = "Rana.csv";
string out_pathologicalFilename = "Pathological.csv";
string out_michalewiczFilename = "Michalewicz.csv";
string out_mastersCosWaveFilename = "MastersCosWave.csv";
string out_quarticFilename = "Quartic.csv";
string out_levyFilename = "Levy.csv";
string out_stepFilename = "Step.csv";
string out_alpineFilename = "Alpine.csv";


#endif //BENCHMARKFUNCTIONS2_FILENAMECONSTANTS_H
