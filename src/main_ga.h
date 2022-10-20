/***************************************************************

   Project: MCG, implementation of the graph layout algorithm 
            using genetic algorithms.
   License: Creative Commons, Attribution
   Author:  Dana Vrajitoru
   File:    main_ga.h
   Update:  October 2022
   
   Some functions that should have been better organized than this,
   but for some reason still ended in the main file. Until I find the
   time and motivation to reorganize them.
  
***************************************************************/

#ifndef MAIN_GA_H
#define MAIN_GA_H

// Initialize the 3 parameters for runing the MCG trailing part at the
// end of tje GA part.
void InitMCG(char **theDict);

// Trailing MCG force-based run for a number of iterations after the
// GA.
void TrailMCG();

// Start the result file with a bunch of parameters for the run so we
// know pretty much how we obtained these.
void InitSaveResults();

// Execute one trial run of the GA for the specified number of
// generations.
void ExecuteOneCross();

// Run the GA for the given number of trials.
void ExecuteGA();

// Delete all dynamically allocated objects.
void FreeAll();

#endif
