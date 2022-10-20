/***************************************************************

   Project: MCG, implementation of the graph layout algorithm 
            using genetic algorithms.
   License: Creative Commons, Attribution
   Author:  Dana Vrajitoru
   File:    SATclause.h
   Update:  October 2022

   A class handling a SAT clause, part of a SAT expression.
   A SAT expression is a collection of clauses. 
   Each clause is a collection of terms or predicates.

***************************************************************/

#ifndef SATclause_h
#define SATclause_h

#include <fstream>
class SATclause {
public:
    int nrTerms;
    int *terms;

    // Constructor with number of terms (size) and initial value for each term
    SATclause(int size = 0, int val = 0);

    // Copy constructor
    SATclause(SATclause *data);

    // Constructor reading the clause from a file
    SATclause(ifstream &aFile);

    // Empty the objects: delete the terms
    void Empty();

    // Destructor: must delete terms
    ~SATclause();

    // Init functions corresponding to each constructor

    // Init the terms array based on the size and each term with the value
    void Init(int size = 0, int val = 0);

    // Copy initialization
    void Init(SATclause *data);

    // Initialize the clause from a file
    void Init(ifstream &aFile);

    // Input from the console
    void Read();

    // Input from a file
    bool Read(ifstream &aFile);

    // Output to the console
    void Print();

    // Output to a file
    void FPrint(ofstream &aFile);
};

#endif
