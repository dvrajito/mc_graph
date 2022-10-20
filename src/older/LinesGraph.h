#ifndef LinesGraph_h
#define LinesGraph_h

List AddElem(int anElem, List aList);

void FreeList(List *aList);

void PrintList(List aList);

void FPrintList(FILE *aFile, List aList);

void FreeTheGraph();

void ReadGraph(EvalInfo *anEvalInfo);

void RandGraphInd(Individual *anInd);

int PathNumber(Individual *anInd);

int PathStart(Individual *anInd, int aPath);

int PathEnd(Individual *anInd, int aPath);

int PathSize(Individual *anInd, int aPath);

void DeletePath(Individual *anInd, int aPath);

List SplitList(List *aList, int where);

void CompleteInd(Individual *anInd);

void ReduceInd(Individual *anInd);

#endif
