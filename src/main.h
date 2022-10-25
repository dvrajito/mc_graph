/***************************************************************

   Project: MC-Graph, a C++ implementation of genetic algorithms
            for graph drawing and visualization
   License: Creative Commons, Attribution
   Author:  Dana Vrajitoru
   File:    main.h
   Update:  October 2022

   The main functions for different versions of running the 
   project.

***************************************************************/

#ifndef MAIN_GA_H
#define MAIN_GA_H

enum MainOption { gaMain, mcgMain, visualMain, wienerMain };

// Main function for the genetic algorithm version.
void mainGA();

// Initialize the 3 parameters for runing the MCG trailing part at the
// end of tje GA part.
void InitMCG(char **theDict);

// Main function for the MC Graph without GA version of the project.
int mainMCG(int argc, char* argv[]);

// The main function in the Wiener version of the program.
int mainWiener(int argc, char** argv);

// The main for the visualization of the results.
int mainVisual(int argc, char** argv);

// The main for graph testing
int mainGraph();

#endif
