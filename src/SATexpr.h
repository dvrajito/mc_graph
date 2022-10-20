/***************************************************************

   Project: MCG, implementation of the graph layout algorithm 
            using genetic algorithms.
   License: Creative Commons, Attribution
   Author:  Dana Vrajitoru
   File:    SATexpr.h
   Update:  October 2022

   A class handling SAT expressions.
   A SAT expression is a collection of clauses. 
   Each clause is a collection of terms or predicates.

***************************************************************/

#ifndef SATexpr_h
#define SATexpr_h

#include "SATclause.h"
#include <fstream>

class SATexpr {
public:
    int nrClauses;
    SATclause **clauses;

    ///////////////////////// Constructors /////////////////////

    // Constructor with given number of clauses
    SATexpr(int size = 0);

    // Copy constructor
    SATexpr(SATexpr *data);

    // Constructor reading the expression from a file
    SATexpr(ifstream &aFile);

    // Constructor reading the expression from a file
    SATexpr(char *fileName);

    // Empty the object: delete all the clauses
    void Empty();

    // Destructor: must delete all the clauses
    ~SATexpr();

    ////// Init functions corresponding to each constructor /////

    // Initialization with given number of clauses
    void Init(int size = 0);
    void Init(SATexpr *data);
    void Init(ifstream &aFile);
    void Init(char *fileName);

    // Input
    void Read();
    bool Read(ifstream &aFile);
    bool Read(char *fileName);

    // Output
    void Print();
    void FPrint(ofstream &aFile);
    void FPrint(char *fileName);
};

#endif
