#include <stdio.h>
#include <iostream.h>
#include "GenTypes.h"
#include "General.h"
#include "GenEval.h"
#include "GenMain.h"
#include "Individual.h"
#include "Generation.h"
#include "GenOper.h"

extern int randPrecision, nInterm;
extern FILE *resFile, *histFile;

double FitnessSum(Generation *aGen)
{ double result = 0.0;
  int i;

  for (i=0; i < aGen->popSize; i++)
    result += aGen->fitValues[i];
  return result;
}

Individual *RouletWheel(Generation *aGen, double totalSum)
{ int which;
  double choice, sum = 0.0;

  choice = RealRand(randPrecision);
  which = 0;
  do {
    sum += aGen->fitValues[which]/totalSum;
    which++;
  } while (sum <= choice);
  which--;
  return aGen->population[which];
}

Generation *Reproduce(Generation *oldGen, 
                      CrossMethod *aCrossover,
                      RepForm aRepForm,
                      EvalInfo *anEval,
                      Generation *whereGen)
{ 
  Generation *newGen;
  Individual *parent1, *parent2, *child1, *child2, 
    *dummy = new Individual(oldGen->population[0]->indInfo);
  int i, genSize;
  double fitSum;

  genSize = oldGen->popSize;
  if (whereGen == NULL)
    newGen = new Generation(genSize, 
			    oldGen->population[0]->indInfo);
  else
    newGen = whereGen;
  fitSum = FitnessSum(oldGen);
  for (i=0; i<genSize; i += 2) { 
    parent1 = RouletWheel(oldGen, fitSum);
    parent2 = RouletWheel(oldGen, fitSum);
    child1 = newGen->population[i];
    if (i+1 < genSize)
      child2 = newGen->population[i + 1];
    else
      child2 = dummy;
    Crossover(parent1, parent2, child1, child2, aCrossover);
    Mutation(child1);
    Mutation(child2);
  }
  if (aRepForm == elitist)
    newGen->population[genSize-1] = new Individual(oldGen->population[genSize-1]);
  EvalGen(newGen, anEval);
  newGen->Sort();
  if (aCrossover->theCrossover == combineCrs &&
      aCrossover->combForm == adaptCmb)
    AdaptCross(newGen->Converge(), 
	       oldGen->fitValues[genSize-1], 
	       newGen->fitValues[genSize-1]);
  if ((aRepForm >= monotone))
    if (newGen->fitValues[genSize-1] < oldGen->fitValues[genSize-1]) {
      newGen->population[0] = new Individual(oldGen->population[genSize-1]);
      newGen->fitValues[0] = oldGen->fitValues[genSize-1];
      newGen->Sort();
    }
  return newGen;
}
