/********************************************************************

   Project: GADX, a C++ implementation of genetic algorithms
   License: Creative Commons, Attribution
   Author:  Dana Vrajitoru
   File:    GenOper.cc
   Update:  March 2019

   The main genetic operations: crossover and mutation.

********************************************************************/

#include <cstdio>
#include <cstdlib>
#include <malloc.h>
#include <iostream>
using namespace std;

#include "General.h"
#include "GenOper.h"
#include "EvalInfo.h"

const int nrCross = 4; // number of crossover operators in the mix

double crossPerf[nrCross] = { 19.48, 23.97, 28.46, 28.09 };
double crossMix[nrCross] = { 25, 25, 25, 25 };
double crossFit[nrCross] = { 0, 0, 0, 0 };
int *fitArray = NULL, fitCount = 0;

/////////////////////////////// Mutation /////////////////////////////////

// Mutation performed on the individual. For each gene, a mutation
// 0->1 or 1->0 is performed with a probability given by the probMut
// parameter in the settings.
void Mutation(Individual *anInd)
{
    int locus, choice;

    for (locus = 0; locus < (anInd->indInfo->indSize); locus++) {
        choice = (RealRand(randPrecision) <= (anInd->indInfo->probMut));
        if (choice)
            anInd->chromosome[locus] = 1 - anInd->chromosome[locus];
    }
}

// Swap two integers provided as pointers
void IntSwap(int *int1, int *int2)
{
    int buff;

    buff = *int1;
    *int1 = *int2;
    *int2 = buff;
}

// quicksort on an array of integers
void Sort(int *arr, short n)
{
    quicksort(arr, 0, n - 1);
}

// Classic quicksort
void quicksort(int a[], int first, int last)
{
    if (last <= first) return;
    int pivot = a[first];
    int i = first + 1, j = last;
    while (i < j) {
        while (a[i] < pivot && i < j) i++;
        while (a[j] > pivot) j--;
        if (i < j)
            swap(a[i++], a[j--]);
    }
    if (a[j] > pivot) j--;
    swap(a[j], a[first]);
    quicksort(a, first, j - 1);
    quicksort(a, j + 1, last);
} // quicksort()

/////////////////////////////// Crossover /////////////////////////////////

// N-point crossover, with n crossover sites. 
// Builds the children chromosomes from the parent ones.
void NPtCrossover(Individual *parent1, Individual *parent2,
                  Individual *child1, Individual *child2,
                  short n)
{
    int locus, *crossSites, crossPt, choice, i;
    crossSites = new int[n + 2];

    choice = (RealRand(randPrecision) <= (parent1->indInfo->probCross));
    crossSites[0] = -1;
    if (choice) {
        for (i = 1; i <= n; i++)
            crossSites[i] = rand() % (parent1->indInfo->indSize);
        crossSites[n + 1] = parent1->indInfo->indSize - 1;
        Sort(crossSites, n);
    }
    else {
        crossSites[1] = parent1->indInfo->indSize - 1;
        crossSites[2] = parent1->indInfo->indSize;
    }
    crossPt = 1;
    while ((crossPt <= n + 1)
           // && (crossSites[crossPt] < parent1->indInfo->indSize)*/
           ) {
        if (crossPt % 2)
            for (locus = crossSites[crossPt - 1] + 1;
                 locus <= crossSites[crossPt]; locus++) {
            child1->chromosome[locus] = parent1->chromosome[locus];
            child2->chromosome[locus] = parent2->chromosome[locus];
        }
        else
            for (locus = crossSites[crossPt - 1] + 1;
                 locus <= crossSites[crossPt]; locus++) {
            child1->chromosome[locus] = parent2->chromosome[locus];
            child2->chromosome[locus] = parent1->chromosome[locus];
        }
        crossPt++;
    }
}

