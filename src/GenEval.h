/********************************************************************

   Project: MC-Graph, a C++ implementation of genetic algorithms
            for graph drawing and visualization
   License: Creative Commons, Attribution
   Author:  Dana Vrajitoru
   File:    GenEval.h
   Update:  October 2022

   Evaluation (fitness) functions for the genetic algorithm.

*********************************************************************/

#ifndef GenEval_h
#define GenEval_h

#include "Generation.h"
#include "EvalInfo.h"

// Evaluate an entire generation based on the settings in anEval.
void EvalGen(Generation *aGen, EvalInfo *anEval);

// Evaluate a chromosome based on the settings in anEval.
double EvalInd(Individual *anInd, EvalInfo *anEval, Individual *decoded);

// source is coded on 0-256, dest is Boolean
void DecodeInd(Individual *source, Individual *dest);

// source is Boolean, dest is coded on 0-256
void EncodeInd(Individual *source, Individual *dest);

#endif
