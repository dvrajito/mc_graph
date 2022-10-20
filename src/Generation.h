/***************************************************************************

   Project: MC-Graph, a C++ implementation of genetic algorithms
            for graph drawing and visualization
   License: Creative Commons, Attribution
   Author:  Dana Vrajitoru
   File:    Generation.h
   Update:  October 2022

   A class handling the generation.

****************************************************************************/

#ifndef Generation_h
#define Generation_h

#include "GenInfo.h"
#include "Individual.h"
#include "RunInfo.h"
#include "EvalInfo.h"

class Generation {
protected:
    int sameSex, diffSex;
    Generation *storeGen;
    int popRadius, sorted;

public:
    Individual *decoded1, *decoded2, *dchild1, *dchild2;
    int popSize;
    Individual **population;
    double *fitValues;

    ///////////////////////////// Constructors /////////////////////////////

    // Constructor with value for size, genetic settings object,
    // randomizing option, and initial value for the genes
    Generation(int genSize, GenInfo *&anInfo, 
               RandIndType randomOpt = setValue, short val = 0);

    // Copy constructor
    Generation(Generation *&data);

    //////////// Init functions corresponding to each constructor //////////

    // Initialize with genetic settings object, randomizing option, 
    // and intial value for the genes
    void Init(GenInfo *&anInfo, RandIndType randomOpt = setValue, 
              short val = 0);

    // Copy init
    void Init(Generation *&data);

    // Reset all the individuals in the population without
    // reallocating the memory
    void ReInit(RandIndType randomOpt = setValue, short val = 0);

    // Destructor
    ~Generation();

    //////////////////////////////// Manipulation //////////////////////////

    // Swap two chromosomes at given indexes
    void SwapInd(int i, int j);

    // Sort the population by the fitness value using a linear
    // insertion sort. We assume the population is not large
    // enough for a more efficient sort.
    void Sort();

    // Find the chromosomes of minimum and maximum fitness and 
    // moves them to the first and last positions
    void MinMax();

    // A measure of how much the population has converged already, or
    // how much diversity we still have in the population.
    double Converge();

    // Output to the console starting with a given message
    void Print(char *message);

    // Output to a file starting with a given message
    void FPrint(FILE *aFile, char *message);

    // Output to a file starting with a given message
    void FPrint(ofstream &fout, char *message);

    //////////////////////////// Genetic operations ///////////////////////

    // Sums the fitness of all the chromosomes 
    double FitnessSum();

    // Sums the fitness of the chromosomes between indexes start and end
    double FitnessSum(int start, int end);

    // Decodes the two parents either into decoded1 and decoded2.
    void Decode(Individual *&parent1, Individual *&parent2,
                Individual *&child1, Individual *&child2);

    // Encodes the children into dchild1 and dchild2.
    void Encode(Individual *&parent1, Individual *&parent2,
                Individual *&child1, Individual *&child2);

    // Fitness-proportionate selection where the individual is selected
    // from a radius around a given position. If radius is 0, then the 
    // percentage is used for the selection instead.
    // Returns the index of the selected chromosome.
    int RouletWheel(double totalSum);

    // Simple itness-proportionate selection. 
    // Returns the index of the selected chromosome.
    int RouletWheel(int position);

    // Counts the occurrence of each sex type in the population and prints
    // it out. Then returns the sex that is most present in the population. 
    int DominateSex();

    // Find a mate for parent1. Needs the total fitness sum in the population. 
    // Returns the index of the second parent (the mate).
    int Mate(int parent1, double fitSum);

    // Find a mate for parent1 that is different from it. Needs the
    // total fitness sum in the population. Returns the index of the
    // second parent (the mate). Makes 5 attempts to find a different mate.
    int MateDiff(int parent1, double fitSum);
    
    // evaluate the individuals in the population
    void Eval();

    // Create two children from two parents using crossover and mutation
    void Breed(Individual *&parent1, Individual *&parent2,
               Individual *&child1, Individual *&child2,
               CrossMethod *&aCrossover, RepForm aRepForm);

    // Create a new generation from the old one stored in the target object
    Generation *Reproduce(CrossMethod *&aCrossover,
                          RepForm aRepForm,
                          Generation *&whereGen);

    // Runs one trial of the genetic algorithm. Entry point function.
    void GARun(RunInfo *&aRInfo);
};

#endif
