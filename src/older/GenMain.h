#ifndef GenMain_h
#define GenMain_h

#include "Generation.h"
#include "RunInfo.h"
#include "EvalInfo.h"

Generation *Reproduce(Generation *oldGen,                      
                      CrossMethod *aCrossover,                      
                      RepForm aRepForm,                      
                      EvalInfo *anEval,                      
                      Generation *whereGen);

Generation *GARun(Generation *oldGen, 
                  RunInfo *aRInfo,                  
                  EvalInfo *aEInfo);

#endif
