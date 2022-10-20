/********************************************************************

   Project: MC-Graph, a C++ implementation of genetic algorithms
            for graph drawing and visualization
   License: Creative Commons, Attribution
   Author:  Dana Vrajitoru
   File:    EvalReal.cc
   Update:  October 2022

   Evaluation (fitness) functions for the genetic algorithm with
   any kind of problems involving real numbers.

********************************************************************/

#include <cstdio>
#include <cmath>
#include "GenTypes.h"
#include "EvalReal.h"
#include "General.h" 

// arg1 to power arg2 
double power(double arg1, double arg2)
{
    double result;

    if (arg1 == 0)
        if (arg2 == 0)
            return 1;
        else
            return 0;
    else
        if (arg1 < 0)
            result = log(-arg1);
        else
            result = log(arg1);
    result *= arg2;
    result = exp(result);
    if ((arg1 < 0) && (((int)arg2) % 2 != 0))
        result = -result;
    return result;
}

/////////////////// Standard Function Set ///////////////////////
////////////////// By Whitley at al, 1996 ///////////////////////

// F1(x1, x2, ..., xk) = sum(xi^2), i=1, k, where k = nLim
double F1(double x[], int nLim)
{
    int i;
    double result = 0;

    for (i = 0; i < nLim; i++)
        result += pow(x[i], 2.0);
    return result;
}

// F2(x1, x2) = 100*(x1^2-x2)^2 + (1-x1)^2, where nLim should be = 2, not used
double F2(double x[], int nLim)
{
    double x1, x2, result;

    x1 = x[0];
    x2 = x[1];
    result = x1 * x1 - x2;
    result = 100 * pow(result, 2.0);
    result += pow(1 - x1, 2.0);
    return result;
}

// F3(x1, x2, ..., xk) = 25+sum(floor(xi)), i=1, k, where k = nLim
double F3(double x[], int nLim)
{
    int i;
    double result = 0;

    for (i = 0; i < nLim; i++)
        result += (int)(x[i]);
    return (25 + result);
}

// F4(x1, x2, ..., xk) = floor(sum(i*xi^4))+rand(0,1), i=1, k, where k = nLim
double F4(double x[], int nLim)
{
    int i;
    double result = 0;

    for (i = 0; i < nLim; i++)
        result += ((i + 1) * pow(x[i], 4.0));
    result = (int)result;
    result += (rand() % randPrecision) / double(randPrecision);
    return result;
}

// F5(x1, x2) = 1/sum(i+(x1-16*(i mod 5)+32)^6 + (x2-16*(i mod 5)+32)^6), i=0 to 24, 
// where nLim should be = 2, not used
double F5(double x[], int nLim)
{
    int i;
    double x1, x2, result = 0.002;

    static double a[2][5] = {
    {-32.0,-16.0,0.0,16.0,32.0},
    {-32.0,-16.0,0.0,16.0,32.0}
    };

    x1 = x[0];
    x2 = x[1];
    for (i = 0; i < 25; i++)
        result += 1 / (i + 1 + pow(x1 - a[0][i % 5], 6.0) +
                       pow(x2 - a[1][i % 5], 6.0));
    return 1 / result;
}

// F6(x1, x2, ..., xk) = 10k + sum(xi^2-10*cos(2 pi xi)), i=1, k, k=nLim
double F6(double x[], int nLim)
{
    int i;
    double result;

    result = nLim * 10;
    for (i = 0; i < nLim; i++)
        result += pow(x[i], 2.0) - 10 * cos(2 * M_PI * x[i]);
    return result;
}

// F7(x1, x2, ..., xk) = 2555 - sum(xi*sin(sqrt(|xi|))) i=1, k, k=nLim
// subtracted from 2555 because this is a minimizing problem.
double F7(double x[], int nLim)
{
    int i;
    double result = 2555;

    for (i = 0; i < nLim; i++)
        result -= x[i] * sin(sqrt(fabs(x[i])));
    return result;
}

