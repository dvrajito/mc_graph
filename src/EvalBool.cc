/********************************************************************

   Project: MC-Graph, a C++ implementation of genetic algorithms
            for graph drawing and visualization
   License: Creative Commons, Attribution
   Author:  Dana Vrajitoru
   File:    EvalBool.cc
   Update:  October 2022

   Evaluation (fitness) functions for the genetic algorithm with
   any kind of logical problem.

*********************************************************************/

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <iostream>
using namespace std;

#include "GenTypes.h"
#include "EvalBool.h"
#include "SATexpr.h"
#include "GAManager.h"

/************************ SAT *********************************/

int decep[8] = { 28, 26, 22, 0, 14, 0, 0, 30 };
int myDecep1[8] = { 22, 0, 28, 26, 0, 30, 14, 0 };
int myDecep2[8] = { 0, 14, 30, 0, 26, 28, 0, 22 };
int symba[8] = { 28, 26, 22, 14, 14, 26, 22, 30 };
int mySymba1[8] = { 22, 14, 28, 26, 22, 30, 14, 26 };
int mySymba2[8] = { 26, 14, 30, 22, 26, 28, 14, 22 };
int decep1[8] = { 22, 14, 28, 26, 14, 30, 24, 14 };
int decep2[8] = { 14, 22, 30, 14, 24, 28, 14, 26 };

// power of a real number if the power is positive, 0 otherwise
double MyPow(double aReal, double aPower)
{
    if (aReal > 0.0)
        return pow(aReal, aPower);
    else
        return 0.0;
}

// Read the global SAT expression from a file
void ReadSATexpr(char *fileName)
{
    GAManager::globalSAT = new SATexpr(fileName);
}

// Delete the global SAT expression on exit.
void deleteGlobalSAT()
{
    if (GAManager::globalSAT != NULL) {
        delete GAManager::globalSAT;
        GAManager::globalSAT = NULL;
    }
}

/////////////////////////// SAT Eval //////////////////////////////////////

// Evaluates a single term, which is part of a clause.
// Returns sign(aTerm)*value of the chromosome at index |aTerm|.
int EvalTerm(Individual *anInd, int aTerm)
{
    if (aTerm < 0)
        return 1 - anInd->chromosome[-aTerm];
    else
        return anInd->chromosome[aTerm];
}

///////////////////////// Evaluate a clause //////////////////////////////////

// Evaluates a clause by applying bitwise XOR to each term in the clause 
// with each gene in the individual and then taking the average of the results. 
double EvalClauseXorAve(Individual *anInd, SATclause *aClause)
{
    double clauseEval = 0;

    for (int trm = 0; trm < aClause->nrTerms; trm++)
        clauseEval += EvalTerm(anInd, aClause->terms[trm]);
    if (clauseEval > 0)
        return (((aClause->nrTerms) - clauseEval + 1) / (aClause->nrTerms));
    else
        return (double)(aClause->nrTerms - 1) / (aClause->nrTerms);
}

// Evaluates a clause by applying bitwise AND to each term in the clause 
// with each gene in the individual and then taking the average of the results. 
double EvalClauseAndAve(Individual *anInd, SATclause *aClause)
{
    double clauseEval = 0;

    for (int trm = 0; trm < aClause->nrTerms; trm++)
        clauseEval += EvalTerm(anInd, aClause->terms[trm]);
    return (double)(clauseEval / (aClause->nrTerms));
}

// Evaluates a clause by applying bitwise OR to each term in the clause 
// with each gene in the individual and then taking taking the or of the results.
double EvalClauseOrAve(Individual *anInd, SATclause *aClause)
{
    double clauseEval = 0, termEval;

    for (int trm = 0; trm < aClause->nrTerms; trm++)
    {
        termEval = EvalTerm(anInd, aClause->terms[trm]);
        if (termEval > clauseEval)
            clauseEval = termEval;
    }
    return (double)clauseEval;
}

