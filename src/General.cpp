/***************************************************************

   Project: GAD, a custom implementation of genetic algorithms.
   Author:  Dana Vrajitoru
   File:    General.cc
   Update:  May 26, 2004.
   
   General purpose useful functions and constants.
  
***************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "General.h"

void SkipLine(FILE *aFile)
{ char c;
  
  c = fgetc(aFile);
  do
    c = fgetc(aFile);
  while (c != '\n');
}

double RealRand(int precision, double start, double end)
{
  return (double)(start + (end-start)*(rand() % precision) / precision);
}
