/************************************************************

  Dana Vrajitoru
  Project MC-Graph.
  Last updated: May 18, 2004.

  DrawGraph.h
  Functions to transform a graph into DataViewer objects.

*************************************************************/

#ifndef DRAW_GRAPH_H
#define DRAW_GRAPH_H

#include "Graph.h"
#include "Point.h"

// Builds the DataViewer objects representing the graph from the
// layout given by the array of points.
void make_graph(DVcontainerbase &cont, Graph *gr, Point *points,
		float sphere_radius, float line_width);

// Stores all the edges in the LCVMarray to be drawn as line segments.
void build_edges(LCVMarray_3D<float> &edges, Graph *grph, Point *points);
// Stores all the vertices in the LCVMarray to be drawn as little
// spheres.
void build_vertices(LCVMarray_2D<float> &vertices, Graph *grph, Point *points);

// Makes an output name from the input name for the mc_graph
void make_mc_out_name(const char *in_name, char *out_name, int option);

// Makes an output name from the input name
void make_out_name(const char *in_name, char *out_name, 
		   const char *folder_name, const char *option,
		   const char *prob_type, int flevel);

#endif
