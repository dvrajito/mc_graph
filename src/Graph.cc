/********************************************************************

   Project: MC-Graph, a C++ implementation of genetic algorithms
            for graph drawing and visualization
   License: Creative Commons, Attribution
   Authors: Dana Vrajitoru
            Jason DeBoni
   File:    Graph.cc
   Updated: October 2022

   Implementation of a weighted graph

*********************************************************************/


#include <fstream>
#include <iostream>
#include <strstream>
using namespace std;
#include <cstdlib>
#include <cstring>
#include <cmath>

#include "Graph.h"
#include "name_table.h"
#include "GraphExtra.h"
#include "GraphPos.h"
#include "pq_vertex.h"

// Constructor: does nothing but call the init function.
Graph::Graph(int verticesNr)
{
    Init(verticesNr);
}

// Destructor: maybe it should delete the edge list.
Graph::~Graph()
{
    ;
}

// Initialize the graph with the default values: undirected,
// unweighted, with no edges.
void Graph::Init(int verticesNr)
{
    vertexNr = verticesNr;
    edgeNr = 0;
    weighted = 0;
    directed = 0;
    if (vertexNr)
        edgeList = new ListW[vertexNr];
    else
        edgeList = NULL;
}

// Read the graph from a graph file.
bool Graph::Read(const char* filename)
{
    char input[10];
    int i, j, k, edges;
    float wght;

    ifstream fin(filename);
    if (fin) {
        fin >> vertexNr >> input
            >> edges >> input;
        if (!fin.good()) {
            fin.close();
            cout << "Error reading the graph file" << endl;
            return false;
        }
        Init(vertexNr);
        directed = 0;
        weighted = 1;
        for (i = 0; i < edges; i++) {
            fin >> input;
            fin >> wght;
            if (!fin.good()) {
                fin.close();
                cout << "Error reading the graph file" << endl;
                return false;
            }
            ArcConvert(input, j, k);
            AddEgde(j, k, wght);
        }
        fin.close();
        return true;
    }
    else {
        cout << "Could not open the graph file" << endl;
        return false;
    }
}

// Read a graph layout from a file. For each vertex, input a 3D point.
Point* Graph::ReadPoints(const char* filename)
{
    char input[10];
    int i, j, k, edges;
    float wght;

    ifstream fin(filename);
    directed = 0;
    weighted = 1;
    if (fin) {
        fin >> vertexNr >> input
            >> edges >> input;
        Init(vertexNr);
        for (i = 0; i < edges; i++) {
            fin >> input;
            fin >> wght;
            ArcConvert(input, j, k);
            AddEgde(j, k, wght);
        }
        Point* points = new Point[vertexNr];
        for (i = 0; i < vertexNr; i++)
            fin >> points[i].x >> points[i].y >> points[i].z;
        fin.close();
        return points;
    }
    else
        return NULL;
}

// Add an edge to the graph with given weight.
void Graph::AddEgde(int start, int end, float wght)
{
    edgeList[start].InsertBack(end, wght);
    if (!directed)
        edgeList[end].InsertBack(start, wght);
    edgeNr++;
}

// Get the vertex numbers and the weight from a line containing the
// letter of the first vertex, comma, the letter of the second vertex,
// another comma, then eventually the weight. Could be rewritten using
// C++ functionality.
int Graph::ProcessEdgeLine(int& j, int& k, float& wght, char* line)
{
    if (strlen(line) >= 4) {
        j = Sequence_number(line);

        // Suppose in the file the name is 1 char, followed by ',' 
        // and by 1 space
        k = Sequence_number(line + 3);

        if (strlen(line) > 6) {
            wght = atof(line + 6);
            weighted = 1;
        }
        return 1;
    }
    else
        return 0;
}

// Reading a graph from a file in the format used with the C251 and
// C243 classes.
int Graph::ReadC251(const char* filename)
{
    char input[30];
    int i, j, k;
    float wght = 1;

    ifstream fin(filename);
    if (fin) {

        fin >> input;  // Read the first char that says U or D. 
        if (input[0] == 'U' || input[0] == 'u')
            directed = 0;
        else if (input[0] == 'D' || input[0] == 'd')
            directed = 1;
        else {
            fin.close();
            return 0;
        }

        fin >> vertexNr;
        Init(vertexNr);

        for (i = 0; i < vertexNr; i++) // read the names
            fin >> input;            // in this version we have nothing to do

        while (!fin.eof()) {
            fin.getline((char*)input, 30);
            if (ProcessEdgeLine(j, k, wght, (char*)input))
                AddEgde(j, k, wght);
        }
        fin.close();
    }
    return 1;
}

