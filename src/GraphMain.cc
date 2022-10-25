/*************************************************************************

   Project: MC-Graph, a C++ implementation of genetic algorithms
            for graph drawing and visualization
   License: Creative Commons, Attribution
   Author:  Dana Vrajitoru
   File:    GraphMain.cc
   Update:  October 2022

   The main function for a C251 homework.

*************************************************************************/

#include "Graph.h"
#include "Point.h"
#include "GraphPos.h"
#include <iostream>
using namespace std;

// The main for graph testing
int mainGraph()
{
    Graph* dg5 = new Graph;
    Point* points;
    char filename[50];
    int nriter, nriter2, stop = 500;
    float eps;
    cout << "Enter the name of file containing the graph" << endl;
    cin >> filename;
    dg5->Read(filename);
    dg5->Print();
    do {
        cout << "Nr of interior iterations" << endl;
        cin >> nriter;
        cout << "Nr of exterior iterations" << endl;
        cin >> nriter2;
        cout << "Epsilon:" << endl;
        cin >> eps;
        CreatePoints(points, dg5, nriter, nriter2, eps, stop);
        //points = MCCreatePoints(dg5, nriter, 1);
        for (int i = 1; i < dg5->vertexNr; i++)
            points[i].Print();
        cout << "Distance: " << dg5->EvalDistance(points) << endl;
    } while (nriter != 0);
    return 0;
}
