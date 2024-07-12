/*******************************************************************

   Project: MC-Graph, a C++ implementation of genetic algorithms
            for graph drawing and visualization
   License: Creative Commons, Attribution
   Author:  Dana Vrajitoru
   File:    common.h
   Update:  July 2024

   Helper function dealing with the output file.

*******************************************************************/

#ifndef COMMON_MCG_H
#define COMMON_MCG_H

// Outputs an integer to the result file on a line preceded by a line
// with a comment given by the parameter name.
void out_int(const char *name, int val);

// Outputs a real number to the result file on a line preceded by a
// line with a comment given by the parameter name.
void out_float(float val);

// Writes a new line to the output file.
void out_endl();

// Outputs a string on a line in the result file.
void out_str(const char *name);

// Copies a file if it already exists such that the new results are
// added to it.
void my_copy_file(const char *name);

#endif
