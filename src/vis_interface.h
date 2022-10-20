/***************************************************************

   Project: MCG, implementation of the graph layout algorithm 
            using genetic algorithms.
   License: Creative Commons, Attribution
   Author:  Dana Vrajitoru
   File:    vis_interface.h
   Update:  October 2022
   
   Some functions to manage the interaction with the OpenGL window.
  
***************************************************************/
/* 
   Project: graph drawing
   Dana Vrajitoru
   vis_interface.h
   
   Some functions to manage the interaction with the OpenGL window.
*/
 
#ifndef VIS_INTERFACE_H
#define VIS_INTERFACE_H

#include <GL/glut.h>
#include "Graph.h"
#include "Point.h"

// Main OpenGL function that is called whenever an event occurs.
void display();

// Spins the entire scene by 2 degrees about selected axis.
void spin_objects();

// Centers the graph.
void center_graph();

// Mouse callback, selects an axis about which to rotate. When the
// appropriate mouse button is pushed, it starts to spin the scene and
// when the mouse button is released, the spinning stops.
//void mouse(int btn, int state, int x, int y);

// A good view of the motorcycle.
void default_view();

// Callback for the keyboard. The arrow keys move the entire scene on
// x and y. The page up and page down keys move the scene on z.
void key(int key, int x, int y);

// Callback for ASCII keyboard characters.
void skey(unsigned char key, int x, int y);

// To be called whenever the window is resized.
void my_reshape(int w, int h);

// Initialize the light settings.
void light_init();

// Initialize all of the objects to be drawn.
void scene_init(Graph *gr, Point *points);

// Draws all the edges as line segments.
void draw_edges(Graph *gr, Point *points);

// Draws all the vertices as spheres.
void draw_vertices(Graph *gr, Point *points);

// Call a gl vertex command using the point's coordinates.
void point_vert(Point &p);

// Make the ball smaller or bigger. The parameter should be 1 to
// increase, -1 to decrease.
void resize_ball(int how);

#endif
