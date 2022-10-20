/*************************************************************************

   Project: MC-Graph, a C++ implementation of genetic algorithms
            for graph drawing and visualization
   License: Creative Commons, Attribution
   Author:  Dana Vrajitoru
   File:    result_manip.cc
   Update:  October 2022

   Functions to manipulate result files for visualization.

*************************************************************************/

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <fstream>
using namespace std;

#include "result_manip.h"

#define M_PI 3.14159

// Finds the file to visualize either from argv (argument to the
// application), or by asking the user for the file name, detects if
// it's a graph file or a result file, then reads the graph from it
// and returns a pointer to the graph object. It returns the parameter
// resf as true if it's a result file, or false if it's a graph file.
Graph* Detect_read_graph(int argc, char** argv,
    char* resultFile, bool& resf)
{
    char c, graphFile[80];
    if (argc > 1)
        strcpy(resultFile, argv[1]);
    else {
        cout << "Enter the result or graph file to visualize" << endl;
        cin >> resultFile;
    }
    ifstream fin(resultFile);
    if (!fin) {
        cout << "Could not open file" << endl;
        return NULL;
    }

    c = fin.peek();
    if (isdigit(c)) { // a graph file starts with the number of vertices.
        resf = false;
        strcpy(graphFile, resultFile);
    }
    else {
        resf = true;
        fin >> graphFile  // skip "Graph" 
            >> graphFile  // skip "file:"
            >> graphFile;
    }

    Graph* g = new Graph;
    g->Read(graphFile);
    return g;
}

// Extracts the original layout from a graph file. It skips the graph
// altogether and only reads the information at the end. If there is
// no layout, it will simply generate the points on a circle.
void Read_gr_layout(Point points[], int size, char* graphFile)
{
    char buff[100];
    int edges, i;
    ifstream fin(graphFile);
    if (!fin) {
        cout << "Could not open file, generating a circle" << endl;
        Generate_circle(points, size);
        return;
    }
    fin >> buff >> buff >> edges >> buff;
    for (i = 0; i < edges && fin.good(); i++)
        fin >> buff >> buff;
    for (i = 0; i < size && fin.good(); i++)
        fin >> points[i].x >> points[i].y >> points[i].z;
    if (!fin.good()) {
        cout << "Could not read the layout properly, generating a circle" << endl;
        Generate_circle(points, size);
    }
    fin.close();
}

// Reads the first layout that it finds in the result file. Will be
// updated later to select a particular layout.
void Read_res_layout(Point points[], int size, char* resFile)
{
    char buff[100];
    ifstream fin(resFile);
    if (!fin) {
        cout << "Could not open file, generating a circle" << endl;
        Generate_circle(points, size);
        return;
    }
    do
        fin >> buff;
    while (fin.good() && strcmp(buff, "layout:") != 0);
    for (int i = 0; i < size && fin.good(); i++)
        fin >> points[i].x >> points[i].y >> points[i].z;
    if (!fin.good()) {
        cout << "Could not read the layout properly, generating a circle" << endl;
        Generate_circle(points, size);
    }
    fin.close();
}

// Reads the a layout from the result file based on a number it
// receives as a parameter.
void Read_res_select(Point points[], int size, char* resFile, int which)
{
    char buff[100];
    int i;
    ifstream fin(resFile);
    if (!fin) {
        cout << "Could not open file, generating a circle" << endl;
        Generate_circle(points, size);
        return;
    }
    for (i = 1; i < which; i++)
        do
            fin >> buff;
    while (fin.good() && strcmp(buff, "layout:") != 0);
    for (i = 0; i < size && fin.good(); i++)
        fin >> points[i].x >> points[i].y >> points[i].z;
    if (!fin.good()) {
        cout << "Could not read the layout properly, generating a circle" << endl;
        Generate_circle(points, size);
    }
    fin.close();
}

// Generates a number of points on a unit circle in the (xy) plane,
// z=0.
void Generate_circle(Point points[], int size)
{
    float alpha = 2 * M_PI / size;
    for (int i = 0; i < size; i++) {
        points[i].x = cos(i * alpha);
        points[i].y = sin(i * alpha);
        points[i].z = 0;
    }
}
