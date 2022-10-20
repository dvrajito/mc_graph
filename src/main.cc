/***************************************************************

   Project: MC-Graph, a C++ implementation of genetic algorithms
            for graph drawing and visualization
   License: Creative Commons, Attribution
   Author:  Dana Vrajitoru
   File:    main.cc
   Update:  October 2022

   The main function for the genetic algorithms and some helping
   functions.

***************************************************************/

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
using namespace std;
#include <ctime>
#include "main.h"
#include "GAManager.h"

// Main function, entry point to the program.
int main(int argc, char **argv)
{
    MainOption option = gaMain;

    switch (option) {
    case gaMain:
        mainGA();
        break;
    case mcgMain:
        return mainMCG(argc, argv);
        // break;
    case visualMain:
        return mainVisual(argc, argv);
        // break;
    case wienerMain:
        return mainWiener(argc, argv);
        // break;
    }

    return 0;
}


// Main function for the genetic algorithm version.
void mainGA()
{

    time_t sTime, eTime;
    GAManager GAM;

    // init files and settings
    GAM.GAInit();
    GAM.InitialPopulation();

    // Run the GA and time it
    time(&sTime);
    GAM.ExecuteGA();
    time(&eTime);
    cout << "total time:" << eTime - sTime << endl;
}









