/********************************************************************

   Project: MC-Graph, a C++ implementation of genetic algorithms
            for graph drawing and visualization
   License: Creative Commons, Attribution
   Author:  Dana Vrajitoru
   File:    GenEval.cc
   Update:  October 2022

   Evaluation (fitness) functions for the genetic algorithm.

*********************************************************************/

#include <cstdio>
#include <cstdlib>
#include <iostream>
using namespace std;
#include "GenTypes.h"
#include "EvalReal.h"
#include "EvalBool.h"
#include "GenEval.h"
#include <cmath>

// Evaluate a chromosome based on the settings in anEval.
double EvalInd(Individual *anInd, EvalInfo *anEval, Individual *decoded)
{
    if (anInd->indInfo->encode) {
        if ((decoded == NULL) ||
            (decoded->indInfo->indSize < anInd->indInfo->indSize))
            decoded = new Individual(anInd, 1);
        DecodeInd(anInd, decoded);
        if (anEval->realEval)
            return EvalReal(decoded, anEval);
        else
            return EvalBoolean(decoded, anEval);
    }
    else
        if (anEval->realEval)
            return EvalReal(anInd, anEval);
        else
            return EvalBoolean(anInd, anEval);
}

// source is coded on 0-256, dest is Boolean
void DecodeInd(Individual *source, Individual *dest)
{
    int ssize = source->GetSize(), i, j;
    short quot;
    for (i = 0; i < ssize; i++) {
        quot = source->chromosome[i];
        for (j = 0; j < 8; j++) {
            dest->chromosome[8 * (i + 1) - j - 1] = quot % 2;
            quot = quot / 2;
        }
    }
}

// source is Boolean, dest is coded on 0-256
void EncodeInd(Individual *source, Individual *dest)
{
    int dsize = dest->GetSize(), i, j;
    short code;
    for (i = 0; i < dsize; i++) {
        code = 0; //
        for (j = 0; j < 8; j++)
            code = code * 2 + source->chromosome[8 * i + j];
        dest->chromosome[i] = code;
    }
}

// Evaluate an entire generation based on the settings in anEval.
void EvalGen(Generation *aGen, EvalInfo *anEval)
{
    short i;

    for (i = 0; i < aGen->popSize; i++)
        aGen->fitValues[i] = EvalInd(aGen->population[i], anEval, aGen->decoded1);
}


