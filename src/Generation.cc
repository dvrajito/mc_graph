/***************************************************************************

   Project: MC-Graph, a C++ implementation of genetic algorithms
            for graph drawing and visualization
   License: Creative Commons, Attribution
   Author:  Dana Vrajitoru
   File:    Generation.cc
   Update:  October 2022

   A class handling the generation.

****************************************************************************/

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
using namespace std;
#include <iomanip>
#include "Generation.h"
#include "General.h"
#include "EvalReal.h"
#include "GenEval.h"
#include "GenOper.h"
#include "GAManager.h"

///////////////////////////// Constructors ////////////////////////////////////

// Constructor with value for size, genetic settings object, randomizing option,
// and initial value for the genes
Generation::Generation(int genSize, GenInfo *&anInfo, RandIndType randomOpt, short val)
    : decoded1(NULL), decoded2(NULL), dchild1(NULL), dchild2(NULL),
    sameSex(0), diffSex(0), storeGen(NULL), popRadius(0), sorted(1)
{
    popSize = genSize;
    int size = (popSize / 2.0 + 0.5) * 2;
    population = new Individual *[size];
    fitValues = new double[size];
    Init(anInfo, randomOpt, val);
}

// Copy constructor
Generation::Generation(Generation *&data)
    : decoded1(NULL), decoded2(NULL), dchild1(NULL), dchild2(NULL),
    sameSex(0), diffSex(0), storeGen(NULL), popRadius(0), sorted(1)
{
    popSize = data->popSize;
    int size = (popSize / 2.0 + 0.5) * 2;
    population = new Individual *[size];
    fitValues = new double[popSize];
    Init(data);
}

////////////// Init functions corresponding to each constructor ////////////////

// Initialize with genetic settings object, randomizing option, 
// and intial value for the genes
void Generation::Init(GenInfo *&anInfo, RandIndType randomOpt, short val)
{
    int size = (popSize / 2.0 + 0.5) * 2;

    for (int i = 0; i < size; i++) {
        population[i] = new Individual(anInfo, randomOpt, val);
        fitValues[i] = 0;
    }
    Eval();
}

// Copy init
void Generation::Init(Generation *&data)
{
    int size = (popSize / 2.0 + 0.5) * 2;

    for (int i = 0; i < size; i++) {
        population[i] = new Individual(data->population[i]);
        if (i < popSize)
            fitValues[i] = data->fitValues[i];
    }
}

// Reset all the individuals in the population without reallocating the memory
void Generation::ReInit(RandIndType randomOpt, short val)
{
    int size = (popSize / 2.0 + 0.5) * 2;

    for (int i = 0; i < size; i++) {
        population[i]->Init(randomOpt, val);
        fitValues[i] = 0;
    }
    Eval();
}

// Destructor
Generation::~Generation()
{
    int i;

    for (i = 0; i < popSize; i++)
        delete population[i];
    delete population;
    delete fitValues;
}

///////////////////////////// Manipulation ///////////////////////////

// Swap two chromosomes at given indexes
void Generation::SwapInd(int i, int j)
{
    double buff;
    Individual *buffInd;

    buff = fitValues[i];
    fitValues[i] = fitValues[j];
    fitValues[j] = buff;
    buffInd = population[i];
    population[i] = population[j];
    population[j] = buffInd;
}

// Sort the population by the fitness value using a linear
// insertion sort. We assume the population is not large
// enough for a more efficient sort.
void Generation::Sort()
{
    int i, j;

    for (i = 1; i < popSize; i++)
        for (j = i; j > 0 && fitValues[j] < fitValues[j-1]; j--)
            SwapInd(j, j-1);
}

// Find the chromosomes of minimum and maximum fitness and 
// moves them to the first and last positions
void Generation::MinMax()
{
    int min = 0, max = 0;
    for (int i = 1; i < popSize; i++) {
        if (fitValues[i] < fitValues[min])
            min = i;
        if (fitValues[i] > fitValues[max])
            max = i;
    }
    if (max != 0) {
        if (min != 0)
            SwapInd(min, 0);
        if (max != popSize - 1)
            SwapInd(max, popSize - 1);
    }
    else {
        SwapInd(max, popSize - 1);
        if (min != 0)
            SwapInd(min, 0);
    }
}

// A measure of how much the population has converged already, or
// how much diversity we still have in the population.
double Generation::Converge()
{
    int i;

    if (population != NULL) {
        Individual sumInd0(population[0]), sumInd1(population[0]);
        for (i = 1; i < popSize - 1; i++) {
            sumInd0.addAnd(population[i]);
            sumInd1.addOr(population[i]);
        }
        return sumInd0.difference(&sumInd1);
    }
    else
        return 0.0;
}

