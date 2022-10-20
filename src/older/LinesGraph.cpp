#include <stdlib.h>
#include "GenTypes.h"
#include "LinesGraph.h"


static Graph *aGraph = NULL;
static int *randArcs;


int RealArc(int anArc)
{
  if (anArc > 0)
    return anArc;
  else
    return (-(arc+1));
}

int ArcStart(int anArc)
{
  if (anArc > 0)
    return aGraph->startArc[anArc];
  else
    return aGraph->endArc[-(anArc+1)];
}

int ArcEnd(int anArc)
{
  if (anArc > 0)
    return aGraph->endArc[anArc];
  else
    return aGraph->startArc[-(anArc+1)];
}

void SwapList(List *l1, List *l2)
{ List aux;

  aux = *l1;
  *l1 = *l2;
  *l2 = aux;
}

List SplitList(List *aList, int where)
{ List list2 = *aList, aux;
  int i;

  for (i=0; (i<where) && (list2 != NULL); i++)
    list2 = list2->next;
  if (list2 != NULL) {
    Swap(&(list2->prec), &((*aList)->prec));
    if (*aList->prec != NULL)
      *aList->prec->next = NULL;
  }
  return list2;
}

void ChangeSign(List *aList)
{ List where = *aList;

  while (where != NULL) {
    where->elem = -(where->elem + 1);
    where = where->next;
  }
}

ReverseList(List *aList)
{ List where, locL = *aList;

  ChangeSign(aList);
  if (locL != NULL) {
    locL->pred->next = locL;
    where = locL;
    while (where->next != locL) {
      SwapList(&(where->pred), &(where->next));
      where = where->pred;
    }
    locL->next = NULL;
    *aList = where;
  }
}

void MergeLists(List *list1, List *list2)
{
  *list1->pred->next = *list2;
  SwapList(&(*list1->pred), &(*list2->pred));
  *list2 = NULL;
}

void PrintList(List aList)
{ List locL = aList;

  while (locL != NULL) {
    printf("%d ", locL->elem);
    locL = locL->next;
  }
  printf("\n");
}

void FPrintList(FILE *aFile, List aList)
{ List locL = aList;

  while (locL != NULL) {
    fprintf(aFile, "%d ", locL->elem);
    locL = locL->next;
  }
  fprintf(aFile, "\n");
}

void FreeGraph(Graph **aGraph)
{ int i;

  for (i=0; i<(*aGraph)->nodeNr; i++)
    FreeList((*aGraph)->arcs[i]);
  free((*aGraph)->arcs);
  free((*aGraph)->startArc);
  free((*aGraph)->endArc);
  free((*aGraph)->nrArc);
  free((*aGraph)->revArc);
  free(*aGraph);
  *aGraph = NULL;
}

void FreeTheGraph()
{
  FreeGraph(&aGraph);
  free(randArcs);
}

void ReadGraph(EvalInfo *anEvalInfo)
{ FILE *graphFile;
  int i, j, arc1, arc2r;
  char arcName[10];
 
  graphFile = fopen(anEvalInfo->aFileName);
  if (graphFile != NULL) {
    aGraph = (Graph *) malloc(sizeof(Graph));
    fscanf(graphFile, "%d", &(aGraph->nodeNr));
    fscanf(graphFile, "%s", arcName);
    fscanf(graphFile, "%d", &(aGraph->arcNr));
    fscanf(graphFile, "%s", arcName);
    aGraph->nrArc = (int *) malloc((aGraph->nodeNr)*sizeof(int));
    aGraph->startArc = (int *) malloc((aGraph->arcNr)*sizeof(int));
    aGraph->endArc = (int *) malloc((aGraph->arcNr)*sizeof(int));
    aGraph->arcs = (List *) malloc(aGraph->nodes*sizeof(List));
    aGraph->revArc = (List *) malloc(aGraph->nodes*
				     aGraph->nodes*sizeof(List));
    for (i=0; i<aGraph->nodeNr; i++) {
      aGraph->nrArc[i] = 0;
      aGraph->arcs[i] = NULL;
      for (j=0; j<aGraph->nodeNr; j++)
	aGraph->revArc[i*aGraph->nodeNr+j]=-1;
    }
    for (i=0; i<aGraph->arcNr; i++) {
      fscanf(graphFile, "%s", arcName);
      if (arcName[0] >= 'A') {
	arc1 = arcName[0] - 'A';
	arc2 = arcName[1] - 'A';
      }
      else {
	arc1 = arcName[0] - 'a';
	arc2 = arcName[1] - 'a';
      }
      aGraph->arcs[arc1] = AddElem(arc2, aGraph->arcs[arc1]);
      aGraph->startArc[i] = arc1;
      aGraph->endArc[i] = arc2;
    }
    fclose(graphFile);
    randArcs = (int *) malloc((aGraph->arcNr)*sizeof(int));
  }
}

