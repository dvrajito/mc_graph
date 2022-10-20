/********************************************************************

   Project: MC-Graph, a C++ implementation of genetic algorithms
            for graph drawing and visualization
   License: Creative Commons, Attribution
   Author:  Dana Vrajitoru
   File:    EvalBool.h
   Update:  October 2022

   Evaluation (fitness) functions for the genetic algorithm with
   any kind of logical problem.

   Evaluation uses SAT expressions which are collection of
   clauses, each of them a collection or terms or predicates.

*********************************************************************/

#ifndef EvalBool_h
#define EvalBool_h

#include "Individual.h"
#include "SATexpr.h"
#include "EvalInfo.h"

// Read the global SAT expression from a file
void ReadSATexpr(char *fileName);

// Delete the global SAT expression on exit.
void deleteGlobalSAT();

// Entry point for the fitness evaluation when the chromosome has
// boolean genes
double EvalBoolean(Individual *anInd, EvalInfo *anEval);

// Choose the fitness evaluation function based on the settings in the 
// anEval parameter.
double ChoseBFunct(Individual *anInd, EvalInfo *anEval);

/////////////////////////// Evaluation functions //////////////////////////

// Returns the count of 1 genes in the chromosome
double OneMax(Individual *anInd);

// Counts the 1 genes. If it's less than half, it returns it.
// Otherwise returns the size of the chromosome.
double MultiMax(Individual *anInd);

// Fitness called deception. Divides the chromosome into pieces of 3 genes.
// Each is converted to binary, then a value is assigned to it based
// on the decepArr. 
double Deception(Individual *anInd, int decepArr[8]);

///////////////////////// SAT Eval /////////////////////////////////////////

// Evaluate a SAT expression bassed on the settings in anEval.
double EvalSAT(Individual *anInd, SATexpr *expr, EvalInfo *anEval);

///////////////////////// Evaluate a clause //////////////////////////////////

// Evaluates a single term, which is part of a clause.
// Returns sign(aTerm)*value of the chromosome at index |aTerm|.
int EvalTerm(Individual *anInd, int aTerm);

// Evaluates a clause by applying bitwise XOR to each term in the clause 
// with each gene in the individual and then taking the average of the results. 
double EvalClauseXorAve(Individual *anInd, SATclause *aClause);

// Evaluates a clause by applying bitwise AND to each term in the clause 
// with each gene in the individual and then taking the average of the results. 
double EvalClauseAndAve(Individual *anInd, SATclause *aClause);

// Evaluates a clause by applying bitwise OR to each term in the clause 
// with each gene in the individual and then taking taking the or of the results.
double EvalClauseOrAve(Individual *anInd, SATclause *aClause);

// Returns (sum((clause[i] or chrom[i])^pNorm)^(1/pNorm)
double EvalClauseOrPN(Individual *anInd, SATclause *aClause, int pNorm);

// Returns (sum((clause[i] and chrom[i])^pNorm)^(1/pNorm)
double EvalClauseAndPN(Individual *anInd, SATclause *aClause, int pNorm);

// Returns (sum((clause[i] xor chrom[i])^pNorm)^(1/pNorm)
double EvalClauseXorPN(Individual *anInd, SATclause *aClause, int pNorm);

///////////////////////// Evaluate a SAT expression ////////////////////////////

// Evaluate a SAT expression by evaluating each of its clauses using OR Average.
// The results are then averaged over all the clauses. 
double EvalSatHCave(Individual *anInd, SATexpr *expr);

// Evaluate a SAT expression by evaluating each of its clauses using OR Average.
// The results are then averaged over all the clauses. 
double EvalSATaveC(Individual *anInd, SATexpr *expr);

// Evaluate a SAT expression by evaluating each of its clauses using AND Average.
// The results are then averaged over all the clauses. 
double EvalSATaveD(Individual *anInd, SATexpr *expr);

// Evaluate a SAT expression by evaluating each of its clauses using AND pNorm.
// The pNorm formulaa is also applied here: a sum of the result from each clause 
// to power pNorm, then the sum to power 1/pNorm.
double EvalSATpnormD(Individual *anInd, SATexpr *expr, int pNorm);

// Evaluate a SAT expression by evaluating each of its clauses using OR pNorm.
// The pNorm formulaa is also applied here: a sum of the result from each clause 
// to power pNorm, then the sum to power 1/pNorm.
double EvalSATpnormC(Individual *anInd, SATexpr *expr, int pNorm);

#endif
