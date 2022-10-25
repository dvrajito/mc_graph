/********************************************************************

   Project: MC-Graph, a C++ implementation of genetic algorithms
            for graph drawing and visualization
   License: Creative Commons, Attribution
   Authors: Dana Vrajitoru
            Jason DeBoni
   File:    Graph.h
   Updated: October 2022

   Definition of a weighted graph

*********************************************************************/


#ifndef Graph_h
#define Graph_h

#include "ListW.h"
#include "Point.h"

class Graph {
public:
    ListW* edgeList;

    int vertexNr, edgeNr;

    int directed, weighted;

    // Constructor: does nothing but call the init function.
    Graph(int verticesNr = 0);
    // Destructor: maybe it should delete the edge list.
    ~Graph();

    // Initialize the graph with the default values: undirected,
    // unweighted, with no edges.
    void Init(int verticesNr = 0);

    // Add an edge to the graph with given weight.
    void AddEgde(int start, int end, float wght = 1);
    // Get the vertex numbers and the weight from a line containing the
    // letter of the first vertex, comma, the letter of the second
    // vertex, another comma, then eventually the weight. Could be
    // rewritten using C++ functionality.
    int ProcessEdgeLine(int& j, int& k, float& wght, char* line);

    // Reading a graph from a file in the format used with the C251 and
    // C243 classes.
    int ReadC251(const char* filename);
    // Read the graph from a graph file.
    bool Read(const char* filename);
    // Read a graph layout from a file. For each vertex, input a 3D
    // point.
    Point* ReadPoints(const char* filename);

    // Simple printout of the graph.
    void Print();

    // Printing the graph in breadth-first order.
    void BreadthFirstPrint(int v);

    // Returns the weight of the edge (i, j). If the edge doesn't exist
    // it returns -1.
    float Weight(int i, int j);
    // Verifies if the edge (i, j) exists.
    bool IsEdge(int i, int j);

    // Scan the graph in breadth-first order. The nodes array contains a
    // mark for the vertices that have already been visited. If the list
    // is empty, then the scan will start with the first unmarked
    // vertex. Otherwise it pops a vertex out of the list, adds all the
    // unmarked neighbors of this vertex to the end of the list (which
    // is in fact a queue), marks them, and returns the vertex that has
    // been popped. A complete breadth-first traversal of the graph is
    // chieved by calling this function repeatedly until all the
    // vertices have been marked.
    int ScanGraph(short*& nodes, ListW& scanList);

    // Start a traversal of the graph in breadth-first order with a
    // random origin.
    int RandScanGraph(short*& nodes, ListW& scanList, int first = 0);

    // Evaluate the total error in the layout given by the array of
    // points.
    float EvalDistance(Point* pts);

    // Select one edge by random.
    void RandomEdge(int& i, int& j, float& wght);

    // Find the maximal weight of all the edges in the graph.
    float MaxWeight();
    // Find the minimal weight of all the edges in the graph.
    float MinWeight();
    // Find the total weight of all the edges in the graph.
    float TotalWeight();

    // Compute the resulting tension vectors in every vertex of the
    // graph from the layout given by the array of points.
    void Compute_vectors(Point*& vectors, Point* points);

    // Compute the resulting tension and repulsion vectors in every 
    // vertex of the graph from the layout given by the array of points.
    void ComputeRepulsionVectors(Point*& vectors, Point* points);

    // Compute just one tension vector at a time if we only want to move
    // one point.
    void ComputeVector(int vertex, Point& theVector, Point* points);

    // Finds the minimal distance between the source and all the
    // vertices in the graph connected to it using Dijkstra's
    // algorithm. It returns -1 for those that are not connected to
    // it. Stores the result in the second parameter.
    void Dijkstra(int source, float* dist);

    // Compute the length of the shortest path from the source vertex to
    // all the others in terms of number of edges and not weight. Stores
    // the result in the second parameter.
    void IntDist(int source, int* dist);

    // Computes the Wiener index of the graph based on the weights of
    // the edges.
    float Wiener();
    // Computes the Wiener indexes based on the path length as number of
    // edges. It ignores the weights.
    int IntWiener();

    // Count the number of triangles in the graph.
    int CountTriangles();

};

#endif
