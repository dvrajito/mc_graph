/********************************************************************

   Project: MC-Graph, a C++ implementation of genetic algorithms
            for graph drawing and visualization
   License: Creative Commons, Attribution
   Author:  Dana Vrajitoru
   File:    RunInfo.cc
   Update:  October 2022

   A class handling some run-time settings such as generation number,
   random seed, number of trials, and others.

*********************************************************************/

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
using namespace std;
#include "RunInfo.h"
#include "General.h"

/************************* constructors *****************************/

// Default constructor
RunInfo::RunInfo()
{
    Init();
}

// Constructor from a file
RunInfo::RunInfo(FILE *aFile)
{
    Init(aFile);
}

// Constructor from a dictionary of (setting, value).
RunInfo::RunInfo(char **aDict)
{
    Init(aDict);
}

// Copy constructor
RunInfo::RunInfo(RunInfo *data)
{
    Init(data);
}

// Constructor with values for all attributes
RunInfo::RunInfo(int aSize,
                 int aNbr,
                 int isHistory,
                 int aRseed,
                 int aTrialNr,
                 int aReportNr,
                 char *aFileName,
                 char *rFileName,
                 RepForm aForm,
                 CrossMethod *aCross)
{
    Init(aSize, aNbr, isHistory,
         aRseed, aTrialNr, aReportNr,
         aFileName, rFileName, aForm, aCross);
}


/* desctructor */
RunInfo::~RunInfo()
{
    delete hFileName;
    delete resFileName;
    delete theCross;
}

/************************** Initialize ******************************/

// Default initialization
void RunInfo::Init()
{
    genSize = 0;
    genNumber = 0;
    history = 0;
    rseed = 10;
    trialNr = 1;
    reportNr = 1;
    hFileName = NULL;
    resFileName = NULL;
    aRForm = simple;
    theCross = new CrossMethod;
}

// Initialize the information from a file
void RunInfo::Init(FILE *aFile)
{
    Init();
    if (aFile == NULL)
        ScanRunInfo();
    else {
        SkipLine(aFile);
        fscanf(aFile, "%d", &(genSize));
        SkipLine(aFile);
        fscanf(aFile, "%d", &(genNumber));
        SkipLine(aFile);
        fscanf(aFile, "%d", &(history));
        if (history) {
            SkipLine(aFile);
            hFileName = new char[50];
            fscanf(aFile, "%s", hFileName);
        }
        SkipLine(aFile);
        SkipLine(aFile);
        fscanf(aFile, "%d", &(aRForm));
        theCross = new CrossMethod(aFile);
    }
}

// Initialize the information from a dictionary of (setting, value).
void RunInfo::Init(char **aDict)
{
    int pSize, gNbr, isHistory, aRseed, aTrialNr, aReportNr, i;
    RepForm aForm;
    CrossMethod *aCross;
    int hfilename, rFileName;

    for (i = 0; i < OPT_NR; i++)
        if (strcmp(aDict[2 * i], "population size") == 0)
            pSize = atoi(aDict[2 * i + 1]);
        else if (strcmp(aDict[2 * i], "generation number") == 0)
            gNbr = atoi(aDict[2 * i + 1]);
        else if (strcmp(aDict[2 * i], "history") == 0)
            isHistory = atoi(aDict[2 * i + 1]);
        else if (strcmp(aDict[2 * i], "random seed") == 0)
            aRseed = atoi(aDict[2 * i + 1]);
        else if (strcmp(aDict[2 * i], "number of trials") == 0)
            aTrialNr = atoi(aDict[2 * i + 1]);
        else if (strcmp(aDict[2 * i], "number of reports") == 0)
            aReportNr = atoi(aDict[2 * i + 1]);
        else if (strcmp(aDict[2 * i], "history file") == 0)
            hfilename = 2 * i + 1;
        else if (strcmp(aDict[2 * i], "result file") == 0)
            rFileName = 2 * i + 1;
        else if (strcmp(aDict[2 * i], "reproduction form") == 0)
            aForm = (RepForm)atoi(aDict[2 * i + 1]);
    aCross = new CrossMethod(aDict);

    Init(pSize, gNbr, isHistory,
         aRseed, aTrialNr, aReportNr,
         aDict[hfilename], aDict[rFileName], aForm, aCross);
}

// Copy initialization
void RunInfo::Init(RunInfo *data)
{
    Init(data->genSize, data->genNumber, data->history,
         data->rseed, data->trialNr, data->reportNr,
         data->hFileName, data->resFileName, data->aRForm, data->theCross);
}

