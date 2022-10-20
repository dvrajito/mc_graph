/*******************************************************************

   Project: MC-Graph, a C++ implementation of genetic algorithms
            for graph drawing and visualization
   License: Creative Commons, Attribution
   Author:  Dana Vrajitoru
   File:    common.cc
   Update:  October 2022

   Helper function dealing with the output file.

*******************************************************************/

#include <iostream> 
#include <fstream> 
#include <strstream>
using namespace std;
#include "common.h"

// Output file for the results.
ofstream* fout = NULL;

// Outputs an integer to the result file on a line preceded by a line
// with a comment given by the parameter name.
void out_int(const char* name, int val)
{
    if (fout)
        (*fout) << name << ": " << val << endl;
    else
        cout << name << ": " << val << endl;
}

// Outputs a real number to the result file on a line preceded by a
// line with a comment given by the parameter name.
void out_float(float val)
{
    if (fout)
        (*fout) << val << ' ';
    else
        cout << val << ' ';
}

// Writes a new line to the output file.
void out_endl()
{
    if (fout)
        (*fout) << endl;
    else
        cout << endl;
}

// Outputs a string on a line in the result file.
void out_str(const char* name)
{
    if (fout)
        (*fout) << name;
    else
        cout << name;
}

// Copies a file if it already exists such that the new results are
// added to it.
void my_copy_file(const char* name)
{
    fout = new ofstream(name, ios::app);
    /*
    char buffer[250], temp_file[]="/temp/dummy.txt";
    ofstream temp_out((char *) temp_file);
    ifstream tin1(name);
    if (temp_out && tin1) {
      while (!tin1.eof()) {
        tin1.getline((char *)buffer, 249, '\n');
        temp_out << buffer << endl;
      }
      temp_out.close();
    }
    fout = new ofstream (name);
    ifstream fin((char *) temp_file);
    if (fout && fin)
      while (!fin.eof()) {
        fin.getline((char *)buffer, 249, '\n');
        (*fout) << buffer << endl;
      }
    */
}