// Output to the console starting with a given message
void Generation::Print(char *message)
{
    int i;
    char indMess[20];
    double fitVal;

    for (i = 0; i < popSize; i++)
    {
        sprintf(indMess, "%s%d\0", message, i);
        /*population[i]->Print(indMess);*/
        fitVal = IndToReal(population[i]);
        cout << indMess << fitVal
            << "Value:" << fitValues[i];
    }
}

// Output to a file starting with a given message
void Generation::FPrint(FILE *aFile, char *message)
{
    int i;
    char indMess[20];
    double fitVal;

    for (i = 0; i < popSize; i++)
    {
        sprintf(indMess, "%s%d\0", message, i);
        /*population[i]->Print(indMess);*/
        fitVal = IndToReal(population[i]);
        fprintf(aFile, "%s", indMess);
        fprintf(aFile, " %.4f", fitVal);
        fprintf(aFile, " Value : %f\n", fitValues[i]);
    }
}

// Output to a file starting with a given message
void Generation::FPrint(ofstream &fout, char *message)
{
    int i;
    double fitVal;

    for (i = 0; i < popSize; i++)
    {
        fout << message << i << ' ';
        fitVal = IndToReal(population[i]);
        fout << fixed << setprecision(4) << fitVal;
        fout.unsetf(ios::fixed);
        fout << " Value : " << fitValues[i] << endl;
    }
}

////////////////////////////// Genetic operations ////////////////////////////

// Sums the fitness of all the chromosomes 
double Generation::FitnessSum()
{
    double result = 0.0;
    int i;

    for (i = 0; i < popSize; i++)
        result += fitValues[i];
    return result;
}

// Sums the fitness of the chromosomes between indexes start and end
double Generation::FitnessSum(int start, int end)
{
    double result = 0.0;
    int i;
    if (start < 0)
        start = 0;
    if (end >= popSize)
        end = popSize - 1;

    for (i = start; i <= end; i++)
        result += fitValues[i];
    return result;
}

// Fitness-proportionate selection where the individual is selected
// from a radius around a given position. If radius is 0, then the 
// percentage is used for the selection instead.
// Returns the index of the selected chromosome.
int Generation::RouletWheel(int position)
{
    int which;
    int radius = matingRadius;
    double choice, sum = 0.0;

    // If the radius is 0, then determine it based on the percentage
    if (matingRadius == 0)
        radius = popSize * matingPercent;

    int start = position - radius - 1, end = position + radius;
    if (start < 0)
        start = 0;
    if (end >= popSize)
        end = popSize - 1;
    double totalSum = FitnessSum(start, end);

    choice = RealRand(randPrecision);
    which = start;
    do {
        sum += fitValues[which] / totalSum;
        which++;
    } while (sum <= choice);
    which--;
    return which;
}

// Simple itness-proportionate selection. 
// Returns the index of the selected chromosome.
int Generation::RouletWheel(double totalSum)
{
    int which, i;
    double offset = 0, minFit = fitValues[0], maxFit = fitValues[0];
    double choice, sum = 0.0;

    // find the min and max value
    for (i = 1; i < popSize; i++)
        if (fitValues[i] < minFit)
            minFit = fitValues[i];
        else if (fitValues[i] > maxFit)
            maxFit = fitValues[i];

    if (minFit == maxFit) // all chromosomes have the same fitness
        return abs(rand() % popSize); // return a random one

    // offset the fitness so that we don't have negative values
    if (minFit <= 0) 
    {                
        offset = -minFit + 0.1*(maxFit - minFit);
        totalSum += popSize * offset;
    }

    // now select a chromosome
    choice = RealRand(randPrecision);
    which = 0;
    do {
        sum += fitValues[which] / totalSum;
        which++;
    } while (sum <= choice);
    which--;
    return which;
}

// Counts the occurrence of each sex type in the population and prints
// it out. Then returns the sex that is most present in the population. 
int Generation::DominateSex()
{
    int count[4] = { 0, 0, 0, 0 };
    int i, max = 0, which = -1;

    // Count occurrences of each sex type in the population
    for (i = 0; i < popSize; i++)
        count[(int)(population[i]->sex)] ++;

    // Output the counts and find the maximum of them
    for (i = 0; i < 4; i++) {
        cout << count[i] << ' ';
        if (max <= count[i]) {
            which = i;
            max = count[i];
        }
    }
    return which;
}

