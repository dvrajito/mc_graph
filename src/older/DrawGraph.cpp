/************************************************************

  Dana Vrajitoru
  Project MC-Graph.
  Last updated: May 18, 2004.

  DrawGraph.cpp
  Functions to transform a graph into DataViewer objects.

*************************************************************/

#include <cstring>
#include <ctype.h>

#include "DV.h"

#include "DrawGraph.h"
#include "ListNode.h"
#include "List.h"
#include "GraphPos.h"

// Builds the DataViewer objects representing the graph from the
// layout given by the array of points.
void make_graph(DVcontainerbase &cont, Graph *gr, Point *points,
		float sphere_radius, float line_width)
{
  DVspheres the_centers;
  DVlines the_edges;
  DVcontainerbase frame_cont;
  LCVMarray_2D<float> nodes(gr->vertexNr, 3); 
  LCVMarray_3D<float> edges(gr->edgeNr, 2, 4);

  build_vertices(nodes, gr, points);
  the_centers.set_data(nodes, sphere_radius);
  the_centers.color[0]=DVcolor(1,1,0);
  frame_cont.copy_child(the_centers);
  
  build_edges(edges, gr, points);
  the_edges.set_data(edges);
  the_edges.color[0]=DVcolor(0.2,0.2,1);
  the_edges.width[0]=line_width;
  the_edges.set_color_coordinate(3);
  frame_cont.copy_child(the_edges);

  cont.copy_child(frame_cont);
}

// Stores all the edges in the LCVMarray to be drawn as line segments.
void build_edges(LCVMarray_3D<float> &edges, Graph *gr, Point *points)
{
  int i, j, k, size=gr->edgeNr, size1=gr->vertexNr, the_edge=0;
  float dist;
  for (i=0; i<size1 && the_edge < size; i++){
    ListNode *start = gr->edgeList[i].head;
    while (start != NULL) {
      j = start->elem;
      if (i < j) {
	dist = start->weight - Distance(points[i], points[j]);

	edges[the_edge][0][0]=points[i].x;
	edges[the_edge][0][1]=points[i].y;
	edges[the_edge][0][2]=points[i].z;
	edges[the_edge][0][3]=dist;

	edges[the_edge][1][0]=points[j].x;
	edges[the_edge][1][1]=points[j].y;
	edges[the_edge][1][2]=points[j].z;
	edges[the_edge][1][3]=dist;

	the_edge++;
      }
      start = start->next;
    }
  }
}

// Stores all the vertices in the LCVMarray to be drawn as little
// spheres.
void build_vertices(LCVMarray_2D<float> &nodes, Graph *gr, Point *points)
{
  int i, j, size=gr->vertexNr;

  for (i=0; i<size; i++){
      nodes[i][0]=points[i].x;
      nodes[i][1]=points[i].y;
      nodes[i][2]=points[i].z;
  }
}

// Makes an output name from the input name for the mc_graph
void make_mc_out_name(const char *in_name, char *out_name, int option)
{
  // Turn /home/danav/develop/mc_graph/Problems/DrawGraph/dg50dv.txt
  // Into /home/danav/develop/mc_graph/Results/mc_graph/dg50bf.txt

  int last_slash=-1, i;
  char prob_name[5]="";
  for (i=0; in_name[i]!='\0'; i++)
    if (in_name[i] == '/')
      last_slash = i;
  for (i=last_slash+1; in_name[i]!='\0' && !isdigit(in_name[i]); i++)
    prob_name[i-last_slash-1] = in_name[i];
  prob_name[i-last_slash-1]='\0';
  if (option == 0)
    make_out_name(in_name, out_name, "mc_graph/", "bf", prob_name, 2);
  else if (option == 1)
    make_out_name(in_name, out_name, "mc_graph/", "r", prob_name, 2);
  else if (option == 2)
    make_out_name(in_name, out_name, "mc_graph/", "tv", prob_name, 2);
  else // if (option == 3)
    make_out_name(in_name, out_name, "mc_graph/", "cb", prob_name, 2);
}

// Makes an output name from the input name
void make_out_name(const char *in_name, char *out_name, 
		   const char *folder_name, const char *option,
		   const char *prob_type, int flevel)
{
  const char res_folder[] = "Results/";
  int size = strlen(in_name), i, count=0, out_size=0,
      res_size = strlen(res_folder),
      fname_size = strlen(folder_name),
      option_size = strlen(option),
      prob_size = strlen(prob_type),
      slashes[15] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

  for (i=0; i<size; i++)
    if (in_name[i] == '/')
      slashes[count++] = i;

  strncpy(out_name, in_name, slashes[count-flevel-1]+1);
  out_size = slashes[count-flevel-1]+1;
  strcpy(out_name+out_size, res_folder);
  out_size += res_size;
  strcpy(out_name+out_size, folder_name);
  out_size += fname_size;
  strcpy(out_name+out_size, prob_type);
  out_size += prob_size;
  
  for (i=slashes[count-1]+prob_size+1; i < size && isdigit(in_name[i]); i++)
    out_name[out_size++] = in_name[i];

  strcpy(out_name+out_size, option);
  out_size += option_size;
  strcpy(out_name+out_size, ".txt");
}
