/******************************************************************************

   Project: MC-Graph, a C++ implementation of genetic algorithms
            for graph drawing and visualization
   License: Creative Commons, Attribution
   Author:  Dana Vrajitoru
   File:    General.h
   Updated: October 2022
 
   General purpose functions and constants.

*******************************************************************************/

#ifndef General_h
#define General_h

#include <cstdio>
#include <cstdlib>
#include <fstream>
using namespace std;

//#define _CRT_SECURE_NO_WARNINGS // Add to the project properties - Preprocessor

const int  OPT_NR = 25;         // number of settings in the file
const int randPrecision = 1000; // for the RealRand function
const int randInc = 25;         // incrementing the random seed between trials

const float matingPercent = 0.25; // Percentage of the population to choose a mate 
                                  // from if the reproduction is herma or diverse.
                                  // Used when the reproduction radius is missing.

const int matingRadius = 0;    // Radius around a parent to find a mate if the
                               // reproduction is herma or diverse.

const int matingTries = 25;    // Number of attempts to find a mate based on the
                               // reproduction scheme. If unsuccessful, a sex
                               // change is made to make the mating possible.

const float matingPopPercent = 0.75; // Maximum percentage of the population that a
                                     // parent attempts to mate with based on the 
                                     // reproduction scheme. If unsuccesful, a sex
                                     // change is made to make the mating possible.

const int TRY_NR = 5;  // Number of potential mates to consider if the
                       // reproduction form is diverse, meaning that we're 
                       // looking for an individual with different genetic material.
                       // The most different mate out of the 5 is chosen.

const double M_PI = 3.14159265359; // Comment out on Linux.

#define FILE_ROOT "C:/Users/Dana/Documents/GitHub/GADX"
#define statFile FILE_ROOT"/Results/gender_stat.txt"
#define settingFile FILE_ROOT"/GAPrefs.txt"

// Skip a line from a file
void SkipLine(FILE *aFile);

// Skip a line from a file
void SkipLine(ifstream &fin);

// Looks for a '#' in the string. If it finds one, 
// it deletes everything following it.
// Also deletes trailing spaces.
void DeleteComment(char *line);

// A random number between 0 and 1 with a given precision
double RealRand(int precision=0.0001);

// clamp a value to a lower and upper bound
double clamp(double a, double low, double high);

#endif
