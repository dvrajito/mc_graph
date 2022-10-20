/*************************************************************************

   Project: MC-Graph, a C++ implementation of genetic algorithms
            for graph drawing and visualization
   License: Creative Commons, Attribution
   Author:  Dana Vrajitoru*
   File:    trackball.h
   Update:  October 2022

   Some useful functions to define a trackball mouse interface.
   * Author: Adapted by Dana Vrajitoru from the E. Angel textbook CD.

*************************************************************************/

#ifndef TRACKBALL_H
#define TRACKBALL_H

// To be called from the display to apply the rotation defined by the
// trackball.
void applyRotation();

// Continues to spin the object if it has to. To be used as the iddle
// function. The applyRotation is calling it too.
void spinObject();

// To be called when the user click a mouse button.
void mouseButton(int button, int state, int x, int y);

// To be called when the mouse is moving.
void mouseMotion(int x, int y);

#endif
