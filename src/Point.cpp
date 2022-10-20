/******************************************************************

  Dana Vrajitoru
  Project MC-Graph.
  Last updated: May 28, 2004.

  Point.h 
  A class handling a 3D point and convenient point and vector
  operations.

*******************************************************************/

#include "Point.h"
#include "General.h"
#include <iostream>
using namespace std;
#include <cmath>


// Constructor with only one value.
Point::Point(float val)
{
  Init(val);
}

// Constructor with given values. Initialize the point either with
// given values for x, y, and z, if the first parameter is 0, or by
// random if the first parameter is not 0. In the second case the
// value of X is used to specify the radius of a cube within which the
// point is randomly generated. Each coordinate will have a value
// between 0 and valX.
Point::Point(int randVal, float valX, float valY, float valZ)
{
  Init(randVal, valX, valY, valZ);
}

// Copy constructor.
Point::Point(Point &data)
  : x(data.x), y(data.y), z(data.z)
{;}

// Initialize the point with a single value.
void Point::Init(float val)
{
  x = val;
  y = val;
  z = val;
}

// Initialize the point either with given values for x, y, and z, if
// the first parameter is 0, or by random if the first parameter is
// not 0. In the second case the value of X is used to specify the
// radius of a cube within which the point is randomly generated. Each
// coordinate will have a value between 0 and valX.
void Point::Init(int randVal, float valX, float valY, float valZ)
{
  if (!randVal) {
    x = valX;
    y = valY;
    z = valZ;
  }
  else {
    x = RealRand(10000)*valX;
    y = RealRand(10000)*valX;
    z = RealRand(10000)*valX;
  }
}

// Copy init function.
void Point::Init(Point &data)
{
  x = data.x;
  y = data.y;
  z = data.z;
}

// Writes out the coordinates of the point.
void Point::Print()
{
  cout << x << " " << y << " " << z << endl;
}

// An operator such that we can access the xyz components by an index
// too.
float &Point::operator[](int i)
{
  switch (i) {
  case 0: return x;
  case 1: return y;
  case 2: return z;
  default: 
    cerr << "Illegal access to a coordinate of a point" << endl
	 << "The coordinate was " << i << endl
	 << "x was returned by default." << endl;
    return x;
  }
}

// Add another point to the current one.
void Point::operator+=(Point &data)
{
  x += data.x;
  y += data.y;
  z += data.z;
}

// Vectorial difference between two point.
void Point::operator-=(Point &data)
{
  x -= data.x;
  y -= data.y;
  z -= data.z;
}

// Multiply the point by a scalar.
void Point::operator*=(float rate)
{
  x *= rate;
  y *= rate;
  z *= rate;
}

// comparison operators
bool Point::operator==(Point &data)
{
  return (x==data.x && y==data.y && z==data.z);
}

bool Point::operator!=(Point &data)
{
  return !(*this == data);
}

// Assignment operator
/*Point &Point::operator=(Point &data)
{
  if (this != &data)
    Init(data);
  return *this;
}*/

// Assignment operator
Point &Point::operator=(Point data)
{
  if (this != &data) {
    x=data.x;
    y=data.y;
    z=data.z;
  }
  return *this;
}

// Compute the norm of the vector.
float Point::Norm()
{
  return sqrt(x*x+y*y+z*z);
}

// Divide the components of the point by the norm to obtain a unit
// vector.
void Point::Normalize()
{
  float nrm = Norm();
  //  if (nrm <= 0.0001 || nrm >= 1e30)
  //    cout << "norm: " << nrm << " before: " 
  //	 << x << ' ' << y << ' ' << z << endl;
  if (nrm != 0) {
    x /= nrm;
    y /= nrm;
    z /= nrm;
  }
  //  if (nrm <= 0.0001 || nrm >= 1e30)
  //   cout << "after: " 
  //	 << x << ' ' << y << ' ' << z << endl;
}

// to be able to input and output a point with the usual << and >>
// operators.
ostream &operator<< (ostream &out, Point &data)
{
  out << data.x << " " << data.y << " " << data.z;
  return out;
}

istream &operator>> (istream &in, Point &data)
{
  in >> data.x >> data.y >> data.z;
  return in;
}

// Same as the class method, but taking the point as parameter.
void Print(Point *data, int size)
{
  cout << size << " Points:" << endl;
  for (int i=0; i<size; i++)
    cout << "<" << data[i] << "> ";
  cout << endl;
}
