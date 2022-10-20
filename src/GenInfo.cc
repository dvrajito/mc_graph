/*************************************************************************

   Project: MC-Graph, a C++ implementation of genetic algorithms
            for graph drawing and visualization
   License: Creative Commons, Attribution
   Author:  Dana Vrajitoru
   File:    GenInfo.cc
   Update:  October 2022

   A class handling the genetic settings, such as representation
   type, choice of genetic operations and parameters.

*************************************************************************/

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
using namespace std;
#include "GenInfo.h"
#include "General.h"

/************************* constructors **********************************/

// Default constructor
GenInfo::GenInfo()
{
    Init();
}

// Constructor with values for the lower limit for the values, 
// upper limit for the values, probability of crossover, 
// probability of mutation, chromosome size, encoded or not
GenInfo::GenInfo(double inferior, double superior,
                 double pcr, double pmt, int aSize, short enc)
{
    Init(inferior, superior, pcr, pmt, aSize, enc);
}

// Copy constructor 
GenInfo::GenInfo(GenInfo *anInfo)
{
    Init(anInfo->infLim,
         anInfo->supLim,
         anInfo->probCross,
         anInfo->probMut,
         anInfo->indSize,
         anInfo->encode);
}

// Constructor from a file
GenInfo::GenInfo(FILE *aFile)
{
    Init(aFile);
}

// Constructor from a file
GenInfo::GenInfo(ifstream &aFile)
{
    Init(aFile);
}

// Constructor from a dictionary of (setting, value)
GenInfo::GenInfo(char **aDict)
{
    Init(aDict);
}

/******************************* Initialize ******************************/

// Initialize everything with default values
void GenInfo::Init()
{
    Init(0.0, 0.0, 0.0, 0.0, 0, 0);
}

// Inferior limit for the values, superior limit for the values,
// probability of crossover, probability of mutation, 
// chromosome size, encoded or not
void GenInfo::Init(double inferior,
                   double superior,
                   double pcr,
                   double pmt,
                   int aSize,
                   short enc)
{
    infLim = inferior;
    supLim = superior;
    probCross = pcr;
    probMut = pmt;
    indSize = aSize;
    encode = enc;
}

// Init the object from a file
void GenInfo::Init(FILE *aFile)
{
    double a;

    if (aFile == NULL)
        ScanInfo();
    else
    {
        SkipLine(aFile);
        fscanf(aFile, "%lg", &a);
        infLim = a;
        SkipLine(aFile);
        fscanf(aFile, "%lg", &a);
        supLim = a;
        SkipLine(aFile);
        fscanf(aFile, "%lg", &a);
        probCross = a;
        SkipLine(aFile);
        fscanf(aFile, "%lg", &a);
        probMut = a;
        SkipLine(aFile);
        fscanf(aFile, "%d", &indSize);
        SkipLine(aFile);
        fscanf(aFile, "%hd", &encode);
    }
}

// Init the object from a file
void GenInfo::Init(ifstream &fin)
{
    double a;

    if (!fin.good())
        ScanInfo();
    else
    {
        SkipLine(fin);
        fin >> a;
        infLim = a;
        SkipLine(fin);
        fin >> a;
        supLim = a;
        SkipLine(fin);
        fin >> a;
        probCross = a;
        SkipLine(fin);
        fin >> a;
        probMut = a;
        SkipLine(fin);
        fin >> indSize;
        SkipLine(fin);
        fin >> encode;
    }
}

// Extract the genetic information from a dictionary containing strings
// for descriptions followed by value
void GenInfo::Init(char **aDict)
{
    for (int i = 0; i < OPT_NR; i++)
        if (strcmp(aDict[2 * i], "superior limit") == 0)
            supLim = atof(aDict[2 * i + 1]);
        else if (strcmp(aDict[2 * i], "inferior limit") == 0)
            infLim = atof(aDict[2 * i + 1]);
        else if (strcmp(aDict[2 * i], "probability of crossover") == 0)
            probCross = atof(aDict[2 * i + 1]);
        else if (strcmp(aDict[2 * i], "probability of mutation") == 0)
            probMut = atof(aDict[2 * i + 1]);
        else if (strcmp(aDict[2 * i], "individual size") == 0)
            indSize = atoi(aDict[2 * i + 1]);
        else if (strcmp(aDict[2 * i], "encode") == 0)
            encode = atoi(aDict[2 * i + 1]);
}

/********************************* Input *********************************/

// Input the info from the console
void GenInfo::ScanInfo()
{
    cout << "inferior limit" << endl;
    cin >> infLim;
    cout << "superior limit" << endl;
    cin >> supLim;
    cout << "probability of crossover" << endl;
    cin >> probCross;
    cout << "probability of mutation" << endl;
    cin >> probMut;
    cout << "individual size" << endl;
    cin >> indSize;
    cout << "encode" << endl;
    cin >> encode;
}

// Input one setting at a time from the console
int GenInfo::OptionInfo(char anOption)
{
    double a;
    int result = 1;

    switch (anOption)
    {
    case 'i':
    case 'I':
        printf("\ninferior limit\n");
        scanf("%lg", &a);
        infLim = a;
        break;
    case 's':
    case 'S':
        printf("\nsuperior limit\n");
        scanf("%lg", &a);
        supLim = a;
        break;
    case 'c':
    case 'C':
        printf("\nprobability of crossover\n");
        scanf("%lg", &a);
        probCross = a;
        break;
    case 'm':
    case 'M':
        printf("\nprobability of mutation\n");
        scanf("%lg", &a);
        probMut = a;
        break;
    case 't':
    case 'T':
        printf("\nindividual size\n");
        scanf("%d", &(indSize));
        break;
    case 'e':
    case 'E':
        printf("\nencode\n");
        scanf("%hd", &(encode));
        break;
    default:
        result = 0;
    }
    return result;
}

// Output the settings and ask the user for any changes to make.
int GenInfo::AskInfo()
{
    char answer[10];
    int i, result = 0;

    cout << "The initialized information" << endl;
    Print();
    cout << "Do you want to change anything ?" << endl
         << "Possible options:" << endl
         << "inferior limit[i], superior limit[s]" << endl
         << "probability of crossover[c], probability of mutation[m]" << endl
         << "individual size[t], encode[e], none[0]" << endl;
    cin >> answer;
    for (i = 0; answer[i] != '\0'; i++)
        result += OptionInfo(answer[i]);
    return result;
}

/********************************* Output ********************************/

// print the info to the console
void GenInfo::Print()
{
    cout << "inferior limit = " << infLim << endl
         << "superior limit = " << supLim << endl
         << "probability of crossover = " << probCross << endl
         << "probability of mutation = " << probMut << endl
         << "individual size = " << indSize << endl
         << "encode = " << encode << endl;
}

// print the info to a file
void GenInfo::FPrint(FILE *aFile)
{
    fprintf(aFile, "inferior limit\n%f\n", infLim);
    fprintf(aFile, "superior limit\n%f\n", supLim);
    fprintf(aFile, "probability of crossover\n%g\n", probCross);
    fprintf(aFile, "probability of mutation\n%g\n", probMut);
    fprintf(aFile, "individual size\n%d\n", indSize);
    fprintf(aFile, "encode\n%d\n", encode);
}

// print the info to a file
void GenInfo::FPrint(ofstream &fin)
{
    fin << "inferior limit" << endl << infLim << endl
        << "superior limit" << endl << supLim << endl
        << "probability of crossover" << endl << probCross << endl
        << "probability of mutation" << endl << probMut << endl
        << "individual size" << endl << indSize << endl
        << "encode" << endl << encode << endl;
}