// Initialization with values for all attributes
void RunInfo::Init(int aSize,
                   int aNbr,
                   int isHistory,
                   int aRseed,
                   int aTrialNr,
                   int aReportNr,
                   char *aFileName,
                   char *rFileName,
                   RepForm aForm,
                   CrossMethod *aCross)
{
    genSize = aSize;
    genNumber = aNbr;
    history = isHistory;
    rseed = aRseed;
    trialNr = aTrialNr;
    reportNr = aReportNr;
    hFileName = new char[strlen(aFileName) + 1];
    strcpy(hFileName, aFileName);
    resFileName = new char[strlen(rFileName) + 1];
    strcpy(resFileName, rFileName);
    aRForm = aForm;
    theCross = new CrossMethod(aCross);
}


/****************************** Input ******************************/

// Input all the information from the console
void RunInfo::ScanRunInfo()
{
    int dummy;

    cout << "population size" << endl;
    cin >> genSize;
    cout << "generation number" << endl;
    cin >> genNumber;
    cout << "history (0/1)" << endl;
    cin >> history;
    if (history) {
        cout << "history file" << endl;
        hFileName = new char[50];
        cin >> hFileName;
    }
    cout << "reproduction form" << endl
        << "0 = simple, 1 = elitist, 2 = monotonical," << endl
        << "3=sexuate, 4=sexBinary, 5=hermaphrodite" << endl;
    cin >> dummy;
    aRForm = RepForm(dummy);
    theCross = new CrossMethod();
    theCross->ScanMethod();
}

// Input a setting from the console
int RunInfo::OptionRunInfo(char anOption)
{
    int result = 1, dummy;

    switch (anOption) {
    case 'p':
    case 'P':
        cout << "population size" << endl;
        cin >> genSize;
        break;
    case 'g':
    case 'G':
        cout << "generation number" << endl;
        cin >> genNumber;
        break;
    case 'h':
    case 'H':
        cout << "history (0/1)" << endl;
        cin >> history;
        break;
    case 'f':
    case 'F':
        cout << "history file" << endl;
        hFileName = new char[50];
        cin >> hFileName;
        break;
    case 'r':
    case 'R':
        cout << "reproduction form" << endl
            << "0 = simple, 1 = elitist, 2 = monotonical," << endl
            << "3=sexuate, 4=sexBinary, 5=hermaphrodite" << endl;
        cin >> dummy;
        aRForm = RepForm(dummy);
        break;
    case 'c':
    case 'C':
        theCross->ScanMethod();
        break;
    default:
        result = 0;
    }
    return result;
}

// Output all the settings and ask if the user wants to change any of them.
int RunInfo::AskRunInfo()
{
    int result = 0, i;
    char answer[7];

    cout << "Running information:" << endl;
    Print();
    cout << "Do you want to chane anything?" << endl
        << "population size [p]\ngeneration number [g]\n"
        << "reproduction form [r]\ncrossover form [c]\n"
        << "history [h] history file [f] or nothing [0]" << endl;
    cin >> answer;
    for (i = 0; (i < 7) && (answer[i] != '\0'); i++)
        result += OptionRunInfo(answer[i]);
    return result;
}

/******************************* Output *****************************/

// Output the settings to the console
void RunInfo::Print()
{
    cout << "population size = " << genSize << endl
         << "generation number = " << genNumber << endl
         << "reproduction form = " << RepString() << endl
         << "history = " << history << endl
         << "random seed = " << rseed << endl
         << "number of trials = " << trialNr << endl
         << "number of reports = " << reportNr << endl;
    if ((history) && (hFileName != NULL))
        cout << "history file = " << hFileName << endl;
    cout << "result file = " << resFileName << endl;
    theCross->Print();
}

// Output the settings to a file
void RunInfo::FPrint(FILE *aFile)
{
    fprintf(aFile, "population size\n%d\n", genSize);
    fprintf(aFile, "generation number\n%d\n", genNumber);
    fprintf(aFile, "reproduction form\n%d\n", aRForm);
    fprintf(aFile, "history\n%d\n", history);
    fprintf(aFile, "random seed\n%d\n", rseed);
    fprintf(aFile, "number of trials\n%d\n", trialNr);
    fprintf(aFile, "number of reports\n%d\n", reportNr);
    fprintf(aFile, "history file\n");
    if (hFileName != NULL)
        fprintf(aFile, "%s\n", hFileName);
    else
        fprintf(aFile, "\n");
    fprintf(aFile, "result file\n%s\n", resFileName);
    theCross->FPrint(aFile);
}

// Return the reproduction form as a string
char *RunInfo::RepString()
{
    char *rez = new char[12];
    switch (aRForm) {
    case simple:
        strcpy(rez, "simple");
    case elitist:
        strcpy(rez, "elitist");
    case monotone:
        strcpy(rez, "monotonical");
    case sexuate:
        strcpy(rez, "sexuate");
    case sexBinary:
        strcpy(rez, "sexBinary");
    case herma:
        strcpy(rez, "herma");
    case diverse:
        strcpy(rez, "diverse");
    }
    return rez;
}


