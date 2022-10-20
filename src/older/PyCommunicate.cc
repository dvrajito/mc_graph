/* Dana Vrajitoru 
   GADissoc project
   PyCommunicate.cc
*/

#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <fstream>
using namespace std;
#include <iomanip>
#include "GenInfo.h"
#include "RunInfo.h"
#include "EvalInfo.h"
#include "General.h"
#include "EvalReal.h"
#include "GenEval.h"
#include "EvalBool.h"
#include "LinesGraph.h"
#include "PyCommunicate.h"
#include "main_ga.h"

extern GenInfo *aGenInfo;
extern RunInfo *aRunInfo;
extern EvalInfo *anEvalInfo;

char **ReadDictionary()
{
  char **result, rline[80];
  char fileName[] = "GAPrefs.txt";
  ifstream finput(fileName);
  int i, j=0;
 
  result = new (char *)[2*OPT_NR];
  for (i=0; i<80; i++)
    rline[i]='\0';
  for (i=0; i<2*OPT_NR; i++) {
    finput.getline(rline, 80);
    j = strlen(rline);
    if (j && rline[j-1] == '\n') {
      rline[j-1] = '\0';
      j--;
    }
    result[i] = new char[j+1];
    strcpy(result[i], rline);
    rline[0]='\0';
  }
  finput.close();
  printf("dictionary read\n");
  return result;
}

void InitGA()
{ 
  char **theDict=ReadDictionary();
  aGenInfo = new GenInfo(theDict);
  aGenInfo->Print();
  aRunInfo = new RunInfo(theDict);
  aRunInfo->Print();
  anEvalInfo = new EvalInfo(theDict);
  cout << "eval info read" << endl;
  anEvalInfo->Print();
  InitMCG(theDict);
}
