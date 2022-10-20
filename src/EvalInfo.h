/********************************************************************

   Project: MC-Graph, a C++ implementation of genetic algorithms
            for graph drawing and visualization
   License: Creative Commons, Attribution
   Author:  Dana Vrajitoru
   File:    EvalInfo.h
   Update:  October 2022

   A class handling the evaluation settings, such as if
   we have Boolean or real-valued genes, the specific
   fitness function and transformation used, and the number of
   variables encoded in a chromosome.

*********************************************************************/

#ifndef EvalInfo_h
#define EvalInfo_h

#include <fstream>
using namespace std;

class EvalInfo {
public:
    int realEval;
    int funcNumber;
    int transNumber;
    int pointNumber;
    char *probFileName;

    //////////////////////////// constructors //////////////////////////////

    // Default constructor
    EvalInfo();

    // Constructor from a file
    EvalInfo(FILE *aFile);

    // Constructor from a file
    EvalInfo(ifstream &aFile);

    // Constructor from a dictionary of (setting, value)
    EvalInfo(char **aDict);

    // Copy constructor
    EvalInfo(EvalInfo *data);

    // Constructor with values for all attributes
    EvalInfo(int isReal,
             int fNbr,
             int tNbr,
             int pNbr,
             char *aName);

    // desctructor 
    ~EvalInfo();

    /******************************* Initialize *****************************/

    // Initialize everything with default values
    void Init();

    // Initialize from a file
    void Init(FILE *aFile);

    // Initialize from a file
    void Init(ifstream &aFile);

    // Initialize from a dictionary of (setting, value)
    void Init(char **aDict);

    // Initialize everything as a copy from another object
    void Init(EvalInfo *data);

    // Initialize with values for all attributes
    void Init(int isReal,
              int fNbr,
              int tNbr,
              int pNbr,
              char *aName);

    /********************************* Input ******************************/

    // Input from the console
    void ScanEvalInfo();
    
    // Input an option from the console
    int OptionEvalInfo(char anOption);

    // Input one attribute from the console
    int AskEvalInfo();

    /******************************** Output ******************************/

    // Output to the console
    void Print();

    // Output to a file
    void FPrint(FILE *aFile);

    // Output to a file
    void FPrint(ofstream &fout);
};

#endif
