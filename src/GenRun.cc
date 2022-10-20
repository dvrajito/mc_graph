#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <iostream.h>
#include <time.h>
#include "General.h"
#include "RunInfo.h"
#include "EvalInfo.h"
#include "GenInfo.h"
#include "GenOper.h"
#include "GenEval.h"
//#include "EvalBool.h"
#include "Individual.h"
#include "Generation.h"
#include "PyCommunicate.h"

//extern SATexpr *globalSAT;

int randPrecision = 1000;
char fileName[15] = "GAPrefs.txt";
FILE *gaFile, *resFile = NULL, *histFile = NULL;
unsigned int randSeed = 201;
GenInfo *aGenInfo;
RunInfo *aRunInfo;
EvalInfo *anEvalInfo;
Generation *initPop;
int dirty = 0, i, j, nTrials, nInterm;
double res0;
long int sTime, eTime;

void ReadData()
{
  gaFile = fopen(fileName, "r");
  aGenInfo = new GenInfo(gaFile);
  dirty += aGenInfo->AskInfo();
  aRunInfo = new RunInfo(gaFile);
  dirty += aRunInfo->AskRunInfo();
  anEvalInfo = new EvalInfo(gaFile);
  dirty += anEvalInfo->AskEvalInfo();
  fclose(gaFile);
  //  ReadSATexpr(anEvalInfo->aFileName);
}

void SaveData()
{
  gaFile = fopen(fileName, "w");
  aGenInfo->FPrint(gaFile);
  aRunInfo->FPrint(gaFile);
  anEvalInfo->FPrint(gaFile);
  fclose(gaFile);
}

void InitSaveResults()
{ 
  char *resName;

  resName = aRunInfo->resFileName;
  if (resName[0] != '0') {
    resFile = fopen(resName, "w");
    fprintf(resFile, "random seed\tpopulation size\tindividual size\t");
    fprintf(resFile, "pc\tpm\tgeneration number\n");
    fprintf(resFile, "%d\t\t%d\t\t%d\t\t%g\t%g\t%d\n", 
	    randSeed, aRunInfo->genSize, aGenInfo->indSize,
	    aGenInfo->pc, aGenInfo->pm, aRunInfo->genNumber);
  }
  if (aRunInfo->history) {
    if ((aRunInfo->hFileName == NULL) ||
	(aRunInfo->hFileName[0] == '\0') ||
	(aRunInfo->hFileName[0] ==' ')) {
      cout << "history filename" << endl;
      cin >> resName;
      histFile = fopen(resName, "a");
    }
    else
      histFile = fopen(aRunInfo->hFileName, "w");
    fprintf(histFile, "random seed\tpopulation size\tindividual size\t");
    fprintf(histFile, "pc\tpm\tgeneration number\n");
    fprintf(histFile, "%d\t\t%d\t\t%d\t\t%g\t%g\t%d\n", 
	    randSeed, aRunInfo->genSize, aGenInfo->indSize,
	    aGenInfo->pc, aGenInfo->pm, aRunInfo->genNumber);
  }
  else
    histFile=NULL;
}

void ExecuteOneCross()
{ 
  int j;

  aRunInfo->theCross->Print();
  randSeed = aRunInfo->rseed;
  fprintf(resFile, "\ncrossover form = %s", 
	  aRunInfo->theCross->MethodString());
  if (aRunInfo->history)
    fprintf(histFile, "\ncrossover form = %s\n", 
	    aRunInfo->theCross->MethodString());
  for (j=0; j<nTrials; j++) {
    srand(randSeed);
    initPop->ReInit(1);
    EvalGen(initPop, anEvalInfo);
    initPop->Sort();
    fprintf(resFile, "\n");
    fprintf(resFile, "%f\t" , initPop->fitValues[initPop->popSize - 1]);
    randSeed = randSeed + 25;
    if (aRunInfo->history)
      fprintf(histFile, "\nTrial number %d\n", j);
    printf("Trial number %d\n", j);
    initPop->GARun(aRunInfo, anEvalInfo);
    fclose(resFile);
    resFile = fopen(aRunInfo->resFileName, "a");
  }
}

void ExecuteGA()
{ 
  int i, k;

  InitFitArray(aRunInfo->genSize);
  if (aRunInfo->theCross->theCrossover == allCrs)
    for (i=onePointCrs; i<allCrs; i++) {
      aRunInfo->theCross->theCrossover = (enum CrossForm) i;
      if ((i == dissocCrs) && (aRunInfo->theCross->dissForm == allOp))
	for (k=zeroOp; k<allOp; k++) {
	  aRunInfo->theCross->dissForm = (enum DissocForm) k;
	  ExecuteOneCross();
	}
      else if ((i == combineCrs) && 
	       (aRunInfo->theCross->combForm == allCmb))
	for (k=perfCmb; k<allCmb; k++) {
	  aRunInfo->theCross->theCrossover = combineCrs;
	  aRunInfo->theCross->combForm = (enum CombineForm) k;
	  ExecuteOneCross();
	}
      else
	ExecuteOneCross();
    }
  else 
    if ((aRunInfo->theCross->theCrossover == dissocCrs) 
	&& (aRunInfo->theCross->dissForm == allOp))
      for (k=zeroOp; k<allOp; k++) {
	aRunInfo->theCross->dissForm = (enum DissocForm) k;
	ExecuteOneCross();
      }
    else if ((aRunInfo->theCross->theCrossover == combineCrs) && 
	     (aRunInfo->theCross->combForm == allCmb))
      for (k=perfCmb; k<allCmb; k++) {
	aRunInfo->theCross->theCrossover = combineCrs;
	aRunInfo->theCross->combForm = (enum CombineForm) k;
	ExecuteOneCross();
      }
    else
      ExecuteOneCross();
  fprintf(resFile, "\n\n");
  fclose(resFile);
  if (aRunInfo->history) {
    fprintf(histFile, "\n\n");
    fclose(histFile);
  }
}

void FreeAll()
{
  delete initPop;
  delete aGenInfo;
  delete aRunInfo;
  delete anEvalInfo;
  //  if (globalSAT != NULL) {
  //    delete globalSAT;
  //    globalSAT = NULL;
  //  }
  dirty = 0;
}

void GenRun()
{ 
  InitGA();
  //  if ((anEvalInfo->aFileName != NULL) &&
  //      (anEvalInfo->aFileName[0] != '\0') &&
  //      (anEvalInfo->aFileName[0] != ' '))
  //    ReadSATexpr(anEvalInfo->aFileName);
  randSeed = aRunInfo->rseed;
  srand(randSeed);
  nTrials = aRunInfo->trialNr;
  nInterm = aRunInfo->reportNr;
  initPop = new Generation(aRunInfo->genSize, aGenInfo, 1);
  //initPop->Print("old");
  InitSaveResults();
  time(&sTime);
  ExecuteGA();
  //FreeAll();
  time(&eTime);
  cout << "total time:" << eTime-sTime << endl;
}










