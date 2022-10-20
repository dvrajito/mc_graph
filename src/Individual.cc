/***************************************************************

   Project: MC-Graph, a C++ implementation of genetic algorithms
            for graph drawing and visualization
   License: Creative Commons, Attribution
   Author:  Dana Vrajitoru
   File:    Individual.cc
   Update:  October 2022

   A class handling the chromosomes or individuals.

***************************************************************/

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
using namespace std;
#include "Individual.h"
#include "EvalInfo.h"
#include "GAManager.h"

//////////////////////// Constructors//////////////////////////////

// Constructor with settings, an option to randomize or not, 
// and an initial value for the genes.
Individual::Individual(GenInfo *anInfo, RandIndType randomOpt, short val)
{
    int size;
    indInfo = anInfo;
    size = GetSize();
    chromosome = new short[size];
    Init(randomOpt, val);
}

// Copy constructor with an option to decode it from another individual
Individual::Individual(Individual *anInd, short decode)
{
    GenInfo *decodeInfo = NULL;

    if ((decode) && (anInd->indInfo->encode)) {
        decodeInfo = new GenInfo(anInd->indInfo);
        decodeInfo->encode = 0;
        if ((decodeInfo->indSize) % 8 != 0)
            decodeInfo->indSize = (decodeInfo->indSize / 8 + 1) * 8;
        indInfo = decodeInfo;
        chromosome = new short[decodeInfo->indSize];
        Init();
    }
    else {
        indInfo = anInd->indInfo;
        chromosome = new short[anInd->GetSize()];
        Init(anInd);
    }
}

////// Initialization corresponding to each constructor ////////

// Initialization with an option to randomize or not, 
// and an initial value for the genes.
void Individual::Init(RandIndType randomOpt, short val)
{
    int i, size = GetSize();

    switch (randomOpt) {
    case setValue:
        for (i = 0; i < size; i++)
            chromosome[i] = val;
        break;
    case binRand:
        for (i = 0; i < size; i++)
            chromosome[i] = rand() % 2;
        break;
    case limRand: {
        int limit = int(GAManager::aGenInfo->supLim);
        for (i = 0; i < size; i++)
            chromosome[i] = rand() % limit;
        break;
    }
    case byteRand:
        for (i = 0; i < size; i++)
            chromosome[i] = rand() % 256;
        break;
    }
    DetermineSex(GAManager::aRunInfo->aRForm);
}

// Copy the chromosome from another individual
void Individual::Init(Individual *anInd)
{
    int i, size = GetSize();

    for (i = 0; i < size; i++)
        chromosome[i] = anInd->chromosome[i];
    sex = anInd->sex;
}

// Destructor, must free the chromosome 
Individual::~Individual()
{
    delete chromosome;
}

///////////////// Convergence checking /////////////////////////////

// Add the chromosome to data's genes by applying a bitwise OR
void Individual::addOr(Individual *data)
{
    for (int i = 0; i < GetSize(); i++)
        chromosome[i] = chromosome[i] || data->chromosome[i];
}

// Add the chromosome to data's genes by applying a bitwise AND
void Individual::addAnd(Individual *data)
{
    for (int i = 0; i < GetSize(); i++)
        chromosome[i] = chromosome[i] && data->chromosome[i];
}

// Find the difference with data based on the transformation number
double Individual::difference(Individual *data)
{
    switch (GAManager::anEvalInfo->transNumber) {
    case 0:
        return diff(data);
    case 1:
        return diff_rand(data, 0.05);
    case 2:
        return diff_rand(data, 0.1);
    case 3:
        return diff_rand(data, 0.2);
    case 4:
        return diff_linear(data);
    case 5:
        return diff_linear_back(data);
    default:
        return diff(data);
    }
}

// Measure for the difference between the target individual and data.
// It counts all the different genes and divides the result by the size. 
double Individual::diff(Individual *data)
{
    int sum = 0, i, size = GetSize();
    for (i = 0; i < size; i++)
        if (chromosome[i] != data->chromosome[i])
            sum++;
    if (size != 0)
        return (((double)sum) / ((double)size));
    else
        return 0.0;
}

