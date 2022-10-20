/*************************************************************************

   Project: MC-Graph, a C++ implementation of genetic algorithms
            for graph drawing and visualization
   License: Creative Commons, Attribution
   Author:  Dana Vrajitoru
   File:    Point.h
   Update:  October 2022

   A class handling a 3D point and convenient point and vector
   operations.

*************************************************************************/

#ifndef POINT_H
#define POINT_H

#include <iostream>
using namespace std;

class Point {
public:
    float x, y, z;

    // Constructor with only one value.
    Point(float val = 0.0);
    // Constructor with given values. Initialize the point either with
    // given values for x, y, and z, if the first parameter is 0, or by
    // random if the first parameter is not 0. In the second case the
    // value of X is used to specify the radius of a cube within which
    // the point is randomly generated. Each coordinate will have a
    // value between 0 and valX.
    Point(int randVal, float valX, float valY = 0.0, float valZ = 0.0);
    // Copy constructor.
    Point(Point& data);

    // Initialize the point with a single value.
    void Init(float val = 0.0);
    // Initialize the point either with given values for x, y, and z, if
    // the first parameter is 0, or by random if the first parameter is
    // not 0. In the second case the value of X is used to specify the
    // radius of a cube within which the point is randomly
    // generated. Each coordinate will have a value between 0 and valX.
    void Init(int randVal, float valX, float valY = 0.0, float valZ = 0.0);
    // Copy init function.
    void Init(Point& data);

    // Compute the norm of the vector.
    float Norm();
    // Divide the components of the point by the norm to obtain a unit
    // vector.
    void Normalize();

    // Writes out the coordinates of the point.
    void Print();

    // An operator such that we can access the xyz components by an
    // index too.
    float& operator[](int i);

    // Add another point to the current one.
    void operator+=(Point& data);
    // Vectorial difference between two point.
    void operator-=(Point& data);
    // Multiply the point by a scalar.
    void operator*=(float rate);

    // comparison operators
    bool operator==(Point& data);
    bool operator!=(Point& data);
    // Assignment operator
    Point& operator=(Point data);

    // to be able to input and output a point with the usual << and >>
    // operators.
    friend ostream& operator<< (ostream& out, Point& data);
    friend istream& operator>> (istream& in, Point& data);

    // Same as the class method, but taking the point as parameter.
    friend void Print(Point* data, int size);

    // Finds the bounding box for an array of points.
    friend void BoundingBox(Point* points, int size, Point& v1, Point& v2);
};

#endif