// F8(x1, x2, ..., xk) = prod(-cos(xi/sqrt(i), i=1,k) + 1 + sum(xi^2 i=1,k)/4000, k=nLim
double F8(double x[], int nLim)
{
    int i;
    double result = 1;

    for (i = 0; i < nLim; i++)
        result *= -cos(x[i] / sqrt(i + 1));
    result++;
    for (i = 0; i < nLim; i++)
        result += x[i] * x[i] / 4000;
    return result;
}

// F9(x1, x2, ..., xk) = 0.5 + sin(sum(xi^2, i=1,k)^2 - 0.5)/(1 + .001*sum(xi^2))^2
double F9(double x[], int nLim)
{
    double result = 0.5, xi2 = 0;

    for (int i = 0; i < nLim; i++)
        xi2 += x[i] * x[i];
    result += (pow(sin(sqrt(xi2)), 2.0) - 0.5) /
        pow(1 + .001*xi2, 2.0);
    return result;
}

// F10(x1, x2, ..., xk) = sum(xi^2, i=1,k)^0.25 * (sin(50*sum(xi^2, i=1,k)^0.1)^2+1)
double F10(double x[], int nLim)
{
    double result, xi2 = 0;

    for (int i = 0; i < nLim; i++)
        xi2 += x[i] * x[i];
    result = pow(xi2, 0.25) *
        (pow(sin(50 * pow(xi2, 0.1)), 2.0) + 1);
    return result;
}

// DV1(x) = 100*(1+sin(x)). Function by D. Vrajitoru 
double DV1(double x)
{
    return 100 * (1 + sin(x));
}

double GraphLayout1(double x[], int nLim)
{
    double result = 0;
    return result;
}

// Redirection to one of the functions based on the function number fNr.
double ChoseFunct(int fNr, double x[], int nLim)
{
    switch (fNr) {
    case 1: return F1(x, nLim);
    case 2: return F2(x, nLim);
    case 3: return F3(x, nLim);
    case 4: return F4(x, nLim);
    case 5: return F5(x, nLim);
    case 6: return F6(x, nLim);
    case 7: return F7(x, nLim);
    case 8: return F8(x, nLim);
    case 9: return F9(x, nLim);
    case 10: return F10(x, nLim);
    case 11: return DV1(x[0]);
    case 15: return GraphLayout1(x, nLim);
    }
    return 0;
}

// Returns a value larger than the maximum value for each of the functions.
double MaxValFunct(int fNr)
{
    switch (fNr) {
    case 1: return 80;
    case 2: return 3000;
    case 3: return 55;
    case 4: return 1300;
    case 5: return 501;
    case 6: return 210;
    case 7: return 5000;
    case 8: return 330;
    case 9: return 2;
    case 10: return 25;
    case 11: return 0;
    }
    return 0;
}

// Returns a value smaller than the minimum value for each of the functions.
double MinValFunct(int fNr)
{
    switch (fNr) {
    case 1: return 15;
    case 2: return 478;
    case 3: return 10;
    case 4: return 930;
    case 5: return 1;
    case 6: return 60;
    case 7: return 1546;
    case 8: return 84.5;
    case 9: return 1;
    case 10: return 2.4;
    case 11: return 210;
    }
    return 0;
}

// Transformation 1 summing the values of fNr(x[i]) for i from 0 to nLim-1
double Transf1(int fNr, double x[], int nLim)
{
    double result = 0;
    int i;

    for (i = 0; i < nLim; i++)
        result += ChoseFunct(fNr, x + i, 1);
    return result;
}

// Transformation 2 adding fNr(xi, x(i+1)mod 3) for i from 0 to nLim-1
double Transf2(int fNr, double x[], int nLim)
{
    double result = 0, x1[2];
    int i;

    for (i = 0; i < nLim; i++)
    {
        x1[0] = x[i];
        x1[1] = x[(i + 1) % 3];
        result += ChoseFunct(fNr, x1, 2);
    }
    return result;
}