// Measure for the difference between the target individual and data.
// It adds all the indexes where the genes are different and and divides 
// by size*(size-1)/2, which is the sum of all the indexes.  
double Individual::diff_linear(Individual *data)
{
    int sum = 0, i, size = GetSize();
    for (i = 0; i < size; i++)
        if (chromosome[i] != data->chromosome[i])
            sum += i;
    if (size > 1)
        return ((2.0 * sum) / ((double)size * (size - 1)));
    else
        return sum;
}

// Measure for the difference between the target individual and data.
// It adds (size-index-1) for all the indexes where the genes are different 
// and and divides by size*(size-1)/2, which is the sum of all the indexes.  
double Individual::diff_linear_back(Individual *data)
{
    int sum = 0, i, size = GetSize();
    for (i = 0; i < size; i++)
        if (chromosome[i] != data->chromosome[i])
            sum += size - i - 1;
    if (size != 0)
        return ((2.0 * sum) / ((double)size * (size - 1)));
    else
        return 0.0;
}

// Measure for the difference between the target individual and data based
// on a sampling rate. It partitions the individual in a 1/rate intervals, 
// and selects a random index in each interval to see if the genes are 
// equal. Counts the different genes found this way and divides by the size. 
double Individual::diff_rand(Individual *data, double rate)
{
    int sum = 0, i, j, size = GetSize(), start, end;
    if (chromosome[size - 1] != data->chromosome[size - 1])
        sum++;
    int step = int(rate*size), samples = int(1.0 / rate);
    for (i = 0; i < samples; i++) {
        start = i*step;
        end = (i < size - 1) ? (i + 1)*step : size - 1;
        j = start + rand() % (end - start);
        if (chromosome[j] != data->chromosome[j])
            sum++;
    }
    if (size != 0)
        return (((double)sum) / ((double)(indInfo->indSize)));
    else
        return 0.0;
}

////////////////////////////////// Output //////////////////////////////////

// Output to the console with an explanation message
void Individual::Print(char *name)
{
    cout << endl << "The individual " << name << "is:";
    for (int i = 0; i < GetSize(); i++)
        cout << chromosome[i];
    cout << endl;
}

// Output to a file with an explanation message
void Individual::FPrint(FILE *aFile, char *name)
{
    if (name != NULL)
        fprintf(aFile, "The individual %s is: \n", name);
    for (int i = 0; i < GetSize(); i++)
        fprintf(aFile, "%1d ", chromosome[i]);
    fprintf(aFile, "\n");
}


// Output to a file with an explanation message
void Individual::FPrint(ofstream &aFile, char *name)
{
    if (name != NULL)
        aFile << "The individual " << name << " is: " << endl;
    for (int i = 0; i < GetSize(); i++)
        aFile << chromosome[i] << ' ';
    aFile << endl;
}

//////////////////////////// Accessors //////////////////////////////////

// The size comes from the info attribute, so we only store it one for 
// the entire generation.
int Individual::GetSize()
{
    if (indInfo->encode) {
        if ((indInfo->indSize) % 8 == 0)
            return (indInfo->indSize) / 8;
        else
            return (indInfo->indSize) / 8 + 1;
    }
    else
        return indInfo->indSize;
}

// Start index for the genes, if we ever decide it's not 0.
int Individual::GenStart()
{
    return 0;
}

// Genetic operator: determine the sex of the child based on the parents and on
// the reproduction form. If the reproduction is sexuate, the child receives
// a random parent's sex. If it's sexBinary, then it receives a random male or
// female sex. Otherwise it's aleha.
void Individual::DetermineSex(Individual *&parent1, Individual *&parent2,
                              RepForm aRepForm)
{
    int s = rand() % 2;

    if (aRepForm == sexuate)
        if (s == 0)
            sex = parent1->sex;
        else
            sex = parent2->sex;
    else if (aRepForm == sexBinary)
        sex = (SexType)s;
    else
        sex = mixMate;
}

// Determine the sex of the individual in the initial population based on 
// the reproduction form. If the reproduction is sexuate, the individual     
// receives a random sex type out of the 4. If it's sexBinary, the individual
// is randomly either male or female. 
void Individual::DetermineSex(RepForm aRepForm)
{
    int s;

    if (aRepForm == sexuate)
    {
        s = rand() % 4;
        sex = (SexType)s;
    }
    else if (aRepForm == sexBinary) 
    {
        s = rand() % 2;
        sex = (SexType)s;
    }
    else
        sex = mixMate;
}