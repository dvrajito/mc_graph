/***************************************************************

   Project: MC-Graph, a C++ implementation of genetic algorithms
            for graph drawing and visualization
   License: Creative Commons, Attribution
   Author:  Dana Vrajitoru
   File:    SATclause.cc
   Update:  October 2022

   A class handling a SAT clause, part of a SAT expression.
   A SAT expression is a collection of clauses. 
   Each clause is a collection of terms or predicates.

***************************************************************/

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
using namespace std;
#include "SATclause.h"

// Constructor with number of terms (size) and initial value for each term
SATclause::SATclause(int size, int val)
    : terms(NULL), nrTerms(0)
{
    Init(size, val);
}

// Copy constructor
SATclause::SATclause(SATclause *data)
    : terms(NULL), nrTerms(0)
{
    Init(data);
}

// Constructor reading the clause from a file
SATclause::SATclause(ifstream &aFile)
    : terms(NULL), nrTerms(0)
{
    Init(aFile);
}

// Empty the objects: delete the terms
void SATclause::Empty()
{
    if (terms != NULL)
        delete terms;
    terms = NULL;
    nrTerms = 0;
}

// Destructor: must delete terms
SATclause::~SATclause()
{
    Empty();
}

// Init the terms array based on the size and each term with the value
void SATclause::Init(int size, int val)
{
    Empty();
    nrTerms = size;
    if (size > 0) {
        terms = new int[size];
        for (int i = 0; i < size; i++)
            terms[i] = val;
    }
}

// Copy initialization
void SATclause::Init(SATclause *data)
{
    Empty();
    Init(data->nrTerms);
    for (int i = 0; i < nrTerms; i++)
        terms[i] = data->terms[i];
}

// Initialize the clause from a file
void SATclause::Init(ifstream &aFile)
{
    Empty();
    Read(aFile);
}

// Input from the console
void SATclause::Read()
{
    Empty();
    cin >> nrTerms;
    terms = new int[nrTerms];
    for (int i = 0; i < nrTerms; i++)
        cin >> terms[i];
}

// Input from a file
bool SATclause::Read(ifstream &aFile)
{
    if (aFile.good()) {
        Empty();
        aFile >> nrTerms;
        terms = new int[nrTerms];
        for (int i = 0; i < nrTerms && aFile.good(); i++)
            aFile >> terms[i];
        return aFile.good(); // return true if we were alble to read everything from the file

    }
    else 
        return false;
}

// Output to the console
void SATclause::Print()
{
    cout << "Clause of size" << nrTerms << ": ";
    for (int i = 0; i < nrTerms; i++)
        cout << terms[i] << ' ';
    cout << endl;
}

// Output to a file. 
// Assuming that we don't have to read this on its own, only part of an expression.
void SATclause::FPrint(ofstream &aFile)
{
    aFile << nrTerms << ' ';
    for (int i = 0; i < nrTerms; i++)
        aFile << terms[i] << ' ';
    aFile << endl;
}
