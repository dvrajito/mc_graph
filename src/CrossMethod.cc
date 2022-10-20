/*********************************************************************

   Project: GADX, a C++ implementation of genetic algorithms
   License: Creative Commons, Attribution
   Author:  Dana Vrajitoru
   File:    CrossMethod.cc
   Update:  March 2019

   A class handling some of the crossover operator settings.

**********************************************************************/

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
using namespace std;
#include "CrossMethod.h"
#include "General.h"

//////////////////////////// constructors //////////////////////////////

// Default constructor
CrossMethod::CrossMethod()
{
    Init();
}

// Constructor reading the attributes from a file
CrossMethod::CrossMethod(FILE *aFile)
{
    Init(aFile);
}

// Constructor reading the attributes from a file
CrossMethod::CrossMethod(ifstream &fin)
{
    Init(fin);
}

// Constructor reading the attributes from a dictionary of seeting, value
CrossMethod::CrossMethod(char **aDict)
{
    Init(aDict);
}

// Copy constructor
CrossMethod::CrossMethod(CrossMethod *data)
{
    Init(data);
}

// Constructor with values for all attributes
CrossMethod::CrossMethod(CrossForm aCrossForm,
                         DissocForm aDissForm,
                         CombineForm aCombForm,
                         int nPoints,
                         double aProb)
{
    Init(aCrossForm, aDissForm, aCombForm, nPoints, aProb);
}

/* destructor */
CrossMethod::~CrossMethod()
{
    ;
}

/******************************* Initialize *****************************/

// Initialize everything with default values
void CrossMethod::Init()
{
    theCrossover = onePointCrs;
    dissForm = zeroOp;
    combForm = perfCmb;
    nPT = 1;
    thePswap = 0.5;
}

// Initialize from a file
void CrossMethod::Init(FILE *aFile)
{
    double readFl;

    SkipLine(aFile);
    SkipLine(aFile);
    fscanf(aFile, "%d", &(theCrossover));
    switch (theCrossover) {
    case nPointCrs:
        SkipLine(aFile);
        fscanf(aFile, "%d", &(nPT));
        break;
    case uniformCrs:
        SkipLine(aFile);
        fscanf(aFile, "%lf", &readFl);
        thePswap = readFl;
        break;
    case dissocCrs:
        SkipLine(aFile);
        fscanf(aFile, "%d", &(dissForm));
        break;
    case allCrs:
        SkipLine(aFile);
        fscanf(aFile, "%d", &(nPT));
        SkipLine(aFile);
        fscanf(aFile, "%lf", &readFl);
        thePswap = readFl;
        SkipLine(aFile);
        fscanf(aFile, "%d", &(dissForm));
    case combineCrs:
        SkipLine(aFile);
        fscanf(aFile, "%d", &(combForm));
    }
}

// Initialize from a file
void CrossMethod::Init(ifstream &fin)
{
    int c;

    SkipLine(fin);
    SkipLine(fin);
    fin >> c;
    theCrossover = (CrossForm)c;
    switch (theCrossover) {
    case nPointCrs:
        SkipLine(fin);
        fin >> nPT;
        break;
    case uniformCrs:
        SkipLine(fin);
        fin >> thePswap;
        break;
    case dissocCrs:
        SkipLine(fin);
        fin >> c;
        dissForm = (DissocForm)c;
        break;
    case allCrs:
        SkipLine(fin);
        fin >> nPT;
        SkipLine(fin);
        fin >> thePswap;
        SkipLine(fin);
        fin >> c;
        dissForm = (DissocForm)c;
    case combineCrs:
        SkipLine(fin);
        fin >> c;
        combForm = (CombineForm)c;
    }
}

// Initialize from a dictionary of (seeting, value)
void CrossMethod::Init(char **aDict)
{
    int i;

    for (i = 0; i < OPT_NR; i++) {
        if (strcmp(aDict[2 * i], "crossover form") == 0)
            theCrossover = CrossForm(atoi(aDict[2 * i + 1]));
        else if (strcmp(aDict[2 * i], "crossover sites") == 0)
            nPT = atoi(aDict[2 * i + 1]);
        else if (strcmp(aDict[2 * i], "swap probability") == 0)
            thePswap = atof(aDict[2 * i + 1]);
        else if (strcmp(aDict[2 * i], "diss form") == 0)
            dissForm = (DissocForm)atoi(aDict[2 * i + 1]);
        else if (strcmp(aDict[2 * i], "combined form") == 0)
            combForm = (CombineForm)atoi(aDict[2 * i + 1]);
    }
}

// Initialize everything as a copy from another object
void CrossMethod::Init(CrossMethod *data)
{
    theCrossover = data->theCrossover;
    dissForm = data->dissForm;
    combForm = data->combForm;
    nPT = data->nPT;
    thePswap = data->thePswap;
}

