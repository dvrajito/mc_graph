/***************************************************************

   Project: MC-Graph, a C++ implementation of genetic algorithms
            for graph drawing and visualization
   License: Creative Commons, Attribution
   Author:  Dana Vrajitoru
   File:    SATexpr.cc
   Update:  October 2022

   A class handling SAT expressions.
   A SAT expression is a collection of clauses. 
   Each clause is a collection of terms or predicates.

***************************************************************/

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
using namespace std;
#include "SATexpr.h"

/////////////////////////// Constructors ///////////////////////

// Constructor with given number of clauses
SATexpr::SATexpr(int size)
{
    nrClauses = 0;
    clauses = NULL;
    Init(size);
}

// Copy constructor
SATexpr::SATexpr(SATexpr *data)
{
    nrClauses = 0;
    clauses = NULL;
    Init(data);
}

// Constructor reading the expression from a file
SATexpr::SATexpr(ifstream &aFile)
{
    nrClauses = 0;
    clauses = NULL;
    Init(aFile);
}

// Constructor reading the expression from a file
SATexpr::SATexpr(char *fileName)
{
    nrClauses = 0;
    clauses = NULL;
    Init(fileName);
}

// Empty the object: delete all the clauses
void SATexpr::Empty()
{
    if (clauses != NULL) {
        for (int i = 0; i < nrClauses; i++)
            delete clauses[i];
        delete clauses;
        clauses = NULL;
    }
}

// Destructor: must delete all the clauses
SATexpr::~SATexpr()
{
    Empty();
}

//////// Init functions corresponding to each constructor ///////

// Initialization with given number of clauses
void SATexpr::Init(int size)
{
    if (clauses != NULL)
        Empty();
    nrClauses = size;
    if (nrClauses > 0) {
        clauses = new SATclause *[nrClauses];
        for (int i = 0; i < nrClauses; i++)
            clauses[i] = NULL;
    }
}

void SATexpr::Init(SATexpr *data)
{
    if (clauses != NULL)
        Empty();
    nrClauses = data->nrClauses;
    if (nrClauses > 0) {
        clauses = new SATclause *[nrClauses];
        for (int i = 0; i < nrClauses; i++)
            clauses[i] = new SATclause(data->clauses[i]);
    }
}

void SATexpr::Init(ifstream &aFile)
{
    if (clauses != NULL)
        Empty();
    Read(aFile);
}

void SATexpr::Init(char *fileName)
{
    if (clauses != NULL)
        Empty();
    Read(fileName);
}

////////////////////////////////// Input //////////////////////////

// Input from the console
void SATexpr::Read()
{
    cin >> nrClauses;
    clauses = new SATclause *[nrClauses];
    for (int i = 0; i < nrClauses; i++) {
        clauses[i] = new SATclause();
        clauses[i]->Read();
    }
}

// Input from a file. Returns true if succesful.
bool SATexpr::Read(char *fileName)
{
    ifstream aFile(fileName);

    if (aFile.good())
        return Read(aFile);
    else {
        cout << "Cannot read from the file:" << fileName << endl;
        return false;
    }
    aFile.close();
}

// Input from a file. Returns true if succesful.
bool SATexpr::Read(ifstream &aFile)
{
    if (aFile.good()) {
        aFile >> nrClauses;
        clauses = new SATclause *[nrClauses];
        for (int i = 0; i < nrClauses; i++)
            clauses[i] = new SATclause(aFile);
        return true;
    }
    else {
        cout << "Cannot read from the file" << endl;;
        return false;
    }
}

////////////////////////////// Output ///////////////////////////////////

// Output to the console
void SATexpr::Print()
{
    cout << "expression with " << nrClauses
        << "clauses:" << endl;
    for (int i = 0; i < nrClauses; i++)
        clauses[i]->Print();
}

// Output to a file
void SATexpr::FPrint(ofstream &aFile)
{
    if (aFile.good()) {
        aFile << nrClauses << endl;
        for (int i = 0; i < nrClauses; i++)
            clauses[i]->FPrint(aFile);
    }
    else {
        cout << "Cannot write to the file" << endl;
        Print();
    }
}

// Output to a file
void SATexpr::FPrint(char *fileName)
{
    ofstream aFile(fileName);
    if (aFile.good())
        FPrint(aFile);
    else {
        cout << "Cannot write to the file:" << fileName << endl;
        Print();
    }
    aFile.close();
}
