/********************************************************************

   Project: GADX, a C++ implementation of genetic algorithms
   License: Creative Commons, Attribution
   Author:  Dana Vrajitoru
   Updated: March 2019

   Type definitions related to genetic operations

*********************************************************************/

#ifndef GenTypes_h
#define GenTypes_h

#include "GenInfo.h"
#include "List.h"
#include "Individual.h"

struct PolynomStr {
    double *coefs;
    int degree;
};

typedef struct PolynomStr Polynom;

typedef enum {
    cnfAve, dnfAve, cnfPnorm,
    dnfPnorm, hcAve, hcPnorm
} SATevalForm;

struct GraphStr {
    List *edges;
    int *startEdge, *endEdge, *nrEdge, *revEdge;
    int size, edgeNr;
};

typedef struct GraphStr Graph;

#endif
