/***************************************************************

   Project: MCG, implementation of the graph layout algorithm 
            using genetic algorithms.
   License: Creative Commons, Attribution
   Author:  Dana Vrajitoru
   File:    result_manip.h
   Update:  October 2022
   
   Functions to manipulate result files for visualization.
  
***************************************************************/

#ifndef RESULT_MANIP_H
#define RESULT_MANIP_H

#include "Graph.h"

// Finds the file to visualize either from argv (argument to the
// application), or by asking the user for the file name, detects if
// it's a graph file or a result file, then reads the graph from it
// and returns a pointer to the graph object. It returns the parameter
// resf as true if it's a result file, or false if it's a graph file.
Graph *Detect_read_graph(int argc, char **argv, 
			 char *resultFile, bool &resf);

// Extracts the original layout from a graph file. It skips the graph
// altogether and only reads the information at the end. If there is
// no layout, it will simply generate the points on a circle.
void Read_gr_layout(Point points[], int size, char *graphFile);

// Reads the first layout that it finds in the result file. Will be
// updated later to select a particular layout.
void Read_res_layout(Point points[], int size, char *resFile);

// Reads the a layout from the result file based on a number it
// receives as a parameter.
void Read_res_select(Point points[], int size, char *resFile, int which);

// Generates a number of points on a unit circle in the (xy) plane,
// z=0.
void Generate_circle(Point points[], int size);

#endif
