/***************************************************************

   Project: MCG, implementation of the graph layout algorithm 
            using genetic algorithms.
   License: Creative Commons, Attribution
   Author:  Dana Vrajitoru
   File:    SettingFile.h
   Updated: October 2022

   Reading the information from the setting file.

****************************************************************/

#ifndef SettingFile_h
#define SettingFile_h

// Read the file "GAPrefs.txt" and return it as a "dictionary"
// which is an array of strings, explanation followed by value.
char **ReadDictionary();

// Initialize the settings of the genetic algorithm from 
// the settings file.
void InitSettings();

#endif
