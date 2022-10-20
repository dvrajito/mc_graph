/********************************************************************

   Project: MC-Graph, a C++ implementation of genetic algorithms
            for graph drawing and visualization
   License: Creative Commons, Attribution
   Author:  Dana Vrajitoru
   File:    CrossMethod.h
   Update:  October 2022

   A class handling some of the crossover operator settings.

*********************************************************************/

#ifndef CrossMethod_h
#define CrossMethod_h

#include <fstream>

enum DissocForm { zeroOp, andOp, xorOp, randOp, allOp } ;

enum CombineForm {
    perfCmb,
    eqCmb,
    RR50Cmb,
    RR100Cmb,
    adaptCmb,
    allCmb
} ;

enum CrossForm {
    onePointCrs = 0,
    nPointCrs,
    uniformCrs,
    dissocCrs,
    combineCrs,
    allCrs,
    graphCrs
} ;

class CrossMethod
{
public:
    CrossForm theCrossover;
    DissocForm dissForm;
    CombineForm combForm;
    int nPT;
    double thePswap;

    /////////////////////////// constructors ///////////////////////////////

    // Default constructor
    CrossMethod();

    // Constructor reading the attributes from a file
    CrossMethod(FILE *aFile);

    // Constructor reading the attributes from a file
    CrossMethod(ifstream &fin);

    // Constructor reading the attributes from a dictionary of seeting, value
    CrossMethod(char **aDict);

    // Copy constructor
    CrossMethod(CrossMethod *data);

    // Constructor with values for all attributes
    CrossMethod(CrossForm aCrossForm,
                DissocForm aDissForm,
                CombineForm aCombForm,
                int nPoints,
                double aProb);

    /* destructor */
    ~CrossMethod();

    /******************************* Initialize *****************************/

    // Initialize everything with default values
    void Init();

    // Initialize from a file
    void Init(FILE *aFile);

    // Initialize from a file
    void Init(ifstream &fin);

    // Initialize from a dictionary of seeting, value
    void Init(char **aDict);

    // Initialize everything as a copy from another object
    void Init(CrossMethod *data);

    // Initialize with values for all attributes
    void Init(CrossForm aCrossForm,
              DissocForm aDissForm,
              CombineForm aCombForm,
              int nPoints,
              double aProb);

    // Input from the console
    void ScanMethod();

    /******************************* Output *********************************/

    // Output to the console
    void Print();

    // Output to a file
    void FPrint(FILE *aFile);

    // Output to a file
    void FPrint(ofstream &fout);

    // Convert the crossover form to a string
    char *MethodString();

    // Convert the for of the dissociated crossover to a string
    char *DissFormStr();

    // Convert the crossover combination mode to a string
    char *CombFormStr();

    // Genetic operator: change the crossover operator in a round robin way
    // based on the generation number. 
    void RoundRobin(int aGen);
};

#endif
