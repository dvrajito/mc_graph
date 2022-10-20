/***************************************************************

   Project: MCG, implementation of the graph layout algorithm
            using genetic algorithms.
   Author:  Dana Vrajitoru
   File:    wiener.cc
   Update:  October 2022

   The main function for a visualization program for the
   solutions.

***************************************************************/

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <fstream>
using namespace std;
#include "Graph.h"
#include "common.h"

extern ofstream* fout;

// The main function in the Wiener version of the program.
int mainWiener(int argc, char** argv)
{
    char infile[80], outfile[80], * grn, * ptr;
    Graph gr;

    if (argc > 1)
        strcpy(infile, argv[1]);
    else {
        cout << "Enter graph file name" << endl;
        cin >> infile;
    }
    if (argc > 2)
        strcpy(outfile, argv[2]);
    else {
        cout << "Enter output file name" << endl;
        cin >> outfile;
    }
    if (!gr.Read(infile)) {
        cout << "could not read the graph file" << endl;
        return 0;
    }
    my_copy_file(outfile);
    if (!fout) {
        cout << "Could not open the output file: " << outfile << endl;
        return 0;
    }
    grn = infile;
    for (ptr = infile; *ptr != '\0'; ptr++)
        if (*ptr == '/')
            grn = ptr;
    grn++;
    (*fout) << grn << '\t' << gr.Wiener() << '\t' << gr.IntWiener() << endl;
    //(*fout) << grn << '\t' << gr.CountTriangles() << endl;
    fout->close();
    return 0;
}