// Returns (sum((clause[i] or chrom[i])^pNorm)^(1/pNorm)
double EvalClauseOrPN(Individual *anInd, SATclause *aClause, int pNorm)
{
    double clauseEval = 0, termEval;
    int trm;

    for (trm = 0; trm < aClause->nrTerms; trm++)
    {
        termEval = EvalTerm(anInd, aClause->terms[trm]);
        clauseEval += MyPow(termEval, (double)pNorm);
    }
    clauseEval = MyPow((clauseEval / aClause->nrTerms), (double)(1.0 / pNorm));
    return (double)clauseEval;
}

// Returns (sum((clause[i] and chrom[i])^pNorm)^(1/pNorm)
double EvalClauseAndPN(Individual *anInd, SATclause *aClause, int pNorm)
{
    double clauseEval = 0, termEval, p;
    double powRes;
    int trm;

    p = pNorm;
    for (trm = 0; trm < aClause->nrTerms; trm++)
    {
        termEval = 1 - EvalTerm(anInd, aClause->terms[trm]);
        powRes = MyPow(termEval, p);
        clauseEval += powRes;
    }
    p = 1 / p;
    clauseEval = MyPow((clauseEval / aClause->nrTerms), p);
    return (double)(1 - clauseEval);
}

// Returns (sum((clause[i] xor chrom[i])^pNorm)^(1/pNorm)
double EvalClauseXorPN(Individual *anInd, SATclause *aClause, int pNorm)
{
    double clauseEval = 0;
    int k = 0, n;

    n = aClause->nrTerms;
    for (int trm = 0; trm < n; trm++)
        k += EvalTerm(anInd, aClause->terms[trm]);
    clauseEval = k * MyPow(1 - MyPow((k - 1) / n, 
                           (double)(1.0 / pNorm)), (double)pNorm);
    clauseEval += (n - k) * MyPow(1 - MyPow((k + 1) / n,
                           (double)(1.0 / pNorm)), (double)pNorm);
    clauseEval = MyPow((clauseEval / aClause->nrTerms), (double)(1.0 / pNorm));
    return (double)clauseEval;
}

///////////////////////// Evaluate a SAT expression ////////////////////////////

// Evaluate a SAT expression by evaluating each of its clauses using XOR Average.
// The results are then averaged over all the clauses. 
double EvalSatHCave(Individual *anInd, SATexpr *expr)
{
    double result = 0;

    for (int cls = 0; cls < expr->nrClauses; cls++)
        result += EvalClauseXorAve(anInd, expr->clauses[cls]);
    result = result / (expr->nrClauses);
    return result;
}

// Evaluate a SAT expression by evaluating each of its clauses using OR Average.
// The results are then averaged over all the clauses. 
double EvalSATaveC(Individual *anInd, SATexpr *expr)
{
    double result = 0;

    for (int cls = 0; cls < expr->nrClauses; cls++)
        result += EvalClauseOrAve(anInd, expr->clauses[cls]);
    result = result / (expr->nrClauses);
    return result;
}

// Evaluate a SAT expression by evaluating each of its clauses using AND Average.
// The results are then averaged over all the clauses. 
double EvalSATaveD(Individual *anInd, SATexpr *expr)
{
    double result = 0, clauseEval;
    int cls;

    for (cls = 0; cls < expr->nrClauses; cls++)
    {
        clauseEval = EvalClauseAndAve(anInd, expr->clauses[cls]);
        if (clauseEval > result)
            result = clauseEval;
    }
    return result;
}

// Evaluate a SAT expression by evaluating each of its clauses using AND pNorm.
// The pNorm formulaa is also applied here: a sum of the result from each clause 
// to power pNorm, then the sum to power 1/pNorm.
double EvalSATpnormD(Individual *anInd, SATexpr *expr, int pNorm)
{
    double result = 0, clauseEval, p;
    int cls;

    p = (double)pNorm;
    for (cls = 0; cls < expr->nrClauses; cls++)
    {
        clauseEval = EvalClauseAndPN(anInd, expr->clauses[cls], pNorm);
        result += MyPow(clauseEval, p);
    }
    p = 1.0 / p;
    result = MyPow((result / expr->nrClauses), p);
    return result;
}

