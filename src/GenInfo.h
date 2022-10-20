/*************************************************************************

   Project: MC-Graph, a C++ implementation of genetic algorithms
            for graph drawing and visualization
   License: Creative Commons, Attribution
   Author:  Dana Vrajitoru
   File:    GenInfo.h
   Update:  October 2022

   A class handling the genetic settings, such as representation
   type, choice of genetic operations and parameters.

*************************************************************************/

#ifndef GenInfo_h
#define GenInfo_h

#include <cstdio>
#include <cstdlib>
#include <fstream>
using namespace std;

class GenInfo
{
public:
    double infLim, supLim; // Lower and upper bounds for the real values
                     // in case the chromosome translates to real values
    double probCross, probMut; // probability of crossover and mutation
    int indSize;
    short encode;

    /**************************** constructors ****************************/

    // Default constructor
    GenInfo();

    // Constructor with values for the lower limit for the values, 
    // upper limit for the values, probability of crossover, 
    // probability of mutation, chromosome size, encoded or not
    GenInfo(double inferior,
            double superior,
            double pcr,
            double pmt,
            int aSize,
            short enc);

    // Copy constructor 
    GenInfo(GenInfo *anInfo);
    
    // Constructor from a file
    GenInfo(FILE *aFile);

    // Constructor from a file
    GenInfo(ifstream &aFile);
    
    // Constructor from a dictionary of (setting, value)
    GenInfo(char **aDict);

    /******************************* Initialize *****************************/

    // Initialize everything with default values
    void Init();

    // Inferior limit for the values, superior limit for the values,
    // probability of crossover, probability of mutation, 
    // chromosome size, encoded or not
    void Init(double inferior,
              double superior,
              double pcr,
              double pmt,
              int aSize,
              short enc);

    // Init the object from a file
    void Init(FILE *aFile);

    // Init the object from a file
    void Init(ifstream &fin);

    // Extract the genetic information from a dictionary containing strings
    // for descriptions followed by value
    void Init(char **aDict);

    /********************************* Input *********************************/

    // Read the settings from the console 
    void ScanInfo();

    // Input one setting at a time from the console
    int OptionInfo(char anOption);

    // Output the settings and ask the user for any changes to make.
    int AskInfo();

    /********************************* Output *********************************/
    // print the info to the console
    void Print();

    // print the info to a file
    void FPrint(FILE *aFile);

    // print the info to a file
    void FPrint(ofstream &fin);
};

#endif
