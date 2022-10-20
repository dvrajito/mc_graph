/*************************************************************************

   Project: MC-Graph, a C++ implementation of genetic algorithms
			for graph drawing and visualization
   License: Creative Commons, Attribution
   Authors: Dana Vrajitoru
			Jason DeBoni
   File:    GraphPos.h
   Update:  October 2022

   Functions to build a consistent graph layout.

*************************************************************************/

#ifndef GRAPH_POS_H
#define GRAPH_POS_H

#include "Point.h"
#include "Graph.h"

enum Method { BreadthFirst, Random, TensionVector, RepulsionVector };

// Builds a random layout within a sphere of the given radius.
Point *RandomPoints(int size, float radius=10.0);

// Iteration function for the tension-vector algorithm. If the array
// of points (the layout) is null, it generates it randomly. Otherwise
// it computes the tension vectors in each vertex of the graph and
// moves the points in the direction of the resulting vectors by a
// distance that is the norm of the vector multiplied by epsilon. It
// repeats this procedure for a number of iterations.
float VectorCreatePoints(Point *&points, Graph *aGraph, 
			 int nrIter, float epsilon,
			 int &StopPoint,
			 int ConvergeCheck = 0, 
			 float ConvergeLimit = 0.0);

//Modification of the tension-vector algorithm.  If any non-adjacent
//points are closer than the average weight of the adjacent points, 
//a repulsion vector is added to the point.
float VectorRepulsionCreatePoints(Point *&points, Graph *aGraph, 
				  int nrIter, float epsilon,
				  int &StopPoint,
				  int ConvergeCheck, float ConvergeLimit);

// Implementation of the Monte Carlo method to generate a consistent
// graph layout that selects a random edge in the graph and modifies
// one of the points on that edge. If the distance between the points
// is larger than the weight of the edge, the point is moved closer on
// the same line segment, and if the distance is shorter than the
// weight, it moves the point farther. The amount of movement depends
// on the difference between the weight and the current length of the
// edge and on epsilon. It repeats this for a number of iterations. At
// each iteration it selects a number of edges equal to the number of
// edges in the graph, but this being random, it may select some edges
// more than once and other not at all. Just as before, if the array
// of points is null, it generates a random layout.
void MCCreatePoints(Point *&points, Graph *aGraph, int nrIter, float epsilon,
      int &StopPoint,
      int ConvergeCheck = 0, float ConvergeLimit = 0.0);

// Implementation of the breadth-first scan method for generating
// consistent graph layouts. It works just like the random one, except
// that the points are visited in breadth-first order starting from a
// random one.
void CreatePoints(Point *&points, Graph *aGraph, 
		  int nrIter, int nrIter2, float epsilon, 
		  int &StopPoint,
		  int ConvergeCheck = 0, float ConvergeLimit = 0.0);

// Creates a new random 3D point for the vertex theNode and adapts all
// of its neighbors for a number of iterations.
void CreateNewPoint(Point *points, short *visited, Graph *aGraph,
		    int theNode, int nrIter, float epsilon);

// Adapts all the neighbors of a given vertex (theNode) for a number
// of iterations.
void AdjustPoint(Point *points, short *visited, Graph *aGraph,
		 int theNode, int nrIter, float epsilon);

// Moves one point (B) on the edge AB according to the difference
// between the weight of the edge (givenDist) and the length of the
// line segment between A and B. B is moved on the same line.
void AdaptPoint(Point &A, Point B, float givenDist, float epsilon);

// Computes the tension vector for just one point based on its
// neighbors and moves the point accordingly.
void AdaptPointTV(int vertex, Point *points, Graph *aGraph, float epsilon);

// Computes the distance between the two points.
double Distance(Point &A, Point &B);

// Computes the scalar product of the two vectors.
double ScalarProd(Point &A, Point &B);

// Center the points around the origin.
void center_points(Point *&points, int size);

#endif