// Simple printout of the graph.
void Graph::Print()
{
    cout << "The graph has " << vertexNr << " vertices and "
        << edgeNr << " edges" << endl;
    for (int i = 0; i < vertexNr; i++) {
        cout << i << "-->(";
        ListNodeW* edgei = edgeList[i].head;
        while (edgei != NULL) {
            if (weighted)
                cout << "(" << edgei->value << ' ' << edgei->value << ")-->";
            else
                cout << edgei->value << "-->";
            edgei = edgei->next;
        }
        cout << "/)" << endl;
    }
    cout << "End graph" << endl;
}

// Printing the graph in breadth-first order.
void Graph::BreadthFirstPrint(int v)
{
    int* marked = new int[vertexNr];
    int i, x, y;
    ListW Queue;
    ListNodeW* edge;

    cout << "This is the order in which the vertices are seen by the" << endl
        << "breadth first search algorithm starting from ";
    Write_name(v);
    cout << endl;

    for (i = 0; i < vertexNr; i++)
        marked[i] = 0;

    marked[v] = 1;
    PreliminaryProcess(v);
    Queue.InsertBack(v);
    while (!Queue.IsEmpty()) {
        x = Queue.Front();
        Queue.RemoveFront();
        edge = edgeList[x].head;
        while (edge != NULL) {
            y = edge->value;
            if (!marked[y]) {
                marked[y] = 1;
                PreliminaryProcess(y);
                Queue.InsertBack(y);
            }
            edge = edge->next;
        }
    }
    cout << endl;
}

// Start a traversal of the graph in breadth-first order with a random
// origin.
int Graph::RandScanGraph(short*& nodes, ListW& scanList, int first)
{
    if (first) {
        if (nodes == NULL)
            nodes = new short[vertexNr];
        int origin = rand() % vertexNr;
        scanList.InsertBack(origin);
        for (int i = 0; i < vertexNr; i++)
            nodes[i] = 0;
    }
    return ScanGraph(nodes, scanList);
}

// Scan the graph in breadth-first order. The nodes array contains a
// mark for the vertices that have already been visited. If the list
// is empty, then the scan will start with the first unmarked
// vertex. Otherwise it pops a vertex out of the list, adds all the
// unmarked neighbors of this vertex to the end of the list (which is
// in fact a queue), marks them, and returns the vertex that has been
// popped. A complete breadth-first traversal of the graph is chieved
// by calling this function repeatedly until all the vertices have
// been marked.
int Graph::ScanGraph(short*& nodes, ListW& scanList)
{
    int first, secnd, i;

    if (nodes == NULL) {
        nodes = new short[vertexNr];
        for (int i = 0; i < vertexNr; i++)
            nodes[i] = 0;
    }
    if (scanList.IsEmpty()) {
        first = -1;
        for (i = 0; (i < vertexNr) && nodes[i]; i++); // look for the first
        // unmarked vertex
        if (i < vertexNr)
            first = i;
    }
    else {
        first = scanList.Front();
        scanList.RemoveFront();
    }
    if (first != -1) {
        ListNodeW* edge1st = edgeList[first].head;
        while (edge1st != NULL) {
            secnd = edge1st->value;
            if (!(nodes[secnd]) && !(scanList.Search(secnd)))
                scanList.InsertBack(secnd);
            edge1st = edge1st->next;
        }
        nodes[first] = 1;
    }
    return first;
}

// Evaluate the total error in the layout given by the array of
// points.
float Graph::EvalDistance(Point* pts)
{
    float sumDist = 0;
    for (int i = 0; i < vertexNr; i++) {
        ListNodeW* edgeCrnt = edgeList[i].head;
        while (edgeCrnt != NULL) {
            int j = edgeCrnt->value;
            sumDist += fabs(Weight(i, j) - Distance(pts[i], pts[j]));
            edgeCrnt = edgeCrnt->next;
        }
    }
    if (!directed)
        return sumDist / 2;
    else
        return sumDist;
}

