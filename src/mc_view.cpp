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

/* Main function */

int mc_view(DVcreate_image_arguments &dv)
{
  int line_width, geometry_complexity, light_complexity;  
  Graph *the_graph = new Graph;
  Point *points = NULL;
  float sphere_radius, minw, maxw;
  char filename[80];
  istrstream top_fin((char const*)dv.input_string,dv.input_string.getLength());
  
  top_fin >> filename ;
  top_fin >> line_width >> sphere_radius;
  top_fin >> geometry_complexity;
  top_fin >> light_complexity; 

  dv.top.geometry_complexity = geometry_complexity;
  dv.top.light_complexity = light_complexity;
  dv.top.color[0] = DVcolor(1.0, 1.0, 0.0);

  cout << "Reading graph from file: " << filename << endl;

  points = the_graph->ReadPoints(filename);

  //the_graph->Print();
  //minw = the_graph->MinWeight();
  //maxw = the_graph->MaxWeight();

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
