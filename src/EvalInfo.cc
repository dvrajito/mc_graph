/********************************************************************

   Project: MC-Graph, a C++ implementation of genetic algorithms
            for graph drawing and visualization
   License: Creative Commons, Attribution
   Author:  Dana Vrajitoru
   File:    EvalInfo.cc
   Update:  October 2022

   A class handling the evaluation settings, such as if
   we have Boolean or real-valued genes, the specific
   fitness function and transformation used, and the number of
   variables encoded in a chromosome.

*********************************************************************/

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
using namespace std;
#include "EvalInfo.h"
#include "General.h"

//////////////////////////// constructors //////////////////////////////

// Default constructor
EvalInfo::EvalInfo()
{
    Init();
}

// Constructor from a file
EvalInfo::EvalInfo(FILE *aFile)
{
    Init(aFile);
}

// Constructor from a file
EvalInfo::EvalInfo(ifstream &aFile)
{
    Init(aFile);
}

// Constructor from a dictionary of (setting, value)
EvalInfo::EvalInfo(char **aDict)
{
    Init(aDict);
}

// Copy constructor
EvalInfo::EvalInfo(EvalInfo *data)
{
    Init(data);
}

// Constructor with values for all attributes
EvalInfo::EvalInfo(int isReal,
                   int fNbr,
                   int tNbr,
                   int pNbr,
                   char *aName)
{
    Init(isReal, fNbr, tNbr, pNbr, aName);
}

// desctructor 
EvalInfo::~EvalInfo()
{
    delete probFileName;
}

/******************************* Initialize *****************************/

// Initialize everything with default values
void EvalInfo::Init()
{
    Init(0, 0, 0, 1, NULL);
}

// Initialize from a file
void EvalInfo::Init(FILE *aFile)
{
    Init();
    if (aFile == NULL)
        ScanEvalInfo();
    else {
        SkipLine(aFile);
        fscanf(aFile, "%d", &(realEval));
        SkipLine(aFile);
        fscanf(aFile, "%d", &(funcNumber));
        if (realEval)
        {
            SkipLine(aFile);
            fscanf(aFile, "%d", &(transNumber));
            SkipLine(aFile);
            fscanf(aFile, "%d", &(pointNumber));
        }
        else if (funcNumber >= 10)
        {
            SkipLine(aFile);
            fscanf(aFile, "%s", probFileName);
            SkipLine(aFile);
            fscanf(aFile, "%d", &(pointNumber));
        }
    }
}

// Initialize from a file
void EvalInfo::Init(ifstream &aFile)
{
    Init();
    if (!aFile.good())
        ScanEvalInfo();
    else {
        SkipLine(aFile);
        aFile >> realEval;
        SkipLine(aFile);
        aFile >> funcNumber;
        if (realEval)
        {
            SkipLine(aFile);
            aFile >> transNumber;
            SkipLine(aFile);
            aFile >> pointNumber;
        }
        else if (funcNumber >= 10)
        {
            SkipLine(aFile);
            aFile >> probFileName;
            SkipLine(aFile);
            aFile >> pointNumber;
        }
    }
}

// Initialize from a dictionary of (setting, value)
void EvalInfo::Init(char **aDict)
{
    int isReal, fNbr, tNbr, pNbr, i;
    int aName;

    for (i = 0; i < OPT_NR; i++)
        if (strcmp(aDict[2 * i], "evaluation form") == 0)
            isReal = atoi(aDict[2 * i + 1]);
        else if (strcmp(aDict[2 * i], "function number") == 0)
            fNbr = atoi(aDict[2 * i + 1]);
        else if (strcmp(aDict[2 * i], "transformation number") == 0)
            tNbr = atoi(aDict[2 * i + 1]);
        else if (strcmp(aDict[2 * i], "var number") == 0)
            pNbr = atoi(aDict[2 * i + 1]);
        else if (strcmp(aDict[2 * i], "problem file") == 0)
            aName = 2 * i + 1;

    Init(isReal, fNbr, tNbr, pNbr, aDict[aName]);
}

