/********************************************************************

   Project: MC-Graph, a C++ implementation of genetic algorithms
            for graph drawing and visualization
   License: Creative Commons, Attribution
   Authors: Dana Vrajitoru
            Jason DeBoni
   File:    GraphEval.h
   Updated: October 2022

   Functions for evaluating graphs for the purpose of
   genetic algorithms.

*********************************************************************/

#ifndef GRAPH_EVAL_H
#define GRAPH_EVAL_H

#include "List.h"
#include "Point.h"
#include <iostream>
#include "Graph.h"
using namespace std;

const float PI = 3.14159265;

//Calculates the standard deviation of the points in the graph
//Returns a Point with the stddev for x,y and z.
void VolumeSwag( Point *Points, int vertexNr, Point &StdDev );

//Calculate the average angle between edges for a particular point
double AngleSwag( Point *Points, Graph *aGraph, int StartPoint, 
	int Projection = 0 ); 

//Basic sort to put the angles calculated from a particular point
//in descending order
void SortAngles( double *AngleList, int AngleCount );

//Uses the AngleSwag function to determine an average for all the 
//points in the specified graph
double GraphAngleEvaluation(Point *Point, Graph *aGraph);

//Calculates the surface area of a graph, using an extended algorithm 
//similar to AngleSwag 
double GraphSurfaceArea(Point *Points, Graph *aGraph);

// Computes the area of a triangle specified as 3 points.
double SurfaceTriangle(Point &A, Point &B, Point &C);
//Calculates the distance between two points 
double PointDistance(Point PointA, Point PointB);

//Modification of the SortAngles function, also sorts a list of
//neighbor points keeping the order of the neighbors the same as the
//angles
void SortNeighborAngles( double *AngleList, int *NeighborList, int 
			 AngleCount );
#endif
