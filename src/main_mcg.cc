/*************************************************************************

   Project: MC-Graph, a C++ implementation of genetic algorithms
            for graph drawing and visualization
   License: Creative Commons, Attribution
   Author:  Dana Vrajitoru
   File:    main_mcg.cc
   Update:  October 2022

   Main function for the graph layout project.

*************************************************************************/

#include <iostream> 
#include <fstream> 
#include <strstream>
//#include <iomanip>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctype.h>
#include <ctime>
using namespace std;
#include "main.h"
#include "common.h"
#include "GraphPos.h"
#include "GraphEval.h"

extern ofstream* fout;

/* Main function */

// Main function for the MC Graph without GA version of the project.
int mainMCG(int argc, char* argv[])
{
    char* infile = new char[20];
    strcpy(infile, "./mcg_prefs.txt");
    if (argc > 1)
        infile = argv[1];
    ifstream fin(infile);
    if (!fin) {
        cout << "Could not open preference file: " << infile << endl;
        return 1;
    }
    srand(time(NULL));
    int method, iterations, intern = 1, step, frames, i,
        ConvergeCheck, StopPoint = -1;
    Graph* the_graph = new Graph;
    Point* points = NULL;
    float epsilon, maxw, ConvergeLimit, last_norm = 0, error;
    char filename[80], outfile[80];

    fin >> filename >> outfile;
    fin >> epsilon >> method >> iterations;
    fin >> step;
    fin >> ConvergeCheck >> ConvergeLimit;

    frames = iterations / step;

    cout << "Reading graph from file: " << filename << endl;

    //make_mc_out_name(filename, outfile, method);
    cout << "Results into file:" << endl << outfile << endl;
    my_copy_file((char*)outfile);
    if (!fout)
        cout << "Could not open the file: " << outfile << endl;
    else {
    }

    if (!(the_graph->Read(filename))) {
        cout << "Aborting computations for " << filename << endl;
        return 1;
    }
    maxw = the_graph->MaxWeight();

    out_int("method", method);
    out_int("iterations", iterations);
    out_str("epsilon: ");
    out_float(epsilon);
    out_endl();
    out_int("vertices", the_graph->vertexNr);
    out_int("edges", the_graph->edgeNr);
    out_str("total edge weight: ");
    out_float(the_graph->TotalWeight());
    out_endl();
    out_str("total error: ");

    points = RandomPoints(the_graph->vertexNr, 1.5 * maxw);

    double tempval;
    tempval = GraphAngleEvaluation(points, the_graph);
    //  cout << endl << "Pre Tempval: " << tempval << endl;

    out_float(the_graph->EvalDistance(points));
    out_endl();

    for (i = 0; i < frames; i++) {
        StopPoint = -1;
        if (method == BreadthFirst)
            CreatePoints(points, the_graph, intern, step, epsilon,
                StopPoint, ConvergeCheck, ConvergeLimit);
        else if (method == Random)
            MCCreatePoints(points, the_graph, step, epsilon, StopPoint,
                ConvergeCheck, ConvergeLimit);
        else if (method == TensionVector)
            last_norm = VectorCreatePoints(points, the_graph, step, epsilon,
                StopPoint, ConvergeCheck, ConvergeLimit);
        else
            last_norm = VectorRepulsionCreatePoints(points, the_graph,
                step, epsilon, StopPoint,
                ConvergeCheck, ConvergeLimit);

        error = the_graph->EvalDistance(points);
        out_float(error);

        if (i % 10 == 9)
            out_endl();

        if (ConvergeCheck && StopPoint >= 0) {
            out_int("\nStop iterations", i * step + StopPoint);
            break;
        }

        if (isnan(error)) {
            out_int("\nDiverge iterations", i * step + StopPoint);
            break;
        }

    }
    if (method >= TensionVector) {
        out_str("\ntotal tension vector norm: ");
        out_float(last_norm);
        out_endl();
    }

    tempval = GraphAngleEvaluation(points, the_graph);
    //  cout << endl << "Pre Tempval: " << tempval << endl;

    out_str("layout:\n");
    for (i = 0; i < the_graph->vertexNr; i++)
        (*fout) << points[i].x << ' ' << points[i].y << ' '
        << points[i].z << ' ';
    out_endl();
    out_endl();
    fout->close();

    cout << "done with graph" << endl;

    return 0;
}