// Transformation 3 returning a large enough value minus fNr(xi). 
// Turns a minimization problem into a maximization one and the other way around.
double Transf3(int fNr, double x[], int nLim)
{
    return MaxValFunct(fNr) - ChoseFunct(fNr, x, nLim);
}

// Transformation 4 that returns a large enough value minux fNr(xi) and also 
// subtracts from it a calibrating value depending on the minimal value for the function.
// Turns a minimization problem into a maximization one and the other way around.
double Transf4(int fNr, double x[], int nLim)
{
    double minRate = 0.1;
    return ((MaxValFunct(fNr) - ChoseFunct(fNr, x, nLim) - MinValFunct(fNr))*
        (1 - minRate) / (MaxValFunct(fNr) - MinValFunct(fNr)) + minRate);
}

// Redirection function based on the transformation number and function number
double EvalTrans(int trNr, int fNr, double x[], int nLim)
{
    switch (trNr)
    {
    case 0: return ChoseFunct(fNr, x, nLim);
    case 1: return Transf1(fNr, x, nLim);
    case 2: return Transf2(fNr, x, nLim);
    case 3: return Transf3(fNr, x, nLim);
    case 4: return Transf4(fNr, x, nLim);
    }
    return 0;
}

// Converts a chromosome into a real value between the lower and upper limits a, b.
// x = a + sum(c[i]*(b-a)/2^(i+1), i=0,length)
double IndToReal(Individual *anInd)
{
    double a, b, result;
    int i;

    a = anInd->indInfo->infLim;
    b = anInd->indInfo->supLim;
    result = a;
    for (i = 0; i < anInd->indInfo->indSize; i++)
    {
        if (anInd->chromosome[i])
            result = result + (b - a) / 2;
        b = a + (b - a) / 2;
    }
    return result;
}

// Converts part of a chromosome to a real number.
double PartIndToReal(Individual *anInd, int inf, int sup)
{
    double a, b, result;
    int i;

    a = anInd->indInfo->infLim;
    b = anInd->indInfo->supLim;
    result = a;
    for (i = inf; i < sup; i++)
    {
        if (anInd->chromosome[i])
            result = result + (b - a) / 2;
        b = a + (b - a) / 2;
    }
    return result;
}

// Converts a chromosome to an array of real numbers.
// Result stored in realVals. nLim is the number of variables.
// limits shows the partitionning of the chromosome into variables.
// It contains the last position in the chromosome for each variable.
void IndToReals(Individual *anInd, double realVals[], int limits[], int nLim)
{
    if (nLim == 1)
        realVals[0] = IndToReal(anInd);
    else
    {
        realVals[0] = PartIndToReal(anInd, 0, limits[0]);
        realVals[nLim - 1] = PartIndToReal(anInd,
                                           limits[nLim - 2],
                                           anInd->indInfo->indSize);
        for (int i = 1; i < nLim - 1; i++)
            realVals[i] = PartIndToReal(anInd, limits[i - 1] + 1, limits[i]);
    }
}

// Converts the chromosome to an array of real numbers and then
// calls the appropriate fitness function and transformation to evaluate them.
double EvalReal(Individual *anInd, EvalInfo *anEval)
{
    static double *reals = NULL;
    static int *limits = NULL;
    static int oldLim = 0;
    double result;
    int i, n, nLim;

    n = anInd->indInfo->indSize;
    nLim = anEval->pointNumber;
    if (oldLim != nLim) {
        if (reals) {
            delete[] reals;
            reals = NULL;
        }
        if (limits) {
            delete[] limits;
            limits = NULL;
        }
    }
    oldLim = nLim;
    if (!reals)
        reals = new double[nLim];
    if (nLim > 1 && !limits) {
        limits = new int[nLim - 1];

        for (i = 1; i < nLim; i++)
            limits[i - 1] = i * (n / nLim);
    }
    IndToReals(anInd, reals, limits, nLim);
    result = EvalTrans(anEval->transNumber,   
                       anEval->funcNumber,
                       reals, nLim);
    return result;
}


