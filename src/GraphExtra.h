/********************************************************************

   Project: MC-Graph, a C++ implementation of genetic algorithms
			for graph drawing and visualization
   License: Creative Commons, Attribution
   Author:  Dana Vrajitoru
   File:    GraphExtra.h
   Updated: October 2022

   Helper functions for the Graph class.

*********************************************************************/

#ifndef GRAPH_EXTRA_H
#define GRAPH_EXTRA_H

// Convert a couple of names like AB or AaBb into vertex numbers.
void EdgeConvert(const char *input, int &j, int &k);

// Converts a name given as one uppercase letter into a vertex number.
int Convert1Char(char c);

// Converts a name given as 2 characters into a vertex number. The
// convention is that the first character is an uppercase and the
// second one is a lowercase letter. This covers graphs up to 675
// vertices.
int Convert2Char(char c1, char c2);

// Some helping function for printing the graph.
void PreliminaryProcess(int v);

#endif