void RandGraphInd(Individual *anInd)
{ int i, arcInd, j;

  anInd->chromosome = (int *)malloc((aGraph->arcNr+1) * sizeof(int));
  anInd->paths = (List *)malloc((aGraph->arcNr) * sizeof(List));
  anInd->chromosome[0] = aGraph->arcNr;
  for (i=0; i<aGraph->arcNr; i++) {
    anInd->chromosome[i+1] = 1;
    randArcs[i] = i;
    anInd->paths[i] = NULL;
  }
  for (i=0; i<aGraph->arcNr; i++) {
    arcInd = rand() % (aGraph->arcNr - i);
    AddElem(arcInd, anInd->paths+i);
    for (j=arcInd; j < aGraph->arcNr-i-1; j++)
      randArcs[j] = randArcs[j+1];
}

int PathNumber(Individual *anInd)
{
  return anInd->chromosome[0];
}

int PathStart(Individual *anInd, int aPath)
{
  return anInd->paths[aPath];
}

int PathEnd(Individual *anInd, int aPath)
{
  return Last(anInd->paths[aPath]);
}

int PathSize(Individual *anInd, int aPath)
{
  return anInd->chromosome[i+1];
}

int IndexArcs(Individual *anInd, int **result)
{ int *present, arcs = aGraph->nrArc, i, j;
  List where;

  present = (int *)malloc(arcs*sizeof(int));
  for (i=0; i<arcs; i++)
    present[i] = 0;
  for (i=0; i<PathNumber(anInd); i++) {
    where = anInd->paths[i];
    while (where != NULL)
      if (!present[RealArc(where->elem)]) {
	arcs--;
	present[RealArc(where->elem)] = 1;
      }
  }
  if (arcs > 0) {
    *result = (int *)malloc(arcs*sizeof(int));
    j = 0;
    for (i=0; i<arcs; i++) {
      while (!present[j])
	j++;
      (*result)[i] = j++;
    }
  }
  return arcs;
}

void AddArcs(Individual *anInd, int nrArcs, int *theArcs)
{ int nrPath, i;

  nrPath = PathNumber(anInd);
  anInd->chromosome = realloc((nrPath + nrArcs + 1) * sizeof(int));
  for (i=nrPath+1; i<nrPath + nrArcs + 1; i++)
    anInd->chromosome[i] = 1;
  anInd->chromosome[0] += nrArcs;
  anInd->paths = realloc((nrPath + nrArcs) * sizeof(List));
  for (i=nrPath; i<nrPath + nrArcs; i++) {
    anInd->paths[i] = NULL;
    AddElem(anInd->paths + i, theArcs[i - nrPath]);
}

void CompleteInd(Individual *anInd)
{ int nrArcs, *theArcs;

  nrArcs = IndexArcs(anInd, &theArcs);
  AddArcs(anInd, nrArcs, theArcs);
}

void DeletePath(Individual *anInd, int aPath)
{ int i, pNr = PathNumber(anInd);

  if (anInd->paths[aPath] != NULL)
    FreeList(anInd->paths + aPath);
  for (i=aPath+1; i< pNr; i++)
    anInd->chromosome[i] = anInd->chromosome[i+1];
  anInd->chromosome = realloc(pNr*sizeof(int));
  for (i=aPath; i<pNr-1; i++)
    anInd->paths[i] = anInd->paths[i+1];
  anInd->paths = realloc((pNr-1)*sizeof(List));
  anInd->chromosome[0]--;
}

int CheckArcInclude(Individual *anInd, int aPath, int anArc)
{ int included = 0, rArc = RealArc(anArc);
  List where;

  where = PathStart(anInd, aPath);

  while ((where != NULL) && !included)
    if (rArc == RealArc(where->elem))
      included = 1;
    else
      where = where->next;
  return included;
}

int CheckPathInclude(Individual *anInd, int path0, int path1)
{ int psize0, psize1, included = 1, deleted;
  List where;

  psize0 = PathSize(anInd, path0);
  psize1 = PathSize(anInd, path1);
  if (psize0 < psize1) {
    where = PathStart(anInd, path0);
    while ((where != NULL) && included) {
      included = CheckArcInclude(anInd, path1, where->elem);
      if (included) {
	DeleteFirst(&where);
	anInd->chromosome[path0]--;
      }
    }
    if (where != NULL) {
      included = 1;
      where = PathEnd(anInd, path0);
      while ((where != NULL) && included) {
	included = CheckArcInclude(anInd, path1, where->elem);
	if (included) {
	  DeleteLast(&where);
	  anInd->chromosome[path0]--;
	}
      }
    }
    anInd->paths[path0] = where;
    if (deleted = (where == NULL))
      DeletePath(anInd, path0);
    return deleted;
  }
  else
    return - CheckPathInclude(anInd, path1, path0);
}

void CheckIncludes(Individual *anInd)
{ int i, j, incl, aSt, anEnd, del;
  
  for (i=0; i<PathNumber(anInd) - 1; i++)
    for (j=i+1; j<PathNumber(anInd);) {
      incl = CheckPathInclude(anInd, i, j);
      if (incl < 0)
	j=i+1;
      else if (incl == 0)
	j++;
    }
}

int CheckPathMerges(Individual *anInd, int path0, int path1, int recursion)
{ int arc0, arc1, deleted = 0;

  arc0 =  PathEnd(andInd, path0)->elem;
  arc1 =  PathStart(andInd, path1)->elem;
  arc2 =  PathEnd(andInd, path1)->elem;
  if (ArcEnd(arc0) == ArcStart(arc1)) {
    if (arc0*arc1 < 0)
      ChangeSign(anInd->paths+path1);
    MergeLists(anInd->paths+path0, anInd->paths+path1);
    DeletePath(anInd, path1);
    deleted = 1;
  }
  else if (ArcEnd(arc0) == ArcEnd(arc2)) {
    if (arc0*arc2 > 0)
      ChangeSign(anInd->paths+path1);
    ReverseList(anInd->paths+path1);
    MergeLists(anInd->paths+path0, anInd->paths+path1);
    DeletePath(anInd, path1);
    deleted = 1;
  }
  else if (recursion)
    deleted = -CheckPathMerges(anInd, path1, path0, 0);
  return deleted;
}

void CheckMerges(Individual *anInd)
{ int i, j, incl, aSt, anEnd, del;
  
  for (i=0; i<PathNumber(anInd) - 1; i++)
    for (j=i+1; j<PathNumber(anInd);) {
      incl = CheckPathMerges(anInd, i, j, 1);
      if (incl < 0)
	j=i+1;
      else if (incl == 0)
	j++;
    }
}

void ReduceInd(Individual *anInd)
{ 
  CheckIncludes(Individual *anInd);
  CheckMerges(Individual *anInd);
}