// Find a mate for parent1. Needs the total fitness sum in the population. 
// Returns the index of the second parent (the mate).
int Generation::Mate(int parent1, double fitSum)
{
    int found = 0, trying = 0;
    int which;
    Individual *res;
    Individual *part = population[parent1];

    while (!found) {
        trying++;
        if (popRadius)
            which = RouletWheel(parent1);
        else
            which = RouletWheel(fitSum);
        res = population[which];
        if (res->sex == selfMate) // selfMate can mate with anyone
            found = 1;
        else if (res != part) {
            if (res->sex == mixMate) // mixMate can mate with anyone but itself
                found = 1;
            else if (res->sex != part->sex) // the others can mate only with
                found = 1;                  // someone of a different sex than itself
            // If we have been unsuccessful enough times:
            else if ((trying > matingPopPercent*popSize) || 
                     (trying > matingTries)) {
                found = 1;
                // make a sex change to make the mating possible
                // assuming that only male/female would have been 
                // unsuccesful this many times
                res->sex = (SexType)(1 - part->sex);
            }
        }
    }
    return which;
}

// Find a mate for parent1 that is different from it. Needs the total fitness 
// sum in the population. Returns the index of the second parent (the mate).
// Makes 5 attempts to find a different mate.
int Generation::MateDiff(int parent1, double fitSum)
{
    double difference = 0.0, newDiff;
    int which;
    Individual *res = NULL, *try_ind;
    Individual *part = population[parent1];

    for (int i = 0; i < TRY_NR; i++) { // consider this many mates
        if (popRadius)                 // get the mate the usual way
            which = RouletWheel(parent1);
        else
            which = RouletWheel(fitSum);
        try_ind = population[which];
        newDiff = part->difference(try_ind); // findout how different they are
        if (res == NULL || newDiff > difference) {
            res = try_ind;            // if a larger difference, keep it
            difference = newDiff;
        }
    }
    if (part->chromosome[part->indInfo->indSize - 1] ==
        res->chromosome[res->indInfo->indSize - 1])
        sameSex++;
    else
        diffSex++;
    return which;
}

// Decodes the two parents either into decoded1 and decoded2.
void Generation::Decode(Individual *&parent1, Individual *&parent2,
                        Individual *&child1, Individual *&child2)
{
    if ((decoded2 == NULL) ||
        (decoded2->indInfo->indSize < parent2->indInfo->indSize)) {
        decoded1 = new Individual(parent1, 1);
        decoded2 = new Individual(parent1, 1);
        dchild1 = new Individual(parent1, 1);
        dchild2 = new Individual(parent1, 1);
    }
    DecodeInd(parent1, decoded1);
    DecodeInd(parent2, decoded2);
}

// Encodes the children into dchild1 and dchild2.
void Generation::Encode(Individual *&parent1, Individual *&parent2,
                        Individual *&child1, Individual *&child2)
{
    EncodeInd(dchild1, child1);
    EncodeInd(dchild2, child2);
}

// evaluate the individuals in the population
void Generation::Eval()
{
    EvalGen(this, GAManager::anEvalInfo);
}

// Create two children from two parents using crossover and mutation
void Generation::Breed(Individual *&parent1, Individual *&parent2,
                       Individual *&child1, Individual *&child2,
                       CrossMethod *&aCrossover, RepForm aRepForm)
{
    // Apply crossover to the parents to build the children,
    // then apply mutation to the two children. 
    if (parent1->indInfo->encode) { // decode the parents and ecode the children if needed
        Decode(parent1, parent2, child1, child2);
        Crossover(decoded1, decoded2, dchild1, dchild2, aCrossover);
        Mutation(dchild1);
        Mutation(dchild2);
        Encode(parent1, parent2, child1, child2);
    }
    else {
        Crossover(parent1, parent2, child1, child2, aCrossover);
        Mutation(child1);
        Mutation(child2);
    }
    // Set the sex type of the children based on the parents
    child1->DetermineSex(parent1, parent2, aRepForm);
    child2->DetermineSex(parent1, parent2, aRepForm);
}

