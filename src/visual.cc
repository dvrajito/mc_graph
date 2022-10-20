/***************************************************************

   Project: MCG, implementation of the graph layout algorithm
            using genetic algorithms.
   Author:  Dana Vrajitoru
   File:    visual.cc
   Update:  October 2022

   The main function for a visualization program for the
   solutions.

***************************************************************/

#include <cstdio>
#include <cstdlib>
#include <iostream>
using namespace std;
#include <ctime>
#include <GL/glut.h>
#include "Graph.h"
#include "vis_interface.h"
#include "trackball.h"
#include "result_manip.h"
#include "Point.h"
#include "GraphPos.h"

int pwindow;
int winWidth = 500, winHeight = 500;
Graph* gr;
Point* points;
Point b1, b2, mid;

// The main for the visualization of the results.
int mainVisual(int argc, char** argv)
{
    char resultFile[80];
    bool resf = true;
    int which = 0;

    gr = Detect_read_graph(argc, argv, resultFile, resf);
    if (argc > 2)
        which = atoi(argv[2]);
    points = new Point[gr->vertexNr];
    if (!resf)
        Read_gr_layout(points, gr->vertexNr, resultFile);
    else
        if (which < 2)
            Read_res_layout(points, gr->vertexNr, resultFile);
        else
            Read_res_select(points, gr->vertexNr, resultFile, which);

    gr->Print();

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(winWidth, winHeight);
    pwindow = glutCreateWindow("Graph Layout Display");

    light_init();
    center_points(points, gr->vertexNr);
    BoundingBox(points, gr->vertexNr, b1, b2);
    mid = b1;
    mid += b2;
    mid *= 0.2;
    cout << "b1: " << b1 << " b2: " << b2 << " mid: " << mid << endl;
    center_graph();
    scene_init(gr, points);
    /* need both double buffering and z buffer */
    glutReshapeFunc(my_reshape);
    glutDisplayFunc(display);
    glutIdleFunc(NULL);
    glutMouseFunc(mouseButton);
    glutMotionFunc(mouseMotion);
    glutKeyboardFunc(skey);
    glutSpecialFunc(key);

    glutMainLoop();
    return 0;
}