// Initialize everything as a copy from another object
void EvalInfo::Init(EvalInfo *data)
{
    Init(data->realEval, data->funcNumber, data->transNumber,
         data->pointNumber, data->probFileName);
}

// Initialize with values for all attributes
void EvalInfo::Init(int isReal,
                    int fNbr,
                    int tNbr,
                    int pNbr,
                    char *aName)
{
    realEval = isReal;
    funcNumber = fNbr;
    transNumber = tNbr;
    pointNumber = pNbr;
    if ((aName == NULL) || (strlen(aName) == 0) ||
        (aName[0] == ' '))
        probFileName = NULL;
    else {
        probFileName = new char[strlen(aName) + 1];
        strcpy(probFileName, aName);
    }
}

/********************************* Input ******************************/

// Input from the console
void EvalInfo::ScanEvalInfo()
{
    cout << "evaluation form [0 = bool], [1 = real]" << endl;
    cin >> realEval;
    cout << "function number" << endl;
    cin >> funcNumber;
    if (realEval) {
        cout << "transformation number" << endl;
        cin >> transNumber;
        cout << "var number" << endl;
        cin >> pointNumber;
    }
    else if (funcNumber >= 10) {
        cout << "problem file" << endl;
        probFileName = new char[50];
        cin >> probFileName;
        cout << "p" << endl;
        cin >> pointNumber;
    }
}

// Input an option from the console
int EvalInfo::OptionEvalInfo(char anOption)
{
    int result = 1;

    switch (anOption) {
    case 'e':
    case 'E':
        cout << "evaluation form [0 = bool], [1 = real]" << endl;
        cin >> realEval;
        break;
    case 'f':
    case 'F':
        cout << "function number" << endl;
        cin >> funcNumber;
        break;
    case 't':
    case 'T':
        cout << "transformation number" << endl;
        cin >> transNumber;
        break;
    case 'v':
    case 'V':
        cout << "var number" << endl;
        cin >> pointNumber;
        break;
    case 'n':
    case 'N':
        cout << "problem file" << endl;
        probFileName = new char[50];
        cin >> probFileName;
        break;
    default:
        result = 0;
    }
    return result;
}

// Input one attribute from the console
int EvalInfo::AskEvalInfo()
{
    int result = 0, i;
    char answer[6];

    cout << "Evaluation information:" << endl;
    Print();
    cout << "Do you want to chane anything?" << endl
        << "evaluation form [e], function number [f]\n"
        << "transformation number [t], var number [v]\n"
        << "problem file [n] or nothing [0]\n";
    cin >> answer;
    for (i = 0; answer[i] != '\0'; i++)
        result += OptionEvalInfo(answer[i]);
    return result;
}

/******************************** Output ******************************/

// Output to the console
void EvalInfo::Print()
{
    if (realEval)
        cout << "Real evaluation\n";
    else
        cout << "Boolean evaluation\n";
    cout << "function number = " << funcNumber << endl;
    if (realEval)
        cout << "transformation number = " << transNumber << endl
        << "var number = " << pointNumber << endl;
    else if (funcNumber >= 10)
        cout << "problem file = " << probFileName << endl
        << "p = " << pointNumber << endl;
}

// Output to a file
void EvalInfo::FPrint(FILE *aFile)
{
    fprintf(aFile, "evaluation form\n%d\n", realEval);
    fprintf(aFile, "function number\n%d\n", funcNumber);
    if (realEval)
        fprintf(aFile, "transformation number\n%d\n", transNumber);
    else if (funcNumber > 10)
        fprintf(aFile, "problem file\n%s\n", probFileName);
    fprintf(aFile, "var number\n%d\n", pointNumber);
}

// Output to a file
void EvalInfo::FPrint(ofstream &fout)
{
    fout << "evaluation form" << endl << realEval << endl
     << "function number" << endl << funcNumber << endl;
    if (realEval)
        fout << "transformation number" << endl << transNumber << endl;
    else if (funcNumber > 10)
        fout << "problem file" << endl << probFileName << endl;
    fout << "var number" << endl << pointNumber << endl;
}

