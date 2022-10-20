/***********************************************************************

   Project: MC-Graph, a C++ implementation of genetic algorithms
			for graph drawing and visualization
   License: Creative Commons, Attribution
   Author:  Dana Vrajitoru
   File:    Individual.h
   Update:  October 2022

   A class handling the chromosomes or individuals.

************************************************************************/

#ifndef Individual_h
#define Individual_h

#include <fstream>
using namespace std;

#include "List.h"
#include "GenInfo.h"
#include "RunInfo.h"

// Sex type: male, female, mixMate = reproducing with any individual but 
// itself, selfMate = reproducing with anyone, even itself (cloning)
typedef enum { female, male, mixMate, selfMate } SexType;

typedef enum {setValue, binRand, limRand, byteRand} RandIndType;

class Individual {
public:
    short *chromosome;
    GenInfo *indInfo;
    SexType sex;

    ///////////////////////// Constructors ///////////////////////////////

    // Constructor with settings, an option to randomize or not, 
    // and an initial value for the genes.
    Individual(GenInfo *anInfo, RandIndType randomOpt = setValue, 
               short val = 0);

    // Copy constructor with an option to decode it from another individual
    Individual(Individual *anInd, short decode = 0);

    ///////// Initialization corresponding to each constructor ////////////

    // Initialization with an option to randomize or not, 
    // and an initial value for the genes.
    void Init(RandIndType randomOpt = setValue, short val = 0);

    // Copy the chromosome from another individual
    void Init(Individual *anInd);

    // Destructor, must free the chromosome 
    ~Individual();

    ////////////////////// Convergence checking ///////////////////////////

    // Add the chromosome to data's genes by applying a bitwise OR
    void addOr(Individual *data);

    // Add the chromosome to data's genes by applying a bitwise AND
    void addAnd(Individual *data);

    // Find the difference with data based on the transformation number
    double difference(Individual *data);

    // Measure for the difference between the target individual and data.
    // It counts all the different genes and divides the result by the size. 
    double diff(Individual *data);

    // Measure for the difference between the target individual and data.
    // It adds all the indexes where the genes are different and and divides 
    // by size*(size-1)/2, which is the sum of all the indexes.  
    double diff_linear(Individual *data);

    // Measure for the difference between the target individual and data.
    // It adds (size-index-1) for all the indexes where the genes are
    // different and and divides by size*(size-1)/2, which is the sum
    // of all the indexes.
    double diff_linear_back(Individual *data);

    // Measure for the difference between the target individual and
    // data based on a sampling rate. It partitions the individual in
    // a 1/rate intervals, and selects a random index in each interval
    // to see if the genes are equal. Counts the different genes found
    // this way and divides by the size.
    double diff_rand(Individual *data, double rate);

    /////////////////////////////// Output ////////////////////////////////

    // Output to the console with an explanation message
    void Print(char *name);

    // Output to a file with an explanation message
    void FPrint(FILE *aFile, char *name);

    // Output to a file with an explanation message
    void FPrint(ofstream &aFile, char *name);

    ///////////////////////////// Accessors ////////////////////////////////

    // The size comes from the info attribute, so we only store it one for 
    // the entire generation.
    int GetSize();

    // Start index for the genes, if we ever decide it's not 0.
    int GenStart();

    // Genetic operator: determine the sex of the child based on the
    // parents and on the reproduction form. If the reproduction is
    // sexuate, the child receives a random parent's sex. If it's
    // sexBinary, then it receives a random male or female
    // sex. Otherwise it's mixMate.
    void DetermineSex(Individual *&parent1, Individual *&parent2, 
                      RepForm aRepForm);

    // Determine the sex of the individual in the initial population
    // based on the reproduction form. If the reproduction is sexuate,
    // the individual receives a random sex type out of the 4. If it's
    // sexBinary, the individual is randomly either male or female.
    void DetermineSex(RepForm aRepForm);
};

#endif