// Uniform crossover with given swap probability.
void UCrossover(Individual *parent1, Individual *parent2,
                Individual *child1, Individual *child2,
                double pswap)
{
    int locus, choice, swap;

    choice = (RealRand(randPrecision) <= (parent1->indInfo->probCross));
    for (locus = 0; locus < parent1->indInfo->indSize; locus++) {
        swap = (RealRand(randPrecision) <= pswap);
        if (choice && swap) {
            child1->chromosome[locus] = parent2->chromosome[locus];
            child2->chromosome[locus] = parent1->chromosome[locus];
        }
        else {
            child1->chromosome[locus] = parent1->chromosome[locus];
            child2->chromosome[locus] = parent2->chromosome[locus];
        }
    }
}

// Dissociative crossover applied to genes.
short DissGeneCross(short parent1Gn, short parent2Gn, DissocForm operForm)
{
    switch (operForm) {
    case zeroOp:
        return 0;
    case andOp:
        return (parent1Gn && parent2Gn);
    case xorOp:
        if (parent1Gn || parent2Gn)
            return 0;
        else
            return 1;
    case randOp:
        if (rand() % 2 == 0)
            return  parent1Gn;
        else
            return  parent2Gn;
    default:
        return 0;
    }
}

// Dissociative crossover applied to chromosomes.
void DissCrossover(Individual *parent1, Individual *parent2,
                   Individual *child1, Individual *child2,
                   DissocForm operForm)
{
    int locus, crossSiteA, crossSiteB, choice;
    Individual *firstChild = NULL, *secondChild = NULL;

    choice = (RealRand(randPrecision) <= (parent1->indInfo->probCross));
    if (choice) {
        crossSiteA = rand() % (parent1->indInfo->indSize - 1);
        crossSiteB = rand() % (parent1->indInfo->indSize - 1);
        if (crossSiteA < crossSiteB) {
            firstChild = child2;
            secondChild = child1;
        }
        else {
            IntSwap(&crossSiteA, &crossSiteB);
            firstChild = child1;
            secondChild = child2;
        }
    }
    else {
        crossSiteA = (parent1->indInfo->indSize - 1);
        crossSiteB = (parent1->indInfo->indSize - 1);
    }
    for (locus = 0; locus <= crossSiteA; locus++) {
        child1->chromosome[locus] = parent1->chromosome[locus];
        child2->chromosome[locus] = parent2->chromosome[locus];
    }
    for (locus = crossSiteA + 1; locus <= crossSiteB; locus++) {
        firstChild->chromosome[locus] = (parent1->chromosome[locus]) ||
            (parent2->chromosome[locus]);
        secondChild->chromosome[locus] = DissGeneCross(parent1->chromosome[locus],
                                                       parent2->chromosome[locus],
                                                       operForm);
    }
    for (locus = crossSiteB + 1; locus < parent1->indInfo->indSize; locus++) {
        child1->chromosome[locus] = parent2->chromosome[locus];
        child2->chromosome[locus] = parent1->chromosome[locus];
    }
}

// Crossover building the children chromosomes from the parent ones.
// Selection of the crossover form done based on the last parameter.
void Crossover(Individual *parent1, Individual *parent2,
               Individual *child1, Individual *child2,
               CrossMethod *aCrossMtd)
{
    int which = -1;
    double sum = 0;
    switch (aCrossMtd->theCrossover) {
    case onePointCrs:
        NPtCrossover(parent1, parent2, child1, child2, 1);
        break;
    case nPointCrs:
        NPtCrossover(parent1, parent2, child1, child2, aCrossMtd->nPT);
        break;
    case uniformCrs:
        UCrossover(parent1, parent2, child1, child2, aCrossMtd->thePswap);
        break;
    case dissocCrs:
        DissCrossover(parent1, parent2, child1, child2, aCrossMtd->dissForm);
        break;
    case combineCrs:
        switch (aCrossMtd->combForm) {
        case perfCmb:
            which = SelectCross(crossPerf);
            aCrossMtd->theCrossover = CrossForm(which);
            Crossover(parent1, parent2, child1, child2, aCrossMtd);
            aCrossMtd->theCrossover = combineCrs;
            break;
        case eqCmb:
            which = rand() % combineCrs;
            aCrossMtd->theCrossover = CrossForm(which);
            Crossover(parent1, parent2, child1, child2, aCrossMtd);
            aCrossMtd->theCrossover = combineCrs;
            break;
        case adaptCmb:
            which = SelectCross(crossMix);
            AddCross(which);
            AddCross(which);
            aCrossMtd->theCrossover = CrossForm(which);
            Crossover(parent1, parent2, child1, child2, aCrossMtd);
            aCrossMtd->theCrossover = combineCrs;
            break;
        }
        break;
    }
}

