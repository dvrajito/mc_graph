/*************************************************************************

   Project: MC-Graph, a C++ implementation of genetic algorithms
            for graph drawing and visualization
   License: Creative Commons, Attribution
   Authors: Dana Vrajitoru
            Jason DeBoni
   File:    GraphPos.cc
   Update:  October 2022

   Functions to build a consistent graph layout.

*************************************************************************/

#include <cmath>
#include "Graph.h"
#include "Point.h"
#include "GraphPos.h"

// Builds a random layout within a sphere of the given radius. For
// each vertex in the graph it generates a random 3D point. It returns
// the array of points that represents the layout.
Point* RandomPoints(int size, float radius)
{
    Point* points = new Point[size];
    for (int i = 0; i < size; i++)
        points[i].Init(1, radius);
    return points;
}

// Iteration function for the tension-vector algorithm. If the array
// of points (the layout) is null, it generates it randomly. Otherwise
// it computes the tension vectors in each vertex of the graph and
// moves the points in the direction of the resulting vectors by a
// distance that is the norm of the vector multiplied by epsilon. It
// repeats this procedure for a number of iterations.
float VectorCreatePoints(Point*& points, Graph* aGraph,
    int nrIter, float epsilon,
    int& StopPoint,
    int ConvergeCheck, float ConvergeLimit)
{
    float total_norm = 0, TotalError, TotalWeight, norm;

    TotalWeight = aGraph->TotalWeight();

    if (points == NULL)
        points = RandomPoints(aGraph->vertexNr);
    Point* vectors = NULL;

    for (int i = 0; i < nrIter; i++) {
        aGraph->Compute_vectors(vectors, points);
        //cout << "Points: ";
        //Print(points, aGraph->vertexNr);
        //cout << "Vectors: ";
        //Print(vectors, aGraph->vertexNr);
        for (int j = 0; j < aGraph->vertexNr; j++) {
            norm = vectors[j].Norm();
            if (isnan(norm))
                break;
            if (norm > 100) {
                while (vectors[j].Norm() > 10000)
                    vectors[j] *= 0.001;
                while (vectors[j].Norm() > 100)
                    vectors[j] *= 0.1;
            }
            vectors[j] *= epsilon;
            points[j] += vectors[j];
        }

        TotalError = aGraph->EvalDistance(points);
        if (isnan(TotalError)) {
            StopPoint = i;
            break;
        }

        if (ConvergeCheck && (TotalError / TotalWeight) < ConvergeLimit) {
            StopPoint = i;
            break;
        }
    }

    for (int i = 0; i < aGraph->vertexNr; i++)
        total_norm += vectors[i].Norm();
    total_norm /= epsilon;
    return total_norm;
}

//Modification of the tension-vector algorithm.  If any non-adjacent
//points are closer than the average weight of the adjacent points, 
//a repulsion vector is added to the point.
float VectorRepulsionCreatePoints(Point*& points, Graph* aGraph,
    int nrIter, float epsilon,
    int& StopPoint,
    int ConvergeCheck, float ConvergeLimit)
{
    //cout << "Executing VectorRepulsionCreatePoints()" << endl;
    float total_norm = 0, TotalError, TotalWeight;
    if (points == NULL)
        points = RandomPoints(aGraph->vertexNr);
    Point* vectors = NULL;

    TotalWeight = aGraph->TotalWeight();

    for (int i = 0; i < nrIter; i++) {
        aGraph->ComputeRepulsionVectors(vectors, points);
        //cout << "Points: ";
        //Print(points, aGraph->vertexNr);
        //cout << "Vectors: ";
        //Print(vectors, aGraph->vertexNr);
        for (int j = 0; j < aGraph->vertexNr; j++) {
            if (isnan(vectors[j][0]) || isnan(vectors[j].Norm()))
                break;
            vectors[j] *= epsilon;
            points[j] += vectors[j];
        }

        TotalError = aGraph->EvalDistance(points);

        if (ConvergeCheck && (TotalError / TotalWeight) < ConvergeLimit) {
            StopPoint = i;
            break;
        }

        if (isnan(TotalError)) {
            StopPoint = i;
            break;
        }
    }

    for (int i = 0; i < nrIter; i++)
        total_norm += vectors[i].Norm();
    total_norm /= epsilon;
    return total_norm;
}

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
void MCCreatePoints(Point*& points, Graph* aGraph, int nrIter, float epsilon,
    int& StopPoint,
    int ConvergeCheck, float ConvergeLimit)
{
    int first, second, i, j;
    float goalDist, TotalError, TotalWeight;
    TotalWeight = aGraph->TotalWeight();

    //StopPoint = nrIter;

    if (points == NULL)
        points = RandomPoints(aGraph->vertexNr);
    for (i = 0; i < nrIter; i++) {
        for (j = 0; j < aGraph->edgeNr; j++) {
            aGraph->RandomArc(first, second, goalDist);
            AdaptPoint(points[first], points[second], goalDist, epsilon);
        }
        if (ConvergeCheck)
        {
            TotalError = aGraph->EvalDistance(points);
            //cout << "Total Error: " << TotalError << endl;
            //cout << "Total Weight: " << TotalWeight << endl;
            //cout << "Convergence: " << (TotalError / TotalWeight) << endl;

            if ((TotalError / TotalWeight) < ConvergeLimit) {
                StopPoint = i;
                //cout << "***Breaking***" << endl;
                //cout << "Break point: " << StopPoint << endl;
                break;
            }
        }
    }
}

