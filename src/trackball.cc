/*************************************************************************

   Project: MC-Graph, a C++ implementation of genetic algorithms
            for graph drawing and visualization
   License: Creative Commons, Attribution
   Author:  Dana Vrajitoru*
   File:    trackball.cc
   Update:  October 2022

   Some useful functions to define a trackball mouse interface.
   * Author: Adapted by Dana Vrajitoru from the E. Angel textbook CD.

*************************************************************************/

#include <stdlib.h>
#include <cmath>
#include <iostream>
using namespace std;
#include <GL/glut.h>
#include "trackball.h"

#define M_PI 3.14159

float lastPos[3] = { 0.0F, 0.0F, 0.0F };
int curx, cury;
int startX, startY;
extern GLfloat trans[3];
float transfactor = 0.05;
int curTx, curTy;
int startTX, startTY;

extern int winWidth, winHeight;

float 	angle = 0.0, axis[3];
bool 	trackingMouse = false;
bool 	redrawContinue = false;
bool    trackballMove = false;

bool 	trackingTrans = false;
bool 	redrawTrans = false;
bool    trackballTrans = false;

// Prototypes not present in the header file because they're not meant
// to be used outside of this file.
void trackball_ptov(int x, int y, int width, int height, float v[3]);
void startMotion(int x, int y);
void stopMotion(int x, int y);

// Determine a point on the 3D trackball based on the 2D position
void trackball_ptov(int x, int y, int width, int height, float v[3])
{
    float d, a;

    /* project x,y onto a hemi-sphere centered within width, height */
    v[0] = (2.0F * x - width) / width;
    v[1] = (height - 2.0F * y) / height;
    d = (float)sqrt(v[0] * v[0] + v[1] * v[1]);
    v[2] = (float)cos((M_PI / 2.0F) * ((d < 1.0F) ? d : 1.0F));
    a = 1.0F / (float)sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
    v[0] *= a;
    v[1] *= a;
    v[2] *= a;
}

// To be called when the mouse is moving.
void mouseMotion(int x, int y)
{
    float curPos[3], dx, dy, dz;

    y = winHeight - y;
    trackball_ptov(x, y, winWidth, winHeight, curPos);
    if (trackingMouse)
    {
        dx = curPos[0] - lastPos[0];
        dy = curPos[1] - lastPos[1];
        dz = curPos[2] - lastPos[2];

        if (dx || dy || dz) {
            angle = 0.5 * 90.0F * sqrt(dx * dx + dy * dy + dz * dz);

            axis[0] = lastPos[1] * curPos[2] - lastPos[2] * curPos[1];
            axis[1] = lastPos[2] * curPos[0] - lastPos[0] * curPos[2];
            axis[2] = lastPos[0] * curPos[1] - lastPos[1] * curPos[0];

            lastPos[0] = curPos[0];
            lastPos[1] = curPos[1];
            lastPos[2] = curPos[2];
        }
    }
    else if (trackingTrans) {
        dx = startTX - x;
        dy = startTY - y;
        if (dx || dy) {
            trans[0] += transfactor * dx;
            trans[1] -= transfactor * dy;
        }
        startTX = x;
        startTY = y;
    }
    glutPostRedisplay();
}

// Called when the user clicks to start the object's motion.
void startMotion(int x, int y)
{

    trackingMouse = true;
    redrawContinue = false;
    startX = x;
    startY = y;
    curx = x;
    cury = y;
    trackball_ptov(x, y, winWidth, winHeight, lastPos);
    trackballMove = true;
}

// Called when the user clicks to stop the object's motion.
void stopMotion(int x, int y)
{

    trackingMouse = false;

    if (startX != x || startY != y) {
        redrawContinue = true;
    }
    else {
        angle = 0.0F;
        redrawContinue = false;
        trackballMove = false;
    }
}

// Called when the user clicks to start the object's motion.
void startTrans(int x, int y)
{
    trackingTrans = true;
    redrawTrans = false;
    startTX = x;
    startTY = y;
    curTx = x;
    curTy = y;
    //trackball_ptov(x, y, winWidth, winHeight, lastPos);
    trackballTrans = true;
}

// Called when the user clicks to stop the object's motion.
void stopTrans(int x, int y)
{

    trackingTrans = false;

    if (startX != x || startY != y) {
        redrawTrans = true;
    }
    else {
        redrawTrans = false;
        trackballTrans = false;
    }
}

// To be called from the display to apply the rotation defined by the
// trackball.
void applyRotation()
{
    /* view transform */

    if (trackballMove) {
        glRotatef(angle, axis[0], axis[1], axis[2]);
    }
    spinObject();
}

// To be called when the user click or un-clicks the mouse button.
void mouseButton(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON)
        switch (state) {
        case GLUT_DOWN:
            y = winHeight - y;
            startMotion(x, y);
            break;
        case GLUT_UP:
            y = winHeight - y;
            stopMotion(x, y);
            break;
        }
    else if (button == GLUT_RIGHT_BUTTON) {
        switch (state) {
        case GLUT_DOWN:
            y = winHeight - y;
            startTrans(x, y);
            break;
        case GLUT_UP:
            y = winHeight - y;
            stopTrans(x, y);
            break;
        }
    }
}

// Spinning the objects - it's being called inside the applyRotation
// and can also be used as the iddle function.
void spinObject()
{
    if (redrawContinue) 
        glutPostRedisplay();
}
