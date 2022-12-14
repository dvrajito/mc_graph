/************************************************************

  Dana Vrajitoru
  Jason DeBoni
  Project MC-Graph.
  Last updated: May 20, 2004.

  Graph.cpp
  Implementation of the Graph class.

*************************************************************/

#include <fstream>
#include <iostream>
#include <strstream>
using namespace std;
#include <stdlib.h>
#include <cstring>
#include <cmath>

#include "Graph.h"
#include "name_table.h"
#include "GraphExtra.h"
#include "GraphPos.h"

// Constructor: does nothing but call the init function.
Graph::Graph(int verticesNr)
{
  Init(verticesNr);
}

// Destructor: maybe it should delete the edge list.
Graph::~Graph()
{;}

// Initialize the graph with the default values: undirected,
// unweighted, with no edges.
void Graph::Init(int verticesNr)
{
  vertexNr = verticesNr;
  edgeNr = 0;
  weighted = 0;
  directed = 0;
  if (vertexNr) 
    edgeList = new List [vertexNr];
  else
    edgeList = NULL;
}

// Read the graph from a graph file.
bool Graph::Read(const char *filename)
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
    directed=0;
    weighted=1;
    for (i=0; i < edges; i++) {
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
Point *Graph::ReadPoints(const char *filename)
{
  char input[10];
  int i, j, k, edges;
  float wght;

  ifstream fin(filename);
  directed=0;
  weighted=1;
  if (fin) {
    fin >> vertexNr >> input
	>> edges >> input;
    Init(vertexNr);
    for (i=0; i < edges; i++) {
      fin >> input;
      fin >> wght;
      ArcConvert(input, j, k);
      AddEgde(j, k, wght);
    }
    Point *points = new Point[vertexNr];
    for (i=0; i<vertexNr; i++)
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
  edgeList[start].AddTail(end, wght);
  if (!directed) 
    edgeList[end].AddTail(start, wght);
  edgeNr++;
}

// Get the vertex numbers and the weight from a line containing the
// letter of the first vertex, comma, the letter of the second vertex,
// another comma, then eventually the weight. Could be rewritten using
// C++ functionality.
int Graph::ProcessEdgeLine(int &j, int &k, float &wght, char *line)
{
  if (strlen(line) >= 4) {
    j = Sequence_number(line);
    
    // Suppose in the file the name is 1 char, followed by ',' 
    // and by 1 space
    k = Sequence_number(line + 3); 
    
    if (strlen(line) > 6) {
      wght = atof(line+6);
      weighted = 1;
    }
    return 1;
  }
  else
    return 0;
}

// Reading a graph from a file in the format used with the C251 and
// C243 classes.
int Graph::ReadC251(const char *filename)
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

    for (i=0; i<vertexNr; i++) // read the names
      fin >> input;            // in this version we have nothing to do

    while (!fin.eof()) {
      fin.getline((char *)input, 30);
      if (ProcessEdgeLine(j, k, wght, (char *)input))
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
  for (int i=0; i<vertexNr; i++) {
    cout << i << "-->(";
    ListNode *arci = edgeList[i].head;
    while (arci != NULL) {
      if (weighted)
	cout << "(" << arci->elem << ' ' << arci->weight << ")-->";
      else
	cout << arci->elem << "-->" ;
      arci=arci->next;
    }
    cout << "/)" << endl;
  }
  cout << "End graph" << endl;
}

// Printing the graph in breadth-first order.
void Graph::BreadthFirstPrint(int v)
{
  int *marked = new int[vertexNr];
  int i, x, y;
  List Queue;
  ListNode *edge;

  cout << "This is the order in which the vertices are seen by the" << endl
       << "breadth first search algorithm starting from ";
  Write_name(v);
  cout << endl;

  for (i=0; i<vertexNr; i++) 
    marked[i]=0;

  marked[v] = 1;
  PreliminaryProcess(v);
  Queue.AddTail(v);
  while (!Queue.IsEmpty()) {
    x = Queue.RemoveHead();
    edge = edgeList[x].head;
    while (edge != NULL) {
      y = edge->elem;
      if (!marked[y]) {
	marked[y] = 1;
	PreliminaryProcess(y);
	Queue.AddTail(y);
      }
      edge=edge->next;
    }
  }
  cout << endl;
}

// Start a traversal of the graph in breadth-first order with a random
// origin.
int Graph::RandScanGraph(short *&nodes, List &scanList, int first)
{
  if (first) {
    if (nodes == NULL) 
      nodes = new short[vertexNr];
    int origin = rand() % vertexNr;
    scanList.AddTail(origin);
    for (int i=0; i<vertexNr; i++)
      nodes[i]=0;
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
int Graph::ScanGraph(short *&nodes, List &scanList)
{
  int first, secnd, i;

  if (nodes == NULL) {
    nodes = new short[vertexNr];
    for (int i=0; i<vertexNr; i++)
      nodes[i]=0;
  }
  if (scanList.IsEmpty()) {
    first = -1;
    for (i=0; (i<vertexNr) && nodes[i]; i++); // look for the first
					      // unmarked vertex
    if (i<vertexNr)
      first = i;
  }
  else {
    first = scanList.RemoveHead();
  }
  if (first != -1) {
    ListNode *arc1st = edgeList[first].head;
    while (arc1st != NULL) {
      secnd = arc1st->elem;
      if (!(nodes[secnd]) && !(scanList.Find(secnd)))
	scanList.AddTail(secnd);
      arc1st = arc1st->next;
    }
    nodes[first]=1;
  }
  return first;
}

// Evaluate the total error in the layout given by the array of
// points.
float Graph::EvalDistance(Point *pts)
{
  float sumDist=0;
  for (int i=0; i<vertexNr; i++) {
    ListNode *arcCrnt = edgeList[i].head;
    while (arcCrnt != NULL) {
      int j = arcCrnt->elem;
      sumDist += fabs(Weight(i, j)-Distance(pts[i], pts[j]));
      arcCrnt = arcCrnt->next;
    }
  }
  if (!directed)
    return sumDist/2;
  else
    return sumDist;
}

// Select one edge by random.
void Graph::RandomArc(int &v, int &w, float &wght)
{
  do 
    v = rand() % vertexNr;
  while (edgeList[v].IsEmpty());
  int k = rand() % edgeList[v].nodeNr;
  ListNode *vert = edgeList[v].head;
  for (int i=0; i<k-1; i++)
    vert = vert->next;
  w = vert->elem;
  wght = vert->weight;
}

// Returns the weight of the edge (i, j). If the edge doesn't exist it
// returns -1.
float Graph::Weight(int i, int j)
{
  ListNode *node = edgeList[i].FindNode(j);
  if (node)
    return node->weight;
  else
    return -1;
}

// Verifies if the edge (i, j) exists.
int Graph::IsArc(int i, int j)
{
  ListNode *node = edgeList[i].FindNode(j);
  if (node)
    return 1;
  else
    return 0;
}

// Find the maximal weight of all the edges in the graph.
float Graph::MaxWeight()
{
  ListNode *neighbor = NULL;
  float max = -1;
  int start = 1;

  for (int i=0; i<vertexNr; i++) {
    neighbor=edgeList[i].head;
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
  ListNode *neighbor = NULL;
  float min = -1;
  int start = 1;

  for (int i=0; i<vertexNr; i++) {
    neighbor=edgeList[i].head;
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
  ListNode *neighbor = NULL;
  float total = -1;

  for (int i=0; i<vertexNr; i++) {
    neighbor=edgeList[i].head;
    while (neighbor != NULL) {
      total += neighbor->weight;
      neighbor = neighbor->next;
    }
  }
  if (!directed)
    return total/2;
  else
    return total;
}

// Compute the resulting tension vectors in every vertex of the graph
// from the layout given by the array of points.
void Graph::Compute_vectors(Point *&vectors, Point *points)
{
  if (vectors == NULL)
    vectors = new Point[vertexNr];

  for (int i=0; i<vertexNr; i++)
    ComputeVector(i, vectors[i], points);
}

// Compute the resulting tension and repulsion vectors in every vertex of 
// the graph  from the layout given by the array of points.
void Graph::ComputeRepulsionVectors(Point *&vectors, Point *points)
{
  if (vectors == NULL)
    vectors = new Point[vertexNr];

  int i, j, k, NeighborCount, Node, node;
  float TotalWeight, AverageWeight, wght, dist;
  int *AdjacentList = new int[vertexNr]; 
  ListNode *AdjacentNode = NULL;
  ListNode *neighbor = NULL;

  TotalWeight = NeighborCount = 0;

  for (int i=0; i<vertexNr; i++)
    ComputeVector(i, vectors[i], points);

  for( i = 0; i < vertexNr; i++ )
    {
      for( k = 0; k < vertexNr; k++ )
	AdjacentList[k] = 0;

      for( AdjacentNode = edgeList[i].head; AdjacentNode != NULL; 
	   AdjacentNode = AdjacentNode -> next )
	{
	  Node = AdjacentNode -> elem;
	  AdjacentList[Node] = 1;
	  wght = AdjacentNode -> weight;
	  TotalWeight += wght;
	  NeighborCount++;
	}

      AverageWeight = TotalWeight / NeighborCount;

      for( j = 0; j < vertexNr; j++ )
	{
	  if( AdjacentList[j] == 1 )
	    continue;

	  dist = Distance( points[i], points[j] );

	  if( dist < AverageWeight )
	    {
	      Point Vect( points[i] );
	      Vect *= -1;
	      Vect += points[j];
	      Vect.Normalize();
	      Vect *= ( dist - AverageWeight );
	    }
	}

      AverageWeight = TotalWeight = NeighborCount = 0;
    }
}

// Compute just one tension vector at a time if we only want to move
// one point.
void Graph::ComputeVector(int vertex, Point &theVector, Point *points)
{
  int node;
  float wght, dist;
  ListNode *neighbor = NULL;

  theVector.Init(0.0);
  for (neighbor=edgeList[vertex].head; neighbor != NULL; 
       neighbor = neighbor->next) {
    node = neighbor->elem;
    Point vect(points[vertex]);
    vect *= -1;
    vect += points[node];
    vect.Normalize();
    dist=Distance(points[vertex], points[node]);
    wght = neighbor->weight;
    vect *= (dist-wght);
    theVector += vect;
  }
}
