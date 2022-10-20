/*
    DataViewer visualization package
    Copyright (C) 1997 Randy Paffenroth and Thomas Stone

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
    MA 02111-1307, USA
*/
/*
  A library created by Dana Vrajitoru for the graph positionning
  problem.  
*/

#include <iostream> 
#include <fstream> 
#include <strstream>
using namespace std;
//#include <iomanip.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <ctype.h>

#include "DV.h"

#include "DrawGraph.h"
#include "GraphPos.h"

// Other prototypes
bool find_graph_name(char *filename, int result_nr, char *graph_file, 
		     Point *&points);

/* Main function */

int view_res(DVcreate_image_arguments &dv)
{
  int line_width, geometry_complexity, light_complexity;  
  Graph *the_graph = new Graph;
  Point *points = NULL;
  float sphere_radius, minw, maxw;
  char filename[80], graph_file[80];
  int result_nr;
  istrstream top_fin((char const*)dv.input_string,dv.input_string.getLength());
  
  top_fin >> filename >> result_nr;
  top_fin >> line_width >> sphere_radius;
  top_fin >> geometry_complexity;
  top_fin >> light_complexity; 

  dv.top.geometry_complexity = geometry_complexity;
  dv.top.light_complexity = light_complexity;
  dv.top.color[0] = DVcolor(1.0, 1.0, 0.0);

  cout << "Reading results from file: " << filename << endl;
  if (!find_graph_name(filename, result_nr, graph_file, points)) 
    return 0;

  cout << "Reading graph from file: " << graph_file << endl;
  the_graph->Read(graph_file);

  DVcolor_by_data tmp_color_by_data;
  DVcolor red(1, 0, 0), yellow(1, 1, 0), blue(0.4, 0.4, 1);
  DVcolor_map the_map(red, blue, yellow);
  tmp_color_by_data.set_color_map(the_map);
  tmp_color_by_data.set_min(-3);
  tmp_color_by_data.set_max(3);
  
  dv.top.color_by_data[0] = tmp_color_by_data;
  

  make_graph(dv.top, the_graph, points, sphere_radius, line_width);

  return 1;
}

bool find_graph_name(char *filename, int result_nr, char *graph_file, 
 		     Point *&points)
{
  ifstream fin(filename);
  char buffer[250];
  int i=0, vertices;
  do {
    do {
      fin >> buffer;
      if (strcmp(buffer, "Graph") == 0) {
	fin >> buffer >> graph_file;
      }
      if (strcmp(buffer, "vertices:") == 0)
	fin >> vertices;
    }
    while (fin.good() && strcmp(buffer, "layout:") != 0);
    i++;
  } while (i <= result_nr);
  if (!fin.good())
    return false;
  points = new Point[vertices];
  for (i=0; i<vertices; i++) 
    for (int j=0; j<3; j++)
      fin >> points[i][j];
  return fin.good();
}
