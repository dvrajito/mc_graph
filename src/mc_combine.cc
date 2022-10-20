/*
    DataViewer visualization package
    Copyright (C) 1997 Randy Paffenroth and Thomas Stone

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in thepy hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
    MA 02111-1307, USA
*/
/*
  A library created by Dana Vrajitoru for the graph layout problem.  
*/

#include <iostream> 
#include <fstream> 
#include <strstream>
using namespace std;
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <ctype.h>
#include "common.h"

#include "DV.h"

#include "DrawGraph.h"
#include "GraphPos.h"

extern DVtips_and_hints global_th;
extern ofstream *fout;

/* Main function */

int mc_combine(DVcreate_image_arguments &dv)
{ 
  int iterations_bf, iterations_tv, step, frames1, frames2, intern = 1,
      geometry_complexity, light_complexity, i;  
  Graph *the_graph = new Graph;
  Point *points = NULL;
  float epsilon_bf, epsilon_tv, minw, maxw, last_norm = 0, sphere_factor;
  char filename[80], outfile[80];
  //  FILE *testfile;
  istrstream top_fin((char const*)dv.input_string,dv.input_string.getLength());
  
  top_fin >> filename ;
  top_fin >> epsilon_bf >> iterations_bf >> epsilon_tv >> iterations_tv;
  top_fin >> geometry_complexity;
  top_fin >> light_complexity; 
  top_fin >> step >> sphere_factor;

  frames1 = iterations_bf/step;
  frames2 = iterations_tv/step;
  cout << "frames1: " << frames1 << " frames2: " << frames2 << endl;

  dv.top.geometry_complexity = geometry_complexity;
  dv.top.light_complexity = light_complexity;
  dv.top.color[0] = DVcolor(1.0, 1.0, 1.0);
  DVselection_container cont;

  cout << "Reading graph from file: " << filename << endl;

  make_mc_out_name(filename, outfile, 3);
  cout << "Results into file:" << endl << outfile << endl;
  my_copy_file((char *)outfile);
  if (!fout)
    cout << "Could not open the file: " << outfile << endl;
  else {
  }

  the_graph->Read(filename);
  //cout << the_graph->weighted<< endl;

  //the_graph->Print();
  minw = the_graph->MinWeight();
  maxw = the_graph->MaxWeight();
  if (minw == 0)
    minw = maxw / 20;

  DVcolor_by_data tmp_color_by_data;
  DVcolor red(1, 0, 0), yellow(1, 1, 0), blue(0, 0, 1);
  DVcolor_map the_map(red, yellow, blue);
  tmp_color_by_data.set_color_map(the_map);
  tmp_color_by_data.set_min(-maxw);
  tmp_color_by_data.set_max(maxw);

  dv.top.color_by_data[0] = tmp_color_by_data;

  out_int("iterations_bf", iterations_bf);
  out_str("epsilon_bf: ");
  out_float(epsilon_bf);
  out_int("iterations_tv", iterations_tv);
  out_str("epsilon_tv: ");
  out_float(epsilon_tv);
  out_endl();
  out_int("vertices", the_graph->vertexNr);
  out_int("edges", the_graph->edgeNr);

  points = RandomPoints(the_graph->vertexNr, 1.5*maxw);
  out_float(the_graph->EvalDistance(points));
  if (the_graph->edgeNr <= 70)
    make_graph(cont, the_graph, points, minw*sphere_factor, 2);
  else
    make_graph(cont, the_graph, points, minw*sphere_factor, 1);
  out_endl();

  for (i=0; i<frames1; i++) {
    CreatePoints(points, the_graph, intern, step, epsilon_bf);

    out_float(the_graph->EvalDistance(points));
    if (i % 10 == 9)
      out_endl();

    if (the_graph->edgeNr <= 70)
      make_graph(cont, the_graph, points, minw*sphere_factor, 2);
    else
      make_graph(cont, the_graph, points, minw*sphere_factor, 1);
  }
  out_endl();
  out_endl();

  for (i=0; i<frames2; i++) {
    last_norm = VectorCreatePoints(points, the_graph, step, epsilon_tv);

    out_float(the_graph->EvalDistance(points));
    if (i % 10 == 9)
      out_endl();

    if (the_graph->edgeNr <= 70)
      make_graph(cont, the_graph, points, minw*sphere_factor, 2);
    else
      make_graph(cont, the_graph, points, minw*sphere_factor, 1);
  }
  out_endl();
  out_float(last_norm);
  out_endl();
  fout->close();

  dv.dvhints.set_first_frame_number(0);
  dv.dvhints.set_last_frame_number(frames1+frames2);
  dv.dvhints.set_animation_optimize_flag(1);
  dv.dvhints.set_animation_first_frame_number(0);
  dv.dvhints.set_animation_last_frame_number(frames1+frames2);
  dv.dvhints.set_animation_step_frame_number(1);
  dv.top.copy_child(cont);

  return 1;
}
