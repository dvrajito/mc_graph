/********************************************************************

   Project: MC-Graph, a C++ implementation of genetic algorithms
            for graph drawing and visualization
   License: Creative Commons, Attribution
   Author:  Dana Vrajitoru
   File:    EvalReal.h
   Update:  October 2022

   Evaluation (fitness) functions for the genetic algorithm with
   any kind of problems involving real numbers.

*********************************************************************/

#ifndef EvalReal_h
#define EvalReal_h

#include "Individual.h"
#include "EvalInfo.h"

// Converts a chromosome into a real value between the lower and upper
// limits a, b.
// x = a + sum(c[i]*(b-a)/2^(i+1), i=0,length)
double IndToReal(Individual *anInd);

// Converts a chromosome to an array of real numbers.
// Result stored in realVals. nLim is the number of variables.
// limits shows the partitionning of the chromosome into variables.
// It contains the last position in the chromosome for each variable.
void IndToReals(Individual *anInd,
                double realVals[],
                int limits[],
                int nLim);

// Converts the chromosome to an array of real numbers and then
// calls the appropriate fitness function and transformation to
// evaluate them.
double EvalReal(Individual *anInd, EvalInfo *anEval);

///////////////////// Fitness functions /////////////////////////

/////////////////// Standard Function Set ///////////////////////
////////////////// By Whitley at al, 1996 ///////////////////////

// F1(x1, x2, ..., xk) = sum(xi^2), i=1, k, where k = nLim
double F1(double x[], int nLim);

// F2(x1, x2) = 100*(x1^2-x2)^2 + (1-x1)^2, where nLim should be = 2,
// not used
double F2(double x[], int nLim);

// F3(x1, x2, ..., xk) = 25+sum(floor(xi)), i=1, k, where k = nLim
double F3(double x[], int nLim);

// F4(x1, x2, ..., xk) = floor(sum(i*xi^4))+rand(0,1), i=1, k, where k = nLim
double F4(double x[], int nLim);

// F5(x1, x2) = 1/sum(i+(x1-16*(i mod 5)+32)^6 + 
// (x2-16*(i mod 5)+32)^6), i=0 to 24,
// where nLim should be = 2, not used
double F5(double x[], int nLim);

// F6(x1, x2, ..., xk) = 10k + sum(xi^2-10*cos(2 pi xi)), i=1, k, k=nLim
double F6(double x[], int nLim);

// F7(x1, x2, ..., xk) = 2555 - sum(xi*sin(sqrt(|xi|))) i=1, k, k=nLim
// subtracted from 2555 because this is a minimizing problem.
double F7(double x[], int nLim);

// F8(x1, x2, ..., xk) = prod(-cos(xi/sqrt(i), i=1,k) + 1 + 
// sum(xi^2 i=1,k)/4000, k=nLim
double F8(double x[], int nLim);

// F9(x1, x2, ..., xk) = 0.5 + sin(sum(xi^2, i=1,k)^2 - 0.5)/
// (1 + .001*sum(xi^2))^2
double F9(double x[], int nLim);

// F10(x1, x2, ..., xk) = sum(xi^2, i=1,k)^0.25 * 
// (sin(50*sum(xi^2, i=1,k)^0.1)^2+1)
double F10(double x[], int nLim);

// DV1(x) = 100*(1+sin(x)). Function by D. Vrajitoru 
double DV1(double x);

// Redirection to one of the functions based on the function number fNr.
double ChoseFunct(int fNr, double x[], int nLim);

// Returns a value larger than the maximum value for each of the functions.
double MaxValFunct(int fNr);

// Returns a value smaller than the minimum value for each of the functions.
double MinValFunct(int fNr);

//////////////////// Transformations /////////////////////////////

// Transformation 1 summing the values of fNr(x[i]) for i from 0 to nLim-1
double Transf1(int fNr, double x[], int nLim);

// Transformation 2 adding fNr(xi, x(i+1)mod 3) for i from 0 to nLim-1
double Transf2(int fNr, double x[], int nLim);

// Transformation 3 returning a large enough value minus fNr(xi). 
// Turns a minimization problem into a maximization one and the other
// way around.
double Transf3(int fNr, double x[], int nLim);

// Transformation 4 that returns a large enough value minux fNr(xi) and also 
// subtracts from it a calibrating value depending on the minimal
// value for the function. Turns a minimization problem into a
// maximization one and the other way around.
double Transf4(int fNr, double x[], int nLim);

// Redirection function based on the transformation number and function number
double EvalTrans(int trNr, int fNr, double x[], int nLim);

#endif