// Evaluate a SAT expression by evaluating each of its clauses using OR pNorm.
// The pNorm formulaa is also applied here: a sum of the result from each clause 
// to power pNorm, then the sum to power 1/pNorm.
double EvalSATpnormC(Individual *anInd, SATexpr *expr, int pNorm)
{
    double result = 0, clauseEval;
    int cls;

    for (cls = 0; cls < expr->nrClauses; cls++)
    {
        clauseEval = 1 - EvalClauseOrPN(anInd, expr->clauses[cls], pNorm);
        result += MyPow(clauseEval, (double)pNorm);
    }
    result = MyPow((result / expr->nrClauses), (double)(1.0 / pNorm));
    return (double)(1 - result);
}

// Evaluate a SAT expression bassed on the settings in anEval.
double EvalSAT(Individual *anInd, SATexpr *expr, EvalInfo *anEval)	/*11*/
{
    SATevalForm evalForm = SATevalForm(anEval->funcNumber - 11);

    if (expr != NULL)
        switch (evalForm)
        {
        case cnfAve:
            return EvalSATaveC(anInd, expr);
        case  cnfPnorm:
            return EvalSATpnormC(anInd, expr, anEval->pointNumber);
        case dnfAve:
            return EvalSATaveD(anInd, expr);
        case dnfPnorm:
            return EvalSATpnormD(anInd, expr, anEval->pointNumber);
        case hcAve:
            return EvalSatHCave(anInd, expr);
        case hcPnorm:
            return 1.0;
        default: return 1.0;
        }
    else
        return 1.0;
}

/*********************** Other usual Boolean functions *************/

// Returns the count of 1 genes in the chromosome
double OneMax(Individual *anInd)				/*1*/
{
    int i, result = 0;

    for (i = 0; i < anInd->indInfo->indSize; i++)
        result += anInd->chromosome[i];
    return ((double)result);
}

// Counts the 1 genes. If it's less than half, it returns it.
// Otherwise returns the size of the chromosome.
double MultiMax(Individual *anInd)				/*2*/
{
    int i, result = 0;

    for (i = 0; i < anInd->indInfo->indSize; i++)
        result += anInd->chromosome[i];
    if (result <= (anInd->indInfo->indSize) / 2)
        return ((double)result);
    else
        return ((double)(anInd->indInfo->indSize - result));
}

// Fitness called deception. Divides the chromosome into pieces of 3 genes.
// Each is converted to binary, then a value is assigned to it based
// on the decepArr. 
double Deception(Individual *anInd, int decepArr[8])		/*3 - 8*/
{
    int i, result = 0, bit3;

    for (i = 0; i < anInd->indInfo->indSize; i += 3)
    {
        bit3 = anInd->chromosome[i] +
            2 * anInd->chromosome[i + 1] +
            4 * anInd->chromosome[i + 2];
        result += decepArr[bit3];
    }
    return ((double)result);
}

/********************************** General ********************************/

// Choose the fitness evaluation function based on the settings in the 
// anEval parameter.
double ChoseBFunct(Individual *anInd, EvalInfo *anEval)
{
    int fNr = anEval->funcNumber;

    if ((fNr > 10) && (fNr < 20))
        return EvalSAT(anInd, GAManager::globalSAT, anEval);
    else
        switch (fNr)
        {
        case 1: return OneMax(anInd);
        case 2: return MultiMax(anInd);
        case 3: return Deception(anInd, decep);
        case 4: return Deception(anInd, symba);
        case 5: return Deception(anInd, myDecep1);
        case 6: return Deception(anInd, myDecep2);
        case 7: return Deception(anInd, mySymba1);
        case 8: return Deception(anInd, mySymba2);
        case 9: return Deception(anInd, decep1);
        case 10: return Deception(anInd, decep2);
        default: return 1;
        }
}

// Entry point for the fitness evaluation when the chromosome has boolean genes
double EvalBoolean(Individual *anInd, EvalInfo *anEval)
{
    return ChoseBFunct(anInd, anEval);
}































