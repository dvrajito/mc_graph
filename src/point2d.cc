/********************************************************************
   Project: Gl Visualizer
   License: Creative Commons, Attribution
   Author:  Dana Vrajitoru
   File:    point2d.cc
   Updated: March 2019

  Implementation of a class of 2D points with integer coordinates.
.
********************************************************************/

#include <GL/glut.h>
#include "point2d.h"
#include <iostream>
using namespace std;
#include <cstdlib>

// Constructor with values for x and y, can be used as default.
Point2d::Point2d(int valx, int valy)
{
  x = valx;
  y = valy;
}

// Copy constructor
Point2d::Point2d(Point2d &data)
{
  if (this != &data) {
    x = data.x;
    y = data.y;
  }
}

// Reinitialize the point later to some other values.
void Point2d::set_data(int valx, int valy)
{
  x = valx;
  y = valy;  
}

// Asignment operator.
Point2d &Point2d::operator=(Point2d &data)
{
  if (this != &data) {
    x = data.x;
    y = data.y;
  }
  return *this;
}

// Addition operator
Point2d &Point2d::operator+=(Point2d &data)
{
  x += data.x;
  y += data.y;
  return *this;
}

// Operator that allows us to use x and y as elements 0 and 1.
int &Point2d::operator[](int index)
{
  if (index == 0)
    return x;
  else if (index == 1)
    return y;
  else {
    cout << "illegal use of operator [] in a Point2d with subscript " 
	 << index << endl;
    exit(1);
  }
}

// a useful shortcut
void Point2d::gl_translate()
{
  glTranslated(x, y, 0);
}

// a useful shortcut
void Point2d::gl_vertex()
{
  glVertex2d(x, y);
}
