/********************************************************************
   Project: Gl Visualizer
   License: Creative Commons, Attribution
   Author:  Dana Vrajitoru
   File:    point2d.h
   Updated: March 2019

   Definition of a class of 2D points with integer coordinates.

********************************************************************/

#ifndef POINT2D_H
#define POINT2D_H

class Point2d {
 public:
  int x, y;

  // Constructor with values for x and y, can be used as default.
  Point2d(int valx=0, int valy=0);
  // Copy constructor
  Point2d(Point2d &data);

  // Reinitialize the point later to some other values.
  void set_data(int valx=0, int valy=0);

  // Asignment operator.
  Point2d &operator=(Point2d &data);

  // Addition operator
  Point2d &operator+=(Point2d &data);
  
  // Operator that allows us to use x and y as elements 0 and 1.
  int &operator[](int index);

  // a useful shortcuts
  void gl_translate();
  void gl_vertex();
};

#endif