// Initialize with values for all attributes
void CrossMethod::Init(CrossForm aCrossForm,
                       DissocForm aDissForm,
                       CombineForm aCombForm,
                       int nPoints,
                       double aProb)
{
    theCrossover = aCrossForm;
    dissForm = aDissForm;
    combForm = aCombForm;
    nPT = nPoints;
    thePswap = aProb;
}

// Input from the console
void CrossMethod::ScanMethod()
{
    int dummy;

    cout << "crossover form" << endl
        << "1pt [0], npt [1], dissociated [2], uniform [3], all [4] :";
    cin >> dummy;
    theCrossover = CrossForm(dummy);
    switch (theCrossover) {
    case nPointCrs:
        cout << "crossover sites";
        cin >> nPT;
        break;
    case uniformCrs:
        cout << "swap probability";
        cin >> thePswap;
        break;
    case allCrs:
        cout << "crossover sites";
        cin >> nPT;
        cout << "swap probability";
        cin >> thePswap;
    case dissocCrs:
        cout << "diss form" << endl
            << "0 [0], and [1], xor [2], random [3]";
        cin >> dummy;
        dissForm = DissocForm(dummy);
    }
}

/******************************** Output ******************************/

// Output to the console
void CrossMethod::Print()
{
    cout << "crossover form = " << MethodString() << endl;
}

// Output to a file
void CrossMethod::FPrint(FILE *aFile)
{
    fprintf(aFile, "crossover form\n%d\n", theCrossover);
    switch (theCrossover)
    {
    case nPointCrs:
        fprintf(aFile, "crossover sites\n%d\n", nPT);
        break;
    case uniformCrs:
        fprintf(aFile, "swap probability\n%f\n", thePswap);
        break;
    case allCrs:
        fprintf(aFile, "crossover sites\n%d\n", nPT);
        fprintf(aFile, "swap probability\n%f\n", thePswap);
    case dissocCrs:
        fprintf(aFile, "diss form\n%d\n", dissForm);
    }
}

// Output to a file
void CrossMethod::FPrint(ofstream &fout)
{
    fout << "crossover form" << endl << theCrossover << endl;
    switch (theCrossover)
    {
    case nPointCrs:
        fout << "crossover sites" << endl << nPT << endl;
        break;
    case uniformCrs:
        fout << "swap probability" << endl << thePswap << endl;
        break;
    case allCrs:
        fout << "crossover sites" << endl << nPT << endl;
        fout << "swap probability" << endl << thePswap << endl;
    case dissocCrs:
        fout << "diss form" << endl << dissForm << endl;
    }
}

// Convert the crossover form to a string
char *CrossMethod::MethodString()
{
    char *result;

    result = new char[40];
    switch (theCrossover)
    {
    case onePointCrs:
        sprintf(result, "1-point\0");
        break;
    case nPointCrs:
        sprintf(result, "%1d-points\0", nPT);
        break;
    case dissocCrs:
        sprintf(result, "dissociated %s\0", DissFormStr());
        break;
    case uniformCrs:
        sprintf(result, "uniform %.2f\0", thePswap);
        break;
    case combineCrs:
        sprintf(result, "combined %s\0", CombFormStr());
        break;
    case allCrs:
        sprintf(result, "all 5\0");
    }
    return result;
}

// Convert the crossover combination mode to a string
char *CrossMethod::CombFormStr()
{
    char *result;

    result = new char[20];
    switch (combForm) {
    case perfCmb:
        sprintf(result, "performance-prop\0");
        break;
    case eqCmb:
        sprintf(result, "balanced\0");
        break;
    case RR50Cmb:
        sprintf(result, "round-robin 50\0");
        break;
    case RR100Cmb:
        sprintf(result, "round-robin 100\0");
        break;
    case adaptCmb:
        sprintf(result, "adaptive\0");
        break;
    case allCmb:
        sprintf(result, "all\0");
        break;
    }
    return result;
}

// Convert the for of the dissociated crossover to a string
char *CrossMethod::DissFormStr()
{
    char *result;

    result = new char[8];
    switch (dissForm) {
    case zeroOp:
        sprintf(result, "0\0");
        break;
    case andOp:
        sprintf(result, "and\0");
        break;
    case xorOp:
        sprintf(result, "xor\0");
        break;
    case randOp:
        sprintf(result, "random\0");
        break;
    case allOp:
        sprintf(result, "all\0");
        break;
    default:
        sprintf(result, "0\0");
        break;
    }
    return result;
}

// Genetic operator: change the crossover operator in a round robin way
// based on the generation number. 
void CrossMethod::RoundRobin(int aGen)
{
    static int rrStep = 0;
    if ((aGen == 0) && (theCrossover == combineCrs)) {
        if (combForm == RR50Cmb)
            rrStep = 50;
        else if (combForm == RR100Cmb)
            rrStep = 20;
        else
            rrStep = 0;
    }
    if ((rrStep != 0) && (aGen % rrStep == 0))
        theCrossover = CrossForm((aGen / rrStep) % combineCrs);
}
