/*******************************************************************

   Project: MC-Graph, a C++ implementation of genetic algorithms
			for graph drawing and visualization
   License: Creative Commons, Attribution
   Author:  Dana Vrajitoru
   File:    gl_draw.h
   Update:  October 2022

   Some functions to help generating OpenGL commands.

*******************************************************************/

 
#ifndef GL_DRAW_H
#define GL_DRAW_H

#define M_PI 3.14159

#include <GL/glut.h>
#include "point3f.h"

// Draws a polygon with a given normal and any number of vertices.
void gl_polygon(const vector3f &coords, const Point3f &normal);

// Draws a triangle strip from a set of coordinates and normals.
void gl_triangle_strip(const vector3f &coords, const vector3f &normals);

// Draws a triangle strip from two set of coordinates and normals.
void gl_triangle_strip(const vector3f &coords1, const vector3f &coords2, 
		       const vector3f &normals1, const vector3f &normals2);

// Draws a triangle grid from two set of coordinates.
void gl_triangle_grid(const vector3f &coords1, const vector3f &coords2);

// Translation of the DataViewer line with ribbon object.
void gl_ribbon(const vector3f &line_coords, const vector3f &direction);

// Draws a strip of cylinders.
void gl_cylinder_strip(const vector3f &coords, float width, int precision);

// Draws a strip of cones.
void gl_cone_strip(const vector3f &coords, float radia[], int precision);

// Draws one ellipsoid with specified coordinates and axes.
void gl_ellipsoid(GLfloat center_x, GLfloat center_y, GLfloat center_z, 
		  GLfloat axis_x, GLfloat axis_y, GLfloat axis_z, 
		  int precision);

// Draws one sphere with specified coordinates and axes.
void gl_sphere(GLfloat center_x, GLfloat center_y, GLfloat center_z, 
	       GLfloat radius, int precision);

// Mirror the vectors over the z axis.
void mirror_z(vector3f &coord);

// Copy a mirror over the z axis of the first vector into the second
// one.
void mirror_z(const vector3f &coord, vector3f &coord1);

// Computes the normals based on two sets of points and adds them to
// two sets of normal vectors. These vectors should be normalized
// later.
void add_normals(const vector3f &coords1, const vector3f &coords2, 
		 vector3f &normals1, vector3f &normals2);

// Normalizes each vector in the array.
void normalize_vectors(Point3f normals[], int size);

// Normalizes each vector in the array.
void normalize_vectors(vector3f &normals);

// Normalizes each vector in the matrix.
void normalize_vectors(matrix3f &normals);

// this function computes the normaalized cross product of two vectors.
// It is assumed that all three vectors have length three (no checking is done)
void normcrossprod(const Point3f &v1, const Point3f &v2, Point3f &out);

// This function computes the normal to the plane determined by the 
// three points v0,v1,v2 (3 vectors).  I.e. it computes the cross product
// of of (v0-v1) and (v1-v2).
// It is assumed that all four vectors have length three (no checking is done)
void compute_normal_vector(const Point3f &v0, const Point3f &v1, 
			   const Point3f &v2, Point3f &out);

// This function normalizes a vector of floats--we make it do anysize
// vector since it is easy.The size is 3 by default.
void normalize(GLfloat v[], int size = 3);

// Computes the norm of a vector of any size.The size is 3 by default.
float norm(GLfloat v[], int size = 3);

// Computes the scalar product of two vectors of any size. The size is
// 3 by default.
float scalarprod(GLfloat v1[], GLfloat v2[], int size = 3);

#endif
