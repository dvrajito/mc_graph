/*******************************************************************

   Project: MC-Graph, a C++ implementation of genetic algorithms
            for graph drawing and visualization
   License: Creative Commons, Attribution
   Author:  Dana Vrajitoru
   File:    GAManager.h
   Update:  October 2022

   A manager class for the execution of the genetic algorithm.

*******************************************************************/

#ifndef GAMANAGER_h
#define GAMANAGER_h

#include <fstream>
using namespace std;

#include "Generation.h"
#include "RunInfo.h"
#include "EvalInfo.h"
#include "GenInfo.h"
#include "SATexpr.h"

class GAManager {
public:
    static ofstream resFile, histFile;
    static unsigned int randSeed;
    static GenInfo *aGenInfo;
    static RunInfo *aRunInfo;
    static EvalInfo *anEvalInfo;
    static Generation *initPop;
    static SATexpr *globalSAT;

    // Default constructor intializing all the attributes
    GAManager();

    // Destructor: delete all the objects
    ~GAManager();

    // Deallocate all the data structures used.
    void FreeAll();

    // Create the initial population for the run
    void InitialPopulation();

    // Initialize settings and files to run the GA
    void GAInit();

    // Deprecated: Read the data from the console
    void ReadData();

    // Deprecated: read the settings from a file
    void SaveData();

    // Open the files storing the results and the history.
    // Result files record the results at the end of each trial.
    // History files also record intermediary results.
    void InitSaveResults();

    // Execute a GA trial for a choice of the parameters.
    // It runs a number of trials with a different random seed
    // based on the setting.
    void ExecuteTrialSet();

    // Run the genetic algorithm based on the settings
    void ExecuteGA();

};

#endif
