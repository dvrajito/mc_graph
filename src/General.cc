/********************************************************************

   Project: MC-Graph, a C++ implementation of genetic algorithms
            for graph drawing and visualization
   License: Creative Commons, Attribution
   Author:  Dana Vrajitoru
   File:    General.cc
   Updated: October 2022

   General purpose functions

*********************************************************************/

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "General.h"

// Skip a line from a file
void SkipLine(FILE *aFile)
{
    char c;

    c = fgetc(aFile); // skip the current new line
    do
        c = fgetc(aFile);
    while (c != '\n');
}

// Skip a line from a file
void SkipLine(ifstream &fin)
{
    char c;

    c = fin.get(); // skip the current new line
    do
        c = fin.get();
    while (c != '\n');
}

// Looks for a '#' in the string. If it finds one, 
// it deletes everything following it.
// Also deletes trailing spaces.
void DeleteComment(char *line)
{
    int i = 0;

    for (i=0; line[i] != '\0'; i++)
        if (line[i] == '#') {
            line[i] = '\0';
            line[i + 1] = '\0'; // so that the for loop stops
        }
    for (i--; i >= 0 && (line[i] == '\0' || isspace(line[i])); i--)
        line[i] = '\0';
}

// A random number between 0 and 1 with a given precision
double RealRand(int precision)
{
    return (double)(rand() % precision) / precision;
}

// clamp a value to a lower and upper bound
double clamp(double a, double low, double high)
{
    if (a < low)
        return low;
    else if (a > high)
        return high;
    else
        return a;
}
