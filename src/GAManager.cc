/*******************************************************************

   Project: MC-Graph, a C++ implementation of genetic algorithms
            for graph drawing and visualization
   License: Creative Commons, Attribution
   Author:  Dana Vrajitoru
   File:    GAManager.cc
   Update:  October 2022

   A manager class for the execution of the genetic algorithm.

*******************************************************************/

#include <cstdio>
#include <iostream>
#include "GAManager.h"
#include "General.h"
#include "GenEval.h"
#include "Individual.h"
#include "Generation.h"
#include "GenOper.h"
#include "EvalBool.h"
#include "SettingFile.h"

ofstream GAManager::resFile, GAManager::histFile;
unsigned int GAManager::randSeed = 201;
GenInfo *GAManager::aGenInfo = NULL;
RunInfo *GAManager::aRunInfo = NULL;
EvalInfo *GAManager::anEvalInfo = NULL;
Generation *GAManager::initPop = NULL;
SATexpr *GAManager::globalSAT = NULL;

// Default constructor intializing all the attributes
GAManager::GAManager()
{}


// Destructor: delete all the objects
GAManager::~GAManager()
{
    FreeAll();
}

// Deallocate all the data structures used.
void GAManager::FreeAll()
{
    delete initPop;
    initPop = NULL;
    delete aGenInfo;
    aGenInfo = NULL;
    delete aRunInfo;
    aRunInfo = NULL;
    delete anEvalInfo;
    anEvalInfo = NULL;
    deleteGlobalSAT();
}

// Create the initial population for the run
void GAManager::InitialPopulation()
{
    // Initialize the population based on the type of fitness function
    if (anEvalInfo->realEval || anEvalInfo->funcNumber < 20)
        initPop = new Generation(aRunInfo->genSize, aGenInfo, binRand);
    else
        initPop = new Generation(aRunInfo->genSize, aGenInfo, limRand);

    // Uncomment to see what the initial population looks like
    //initPop->Print("old");
}

// Initialize settings and files to run the GA
void GAManager::GAInit()
{
    // Read the settings from a dictionary and initialize the
    // settings storing objects from it.
    InitSettings();

    // load problem data from files if needed
    if ((anEvalInfo->probFileName != NULL) &&
        (anEvalInfo->probFileName[0] != '\0') &&
        (anEvalInfo->probFileName[0] != ' '))
        if (anEvalInfo->funcNumber < 20)
            ReadSATexpr(anEvalInfo->probFileName);
    // else: add other functions to load data from a file here.

    // apply runtime settings
    randSeed = aRunInfo->rseed;
    srand(randSeed);

    // Start out the file storing results
    InitSaveResults();
}

// Deprecated: Read the data from the console
void GAManager::ReadData()
{
    int dirty = 0;
    FILE *gaFile;
    gaFile = fopen(settingFile, "r");
    aGenInfo = new GenInfo(gaFile);
    dirty += aGenInfo->AskInfo();
    aRunInfo = new RunInfo(gaFile);
    dirty += aRunInfo->AskRunInfo();
    anEvalInfo = new EvalInfo(gaFile);
    dirty += anEvalInfo->AskEvalInfo();
    fclose(gaFile);
    if (dirty)
        SaveData();
    ReadSATexpr(anEvalInfo->probFileName);
}

// Deprecated: read the settings from a file
void GAManager::SaveData()
{
    FILE *gaFile;
    gaFile = fopen(settingFile, "w");
    aGenInfo->FPrint(gaFile);
    aRunInfo->FPrint(gaFile);
    anEvalInfo->FPrint(gaFile);
    fclose(gaFile);
}