////////////////// Mixed and adaptable crossover /////////////////////////

// Selecting a crossover out of the 4 possible ones with 
// the probability to select each is given by the array how. 
int SelectCross(double *how)
{
    double select = RealRand(1000);
    int which = 0, i;
    double sum = 0, interm[nrCross];
    interm[0] = how[0];
    for (i = 0; i < nrCross; i++) {
        sum += how[i];
        interm[i] = interm[i - 1] + how[i];
    }
    while (interm[which] < select*sum)
        which++;
    return which;
}

// Change the crossover mix randomly
void ChangeMixRandom()
{
    for (int i = 0; i < 4; i++) {
        crossMix[i] += (rand() % 10) - 5;
        if (crossMix[i] < 0)
            crossMix[i] *= -1;
    }
}

// Change the crossover mix to make more exploratory
void ChangeMixExplore()
{
    crossMix[2] += rand() % 5;
    crossMix[3] += rand() % 5;
}

// Change the crossover mix to make more exploitative
void ChangeMixExploit()
{
    crossMix[0] += rand() % 5;
    crossMix[1] += rand() % 5;
}

// Modify the crossover mix based on the difference between the old
// and new fitness.
void AdaptCross(double homogen, double oldf, double newf)
{
    double fdiff = 1;
    if (oldf >= newf && oldf != 0)
        fdiff = (oldf - newf) / oldf;
    else
        fdiff = (newf - oldf) / newf;
    if (homogen <= 0.25 || fdiff >= 0.75)
        ChangeMixExploit();
    else if (homogen >= 0.75 || fdiff <= 0.25)
        ChangeMixExplore();
    else if (rand() % 2)
        ChangeMixRandom();
}

// Initialize the fitness array based on the population size
void InitFitArray(int popSize)
{
    if (fitArray != NULL)
        delete fitArray;
    fitArray = new int[popSize];
}

// Reset the fitness count to 0.
void ResetFitCount()
{
    fitCount = 0;
}

// Resets the percentage of crossover mis to 25% each.
void ResetCrossMix()
{
    for (int i = 0; i < nrCross; i++)
        crossMix[i] = 25;
    ResetFitCount();
}

// Add a fitness value to the crossFit array
void AddFitness(double fitv)
{
    crossFit[fitArray[fitCount]] += fitv;
    fitCount++;
}

// Add a crossover number to the fitArray
void AddCross(int crossNr)
{
    fitArray[fitCount] = crossNr;
    fitCount++;
}

// Readjust the crossover mix based on the fitness values array
void ReadaptCross(double *fitvals, int nrvals)
{
    double sum = 0;
    int i;
    ResetFitCount();
    for (i = 0; i < nrvals; i++)
        AddFitness(fitvals[i]);
    ResetFitCount();
    for (i = 0; i < nrCross; i++)
        sum += crossFit[i];
    for (i = 0; i < nrCross; i++) {
        crossFit[i] = 100 * (crossFit[i] / sum);
        crossMix[i] = (crossMix[i] + crossFit[i]) / 2;
        //cout << crossFit[i] << ' ';
    }
    for (i = 0; i < nrCross; i++)
        crossFit[i] = 0;
    //cout << endl;
    //WriteCrossMix();
}

// Output a crossover mix
void WriteCrossMix()
{
    for (int i = 0; i < nrCross; i++)
        cout << crossMix[i] << ' ';
    cout << endl;
}