// Select one edge by random.
void Graph::RandomEdge(int& v, int& w, float& wght)
{
    do
        v = rand() % vertexNr;
    while (edgeList[v].IsEmpty());
    int k = rand() % edgeList[v].size;
    ListNodeW* vert = edgeList[v].head;
    for (int i = 0; i < k - 1; i++)
        vert = vert->next;
    w = vert->value;
    wght = vert->weight;
}

// Returns the weight of the edge (i, j). If the edge doesn't exist it
// returns -1.
float Graph::Weight(int i, int j)
{
    ListNodeW* node = edgeList[i].Search(j);
    if (node)
        return node->weight;
    else
        return -1;
}

// Verifies if the edge (i, j) exists.
bool Graph::IsEdge(int i, int j)
{
    ListNodeW* node = edgeList[i].Search(j);
    if (node)
        return true;
    else
        return false;
}

// Find the maximal weight of all the edges in the graph.
float Graph::MaxWeight()
{
    ListNodeW* neighbor = NULL;
    float max = -1;
    int start = 1;

    for (int i = 0; i < vertexNr; i++) {
        neighbor = edgeList[i].head;
        while (neighbor != NULL) {
            if (start || neighbor->weight > max) {
                start = 0;
                max = neighbor->weight;
            }
            neighbor = neighbor->next;
        }
    }
    return max;
}

// Find the minimal weight of all the edges in the graph.
float Graph::MinWeight()
{
    ListNodeW* neighbor = NULL;
    float min = -1;
    int start = 1;

    for (int i = 0; i < vertexNr; i++) {
        neighbor = edgeList[i].head;
        while (neighbor != NULL) {
            if (start || neighbor->weight < min) {
                start = 0;
                min = neighbor->weight;
            }
            neighbor = neighbor->next;
        }
    }
    return min;
}

// Find the total weight of all the edges in the graph.
float Graph::TotalWeight()
{
    ListNodeW* neighbor = NULL;
    float total = -1;

    for (int i = 0; i < vertexNr; i++) {
        neighbor = edgeList[i].head;
        while (neighbor != NULL) {
            total += neighbor->weight;
            neighbor = neighbor->next;
        }
    }
    if (!directed)
        return total / 2;
    else
        return total;
}

// Compute the resulting tension vectors in every vertex of the graph
// from the layout given by the array of points.
void Graph::Compute_vectors(Point*& vectors, Point* points)
{
    if (vectors == NULL)
        vectors = new Point[vertexNr];

    for (int i = 0; i < vertexNr; i++)
        ComputeVector(i, vectors[i], points);
}

// Compute the resulting tension and repulsion vectors in every vertex of 
// the graph  from the layout given by the array of points.
void Graph::ComputeRepulsionVectors(Point*& vectors, Point* points)
{
    if (vectors == NULL)
        vectors = new Point[vertexNr];

    int i, j, k, NeighborCount, Node, node;
    float TotalWeight, AverageWeight, wght, dist;
    int* AdjacentList = new int[vertexNr];
    ListNodeW* AdjacentNode = NULL;
    ListNodeW* neighbor = NULL;

    TotalWeight = NeighborCount = 0;

    for (int i = 0; i < vertexNr; i++)
        ComputeVector(i, vectors[i], points);

    for (i = 0; i < vertexNr; i++)
    {
        for (k = 0; k < vertexNr; k++)
            AdjacentList[k] = 0;

        for (AdjacentNode = edgeList[i].head; AdjacentNode != NULL;
            AdjacentNode = AdjacentNode->next)
        {
            Node = AdjacentNode->value;
            AdjacentList[Node] = 1;
            wght = AdjacentNode->weight;
            TotalWeight += wght;
            NeighborCount++;
        }

        AverageWeight = TotalWeight / NeighborCount;

        for (j = 0; j < vertexNr; j++)
        {
            if (AdjacentList[j] == 1)
                continue;

            dist = Distance(points[i], points[j]);

            if (dist < AverageWeight)
            {
                Point Vect(points[i]);
                Vect *= -1;
                Vect += points[j];
                Vect.Normalize();
                Vect *= (dist - AverageWeight);
            }
        }

        AverageWeight = TotalWeight = NeighborCount = 0;
    }
}