// Implementation of the breadth-first scan method for generating
// consistent graph layouts. It works just like the random one, except
// that the points are visited in breadth-first order starting from a
// random one.
void CreatePoints(Point*& points, Graph* aGraph,
    int nrIter, int nrIter2, float epsilon,
    int& StopPoint,
    int ConvergeCheck, float ConvergeLimit)
{
    int current;
    float TotalError, TotalWeight;
    TotalWeight = aGraph->TotalWeight();

    //StopPoint = nrIter;

    static short* visited = NULL;
    static List toBeVisited;

    if (points == NULL) {
        points = RandomPoints(aGraph->vertexNr);
        current = aGraph->ScanGraph(visited, toBeVisited);
        while (current >= 0) {
            CreateNewPoint(points, visited, aGraph, current, nrIter, epsilon);
            current = aGraph->RandScanGraph(visited, toBeVisited);
        }
    }
    for (int i = 0; i < nrIter2; i++) {
        current = aGraph->RandScanGraph(visited, toBeVisited, 1);
        while (current >= 0) {
            AdjustPoint(points, visited, aGraph, current, nrIter, epsilon);
            current = aGraph->RandScanGraph(visited, toBeVisited);
        }
        if (ConvergeCheck)
        {
            TotalError = aGraph->EvalDistance(points);

            if ((TotalError / TotalWeight) < ConvergeLimit) {
                StopPoint = i;
                //cout << "***Breaking***" << endl;
                //cout << "Break point: " << StopPoint << endl;
                break;
            }
        }
    }
    //StopPoint = -1;
}

// Creates a new random 3D point for the vertex theNode and adapts all
// of its neighbors for a number of iterations.
void CreateNewPoint(Point* points, short* visited, Graph* aGraph,
    int theNode, int nrIter, float epsilon)
{
    float givenDist;
    int j;
    points[theNode].Init(1, 10.0);
    for (int i = 0; i < nrIter; i++) {
        ListNode* arcCrnt = aGraph->edgeList[theNode].head;
        while (arcCrnt != NULL) {
            j = arcCrnt->elem;
            givenDist = arcCrnt->weight;
            if (visited[j])
                AdaptPoint(points[theNode], points[j],
                    givenDist, epsilon);
            arcCrnt = arcCrnt->next;
        }
    }
}

// Adapts all the neighbors of a given vertex (theNode) for a number
// of iterations.
void AdjustPoint(Point* points, short* visited, Graph* aGraph,
    int theNode, int nrIter, float epsilon)
{
    float givenDist;
    int j;
    for (int i = 0; i < nrIter; i++) {
        ListNode* arcCrnt = aGraph->edgeList[theNode].head;
        while (arcCrnt != NULL) {
            j = arcCrnt->elem;
            givenDist = arcCrnt->weight;
            if (visited[j])
                AdaptPoint(points[theNode], points[j], givenDist, epsilon);
            arcCrnt = arcCrnt->next;
        }
    }
}

// Moves one point (B) on the edge AB according to the difference
// between the weight of the edge (givenDist) and the length of the
// line segment between A and B. B is moved on the same line.
void AdaptPoint(Point& A, Point B, float givenDist, float epsilon)
{
    double realDist = Distance(A, B), factor;
    if (realDist != 0.0)
        factor = epsilon * (givenDist - realDist) / realDist;
    else factor = epsilon * givenDist;

    if (realDist != givenDist) {
        A.x = A.x + (A.x - B.x) * factor;
        A.y = A.y + (A.y - B.y) * factor;
        A.z = A.z + (A.z - B.z) * factor;
    }
}

// Computes the tension vector for just one point based on its
// neighbors and moves the point accordingly.
void AdaptPointTV(int vertex, Point* points, Graph* aGraph, float epsilon)
{
    Point theVector;
    aGraph->ComputeVector(vertex, theVector, points);
    theVector *= epsilon;
    points[vertex] += theVector;
}

// Computes the distance between the two points.
double Distance(Point& A, Point& B)
{
    return sqrt((A.x - B.x) * (A.x - B.x) +
        (A.y - B.y) * (A.y - B.y) +
        (A.z - B.z) * (A.z - B.z));
}

// Computes the scalar product of the two vectors.
double ScalarProd(Point& A, Point& B)
{
    return A.x * B.x + A.y * B.y + A.z * B.z;
}

// Center the points around the origin.
void center_points(Point*& points, int size)
{
    Point b1, b2, mid;
    BoundingBox(points, size, b1, b2);
    mid = b1;
    mid += b2;
    mid *= 0.2;
    for (int i = 0; i < size; i++)
        points[i] -= mid;
}