// Open the files storing the results and the history.
// Result files record the results at the end of each trial.
// History files also record intermediary results.
void GAManager::InitSaveResults()
{
    char *resName;

    resName = aRunInfo->resFileName;
    // Open the result file and write current settings into it
    if (resName[0] != '0') {
        resFile.open(resName);
        resFile << "random seed\tpopulation size\tindividual size\t";
        resFile << "probCross\tprobMut\tgeneration number\n";
        resFile << randSeed << "\t\t" << aRunInfo->genSize << "\t\t"
            << aGenInfo->indSize << "\t\t" << aGenInfo->probCross << "\t"
            << aGenInfo->probMut << "\t" << aRunInfo->genNumber << endl;
    }
    // open the history file if it's turned on and write current settings into it
    if (aRunInfo->history) {
        if ((aRunInfo->hFileName == NULL) ||
            (aRunInfo->hFileName[0] == '\0') ||
            (aRunInfo->hFileName[0] == ' ')) {
            cout << "history filename" << endl;
            cin >> resName;
            histFile.open(resName, ios::app);
        }
        else
            histFile.open(aRunInfo->hFileName);
        histFile << "random seed\tpopulation size\tindividual size\t";
        histFile << "probCross\tprobMut\tgeneration number" << endl;
        histFile << randSeed << "\t\t" << aRunInfo->genSize << "\t\t"
            << aGenInfo->indSize << "\t\t" << aGenInfo->probCross << "\t"
            << aGenInfo->probMut << "\t" << aRunInfo->genNumber << endl;
    }
}

// Execute a GA trial for a choice of the parameters.
// It runs a number of trials with a different random seed
// based on the setting.
void GAManager::ExecuteTrialSet()
{
    int j, nTrials;

    aRunInfo->theCross->Print();
    randSeed = aRunInfo->rseed;
    nTrials = aRunInfo->trialNr;
    resFile << endl << "crossover form = " 
            << aRunInfo->theCross->MethodString()
            << endl << "Best fitness:";
    if (aRunInfo->history)
        histFile << endl << "crossover form = " 
                 << aRunInfo->theCross->MethodString() << endl;

    // Run the GA for the given number of trials
    for (j = 0; j < nTrials; j++) {
        // randomize the population again for the new trial
        srand(randSeed);
        if (anEvalInfo->realEval || anEvalInfo->funcNumber < 20)
            initPop->ReInit(binRand);
        else
            initPop->ReInit(limRand);
        EvalGen(initPop, anEvalInfo);
        initPop->Sort();
        resFile << endl << initPop->fitValues[initPop->popSize - 1] << "\t";

        // Increment the random seed by randInc for the next trial
        randSeed = randSeed + randInc;
        if (aRunInfo->history)
            histFile << endl << "Trial number " << j << endl;

        // run one trial
        cout << "Trial number " << j << endl;
        initPop->GARun(aRunInfo);
        resFile.close();
        resFile.open(aRunInfo->resFileName, ios::app);
    }
}

// Run the genetic algorithm based on the settings
void GAManager::ExecuteGA()
{
    int i, k;

    // Initialize the fitness array
    InitFitArray(aRunInfo->genSize);

    // execute a GA trial set based on the choice of crossover operation
    if (aRunInfo->theCross->theCrossover == allCrs)
        for (i = onePointCrs; i < allCrs; i++) {
            aRunInfo->theCross->theCrossover = CrossForm(i);
            if ((i == dissocCrs) && (aRunInfo->theCross->dissForm == allOp))
                for (k = zeroOp; k < allOp; k++) {
                    aRunInfo->theCross->dissForm = DissocForm(k);
                    ExecuteTrialSet();
                }
            else if ((i == combineCrs) && (aRunInfo->theCross->combForm == allCmb))
                for (k = perfCmb; k < allCmb; k++) {
                    aRunInfo->theCross->theCrossover = combineCrs;
                    aRunInfo->theCross->combForm = CombineForm(k);
                    ExecuteTrialSet();
                }
            else
                ExecuteTrialSet();
        }
    else if ((aRunInfo->theCross->theCrossover == dissocCrs) &&
        (aRunInfo->theCross->dissForm == allOp))
        for (k = zeroOp; k < allOp; k++) {
            aRunInfo->theCross->dissForm = DissocForm(k);
            ExecuteTrialSet();
        }
    else if ((aRunInfo->theCross->theCrossover == combineCrs) &&
        (aRunInfo->theCross->combForm == allCmb))
        for (k = perfCmb; k < allCmb; k++) {
            aRunInfo->theCross->theCrossover = combineCrs;
            aRunInfo->theCross->combForm = CombineForm(k);
            ExecuteTrialSet();
        }
    else
        ExecuteTrialSet();
    resFile << endl << endl;
    resFile.close();
    if (aRunInfo->history) {
        histFile << endl << endl;
        histFile.close();
    }
}

