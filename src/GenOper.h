/*****************************************************************************

   Project: MC-Graph, a C++ implementation of genetic algorithms
            for graph drawing and visualization
   License: Creative Commons, Attribution
   Author:  Dana Vrajitoru
   File:    GenOper.h
   Update:  October 2022

   The main genetic operations: crossover and mutation.

******************************************************************************/

#ifndef GenOper_h
#define GenOper_h

#include "GenTypes.h"
#include "CrossMethod.h"
#include "General.h"

///////////////////////////////// Mutation /////////////////////////////////////

// Mutation performed on the individual. For each gene, a mutation 0->1 or 1->0
// is performed with a probability given by the probMut parameter in the settings.
void Mutation(Individual *anInd);

// Swap two integers provided as pointers
void IntSwap(int *int1, int *int2);

// quicksort on an array of integers
void Sort(int *arr, short n);

// Classic quicksort
void quicksort(int a[], int first, int last);

////////////////////////////////// Crossover ///////////////////////////////////

// Crossover building the children chromosomes from the parent ones.
// Selection of the crossover form done based on the last parameter.
void Crossover(Individual *parent1, Individual *parent2,
               Individual *child1, Individual *child2,
               CrossMethod *aCrossover);

// N-point crossover, with n crossover sites. 
void NPtCrossover(Individual *parent1, Individual *parent2,
                  Individual *child1, Individual *child2,
                  short n);

// Uniform crossover with given swap probability.
void UCrossover(Individual *parent1, Individual *parent2,
                Individual *child1, Individual *child2,
                double pswap);

// Dissociative crossover applied to genes.
short DissGeneCross(short parent1Gn, short parent2Gn, DissocForm operForm);

// Dissociative crossover applied to chromosomes.
void DissCrossover(Individual *parent1, Individual *parent2,
                   Individual *child1, Individual *child2,
                   DissocForm operForm);

// Selecting a crossover out of the 4 possible ones with 
// the probability to select each is given by the array how.
int SelectCross(double *how);

////////////////// Mixed and adaptable crossover /////////////////////////

// Initialize the fitness array based on the population size
void InitFitArray(int popSize);

// Reset the fitness count to 0.
void ResetFitCount();

// Add a fitness value to the crossFit array
void AddFitness(double fitv);

// Add a crossover number to the fitArray
void AddCross(int crossNr);

void ResetCrossMix();

// Modify the crossover mix based on the difference between the old and new fitness.
void AdaptCross(double homogeneity, double old_fitness, double new_fitness);

// Readjust the crossover mix based on the fitness values array
void ReadaptCross(double *fitvals, int nrvals);

// Output a crossover mix
void WriteCrossMix();

#endif