// Create a new generation from the old one stored in the target object
Generation *Generation::Reproduce(CrossMethod *&aCrossover,
                                  RepForm aRepForm,
                                  Generation *&whereGen)
{
    Generation *newGen;
    Individual *parent1, *parent2, *child1, *child2;
    int i, genSize, p1, p2;
    double fitSum;

    genSize = popSize;

    // check if we have the memory allocated
    if (whereGen == NULL) {
        if (storeGen == NULL)
            storeGen = new Generation(genSize,
                                      population[0]->indInfo);
        newGen = storeGen;
        newGen->ReInit(binRand);
    }
    else
        newGen = whereGen;

    // select size/2 pairs of parents to create children from 
    // by crossover or cloning.
    fitSum = FitnessSum();
    for (i = 0; i < 2*(genSize/2); i += 2) {// in case the reproduction is elitist                       
        p1 = RouletWheel(fitSum);
        parent1 = population[p1]; // select the first parent based on the fitness
        if (aRepForm < sexuate) { // select the second parent 
            if (popRadius)
                p2 = RouletWheel(p1);
            else
                p2 = RouletWheel(fitSum);
        }
        else if (aRepForm == diverse)
            p2 = MateDiff(p1, fitSum);
        else
            p2 = Mate(p1, fitSum);
        parent2 = population[p2];
        child1 = newGen->population[i];
        child2 = newGen->population[i + 1];

        // Create the children
        Breed(parent1, parent2, child1, child2, aCrossover, aRepForm);
    }

    // If the reproduction is elitist, copy the last individual (of best fitness) from 
    // the old generation to the new one.
    if (aRepForm == elitist)
        newGen->population[genSize - 1] = new Individual(population[genSize - 1]);

    // Evaluate the fitness of the new population 
    newGen->Eval();

    // Reset the crossover form if we're doing some combination
    if (aCrossover->theCrossover == combineCrs &&
        aCrossover->combForm == adaptCmb)
        ReadaptCross(newGen->fitValues, genSize);

    // Sort the population of just move the worst to position 0 and the best to the last
    if (sorted)
        newGen->Sort();
    else
        newGen->MinMax();

    // If the reproduction is monotonous, compare the best individual from the old generation
    // with the best in the new one. If the old one was better, replace the worst individual
    // in the new generation with the best in the old and re-sort. 
    if (aRepForm >= monotone)
        if (newGen->fitValues[genSize - 1] < fitValues[genSize - 1]) {
            newGen->population[0]->Init(population[genSize - 1]);
            newGen->fitValues[0] = fitValues[genSize - 1];
            if (sorted)
                newGen->Sort();
            else
                newGen->MinMax();
        }
    return newGen;
}

// Runs one trial of the genetic algorithm. Entry point function.
void Generation::GARun(RunInfo *&aRInfo)
{
    short i, step, converge = 0;
    short saveCross = aRInfo->theCross->theCrossover;
    Generation *oldGen, *newGen = NULL, *interm;
    double lastFitness = 0.0;
    int nInterm = GAManager::aRunInfo->reportNr;
    ofstream gendStat;

    // gather information
    ResetCrossMix();
    oldGen = this;
    step = aRInfo->genNumber / nInterm;
    if (aRInfo->aRForm == diverse)
        gendStat.open(statFile, ios::app);

    // if we choose an elitist reproduction where we keep the best chromosome
    // from the old generation if we don't find anything better,
    // then we need one more space to conserve this chromosome.
    if (aRInfo->aRForm == elitist) {
        oldGen->popSize += 1;
        oldGen->fitValues = (double *)realloc(oldGen->fitValues,
                                    oldGen->popSize * sizeof(double));
        oldGen->fitValues[oldGen->popSize - 1] = 0;
    }

    // Rebuild the new generation from the old for the given number of generations
    for (i = 0; i < aRInfo->genNumber; i++) {
        aRInfo->theCross->RoundRobin(i);

        // Call the function creating the new generation from the old one
        newGen = oldGen->Reproduce(aRInfo->theCross,
                                   aRInfo->aRForm, newGen);

        // every step number of steps write the results to the file
        if ((i != 0) && ((i % step) == 0)) {
            GAManager::resFile << newGen->fitValues[newGen->popSize - 1] << "\t";
            if (aRInfo->aRForm == diverse) {
                gendStat << sameSex << " " << diffSex << "\t";
                sameSex = diffSex = 0;
            }
            if (GAManager::aRunInfo->aRForm > monotone)
                newGen->DominateSex();
        }
        
        // swap the old and new generation
        interm = oldGen;
        oldGen = newGen;
        newGen = interm;

        // update last best fitness and convergence information
        if (oldGen->fitValues[oldGen->popSize - 1] != lastFitness) {
            lastFitness = oldGen->fitValues[oldGen->popSize - 1];
            converge = i;
            if (aRInfo->history) {
                GAManager::histFile << converge << "\t" << lastFitness << endl;
                oldGen->population[oldGen->popSize - 1]->FPrint(GAManager::histFile, NULL);
            }
        }
    }

    // Output the results
    aRInfo->theCross->theCrossover = (CrossForm)saveCross;
    GAManager::resFile << endl << "Last best fitness:\t" << newGen->fitValues[newGen->popSize - 1] << endl;
    GAManager::resFile << "Last change generation:\t" << converge << endl;
    GAManager::resFile << "Convergence rate:\t" << newGen->Converge() << endl;
    if (aRInfo->aRForm == diverse) {
        gendStat << endl <<  sameSex << " " << diffSex << "\t" << endl;
        gendStat.close();
    }
    else if (aRInfo->aRForm >= sexuate) {
        GAManager::resFile << "Dominate sex:\t" << newGen->DominateSex() << endl;
    }
}