// Compute just one tension vector at a time if we only want to move
// one point.
void Graph::ComputeVector(int vertex, Point& theVector, Point* points)
{
    int node;
    float wght, dist;
    ListNodeW* neighbor = NULL;

    theVector.Init(0.0);
    for (neighbor = edgeList[vertex].head; neighbor != NULL;
        neighbor = neighbor->next) {
        node = neighbor->value;
        Point vect(points[vertex]);
        vect *= -1;
        vect += points[node];
        vect.Normalize();
        dist = Distance(points[vertex], points[node]);
        wght = neighbor->weight;
        vect *= (dist - wght);
        theVector += vect;
    }
}


// Finds the minimal distance between the source and all the
// vertices in the graph connected to it using Dijkstra's
// algorithm. It returns -1 for those that are not connected to
// it. Stores the result in the second parameter.
void Graph::Dijkstra(int source, float* dist)
{
    pq_vertex pq;
    vertex_weight vw(source, 0);

    bool* finalized = new bool[vertexNr];
    int* pred = new int[vertexNr];
    int i, v, y;
    float wght;
    ListNodeW* neighbor = NULL;
    for (i = 0; i < vertexNr; i++) {
        finalized[i] = false;
        pred[i] = -1;
        dist[i] = -1;
    }
    pq += vw;
    dist[source] = 0;
    while (pq) {
        pq.dequeue(vw);
        v = vw.vertex;
        if (!finalized[v]) {
            finalized[v] = true;
            for (neighbor = edgeList[v].head; neighbor != NULL;
                neighbor = neighbor->next) {
                y = neighbor->value;
                wght = neighbor->weight;
                if (dist[y] == -1 || dist[y] > dist[v] + wght) {
                    dist[y] = dist[v] + wght;
                    vw.set_data(y, wght);
                    pq += vw;
                }
            }
        }
    }

    delete[] finalized;
    delete[] pred;
}

// Compute the length of the shortest path from the source vertex to
// all the others in terms of number of edges and not weight. Stores
// the result in the second parameter.
void Graph::IntDist(int source, int* dist)
{

    bool* marked = new bool[vertexNr];
    int* pred = new int[vertexNr];
    int i, x, y;
    ListW q;
    ListNodeW* edge;

    for (i = 0; i < vertexNr; i++) {
        marked[i] = false;
        pred[i] = -1;
    }

    marked[source] = 1;
    dist[source] = 0;
    q.InsertBack(source);
    while (!q.IsEmpty()) {
        x = q.RemoveFront();
        edge = edgeList[x].head;
        while (edge != NULL) {
            y = edge->value;
            if (!marked[y]) {
                marked[y] = 1;
                dist[y] = dist[x] + 1;
                q.InsertBack(y);
            }
            edge = edge->next;
        }
    }
    delete[] marked;
}


// Computes the Wiener index of the graph based on the weights of
// the edges.
float Graph::Wiener()
{
    float* dist = new float[vertexNr];
    int i, j;
    float w = 0;
    for (i = 0; i < vertexNr; i++) {
        Dijkstra(i, dist);
        for (j = 0; j < vertexNr; j++)
            if (dist[j] > 0) // adding only those that are connected. 
                w += dist[j];
    }
    delete[] dist;
    return w;
}

// Computes the Wiener indexes based on the path length as number of
// edges. It ignores the weights.
int Graph::IntWiener()
{
    int* dist = new int[vertexNr];
    int i, j;
    int w = 0;
    for (i = 0; i < vertexNr; i++) {
        IntDist(i, dist);
        for (j = 0; j < vertexNr; j++)
            if (dist[j] > 0) // adding only those that are connected.
                w += dist[j];
    }
    delete[] dist;
    return w;
}

// Count the number of triangles in the graph.
int Graph::CountTriangles()
{
    int count = 0, x, y, z;
    ListNodeW* edge, * edge1;
    for (x = 0; x < vertexNr; x++) {
        edge = edgeList[x].head;
        while (edge != NULL && edge->next != NULL) {
            y = edge->value;
            if (y > x) {
                edge1 = edge->next;
                while (edge1) {
                    z = edge1->value;
                    if (z > y && IsEdge(z, y))
                        count++;
                    edge1 = edge1->next;
                }
            }
            edge = edge->next;
        }
    }
    return count;
}
