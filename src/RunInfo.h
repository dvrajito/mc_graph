/*******************************************************************

   Project: MCG, implementation of the graph layout algorithm 
            using genetic algorithms.
   License: Creative Commons, Attribution
   Author:  Dana Vrajitoru
   File:    RunInfo.h
   Update:  October 2022

   A class handling some run-time settings such as generation number,
   random seed, number of trials, and others.

********************************************************************/

#ifndef RunInfo_h
#define RunInfo_h

#include "CrossMethod.h"

typedef enum {
    simple, elitist, monotone,
    sexuate, sexBinary, herma,
    diverse
} RepForm;

class RunInfo {
public:
    int genSize;
    int genNumber;
    int history;
    int rseed;
    int trialNr;
    int reportNr;
    char *hFileName;
    char *resFileName;
    RepForm aRForm;
    CrossMethod *theCross;

    /******************* constructors *****************************/

    // Default constructor
    RunInfo();

    // Constructor from a file
    RunInfo(FILE *aFile);

    // Constructor from a dictionary of (setting, value).
    RunInfo(char **aDict);

    // Copy constructor
    RunInfo(RunInfo *data);

    // Constructor with values for all attributes
    RunInfo(int aSize,
            int aNbr,
            int isHistory,
            int aRseed,
            int aTrialNr,
            int aReportNr,
            char *aFileName,
            char *rFileName,
            RepForm aForm,
            CrossMethod *aCross);

    /* desctructor */
    ~RunInfo();

    /*************************** Initialize ************************/

    // Default initialization
    void Init();

    // Initialize the information from a file
    void Init(FILE *aFile);

    // Initialize the information from a dictionary of (setting, value).
    void Init(char **aDict);

    // Copy initialization
    void Init(RunInfo *data);

    // Initialization with values for all attributes
    void Init(int aSize,
              int aNbr,
              int isHistory,
              int aRseed,
              int aTrialNr,
              int aReportNr,
              char *aFileName,
              char *rFileName,
              RepForm aForm,
              CrossMethod *aCross);
    
    /**************************** Input *****************************/

    // Input all the information from the console
    void ScanRunInfo();

    // Input a setting from the console
    int OptionRunInfo(char anOption);

    // Output all the settings and ask if the user wants to change any of them.
    int AskRunInfo();

    /**************************** Output *************************/

    // Output the settings to the console
    void Print();

    // Output the settings to a file
    void FPrint(FILE *aFile);

    // Return the reproduction form as a string
    char *RepString();

};

#endif
