/*******************************************************************

   Project: MC-Graph, a C++ implementation of genetic algorithms
            for graph drawing and visualization
   License: Creative Commons, Attribution
   Author:  Dana Vrajitoru
   File:    gl_draw.cc
   Update:  October 2022

   Some functions to help generating OpenGL commands.

*******************************************************************/

#include "gl_draw.h"
#include <cmath>
#include <cstdlib>
#include <iostream>

// Draws a polygon with a given normal and any number of vertices.
void gl_polygon(const vector3f& coords, const Point3f& normal)
{
    glBegin(GL_POLYGON);
    for (int i = 0; i < coords.size(); i++) {
        glNormal3f(normal[0], normal[1], normal[2]);
        glVertex3f(coords[i][0], coords[i][1], coords[i][2]);
    }
    glEnd();
}

// Draws a triangle strip from a set of coordinates and normals.
void gl_triangle_strip(const vector3f& coords, const vector3f& normals)
{
    glBegin(GL_TRIANGLE_STRIP);
    for (int i = 0; i < coords.size(); i++) {
        glNormal3f(normals[i][0], normals[i][1], normals[i][2]);
        glVertex3f(coords[i][0], coords[i][1], coords[i][2]);
    }
    glEnd();
}

// Draws a triangle strip from two set of coordinates and normals.
void gl_triangle_strip(const vector3f& coords1, const vector3f& coords2,
    const vector3f& normals1, const vector3f& normals2)
{
    glBegin(GL_TRIANGLE_STRIP);
    for (int i = 0; i < coords1.size(); i++) {
        glNormal3f(normals1[i][0], normals1[i][1], normals1[i][2]);
        glVertex3f(coords1[i][0], coords1[i][1], coords1[i][2]);
        glNormal3f(normals2[i][0], normals2[i][1], normals2[i][2]);
        glVertex3f(coords2[i][0], coords2[i][1], coords2[i][2]);
    }
    glEnd();
}

// Translation of the DataViewer line with ribbon object.
void gl_ribbon(const vector3f& line_coords, const vector3f& direction)
{
    int size = line_coords.size();
    vector3f normals1(size);
    vector3f normals2(size);
    vector3f normals3(size);
    vector3f border1(size);
    vector3f border2(size);
    int i, j;

    // Compute the ribbon borders and initialize the normals to 0.
    for (i = 0; i < size; i++)
        for (j = 0; j < 3; j++) {
            border1[i][j] = line_coords[i][j] + direction[i][j];
            border2[i][j] = line_coords[i][j] - direction[i][j];
            normals1[i][j] = 0;
            normals2[i][j] = 0;
            normals3[i][j] = 0;
        }

    // Compute all of the normals and normalize them.
    add_normals(border1, line_coords, normals1, normals2);
    add_normals(line_coords, border2, normals2, normals3);
    normalize_vectors(normals1);
    normalize_vectors(normals2);
    normalize_vectors(normals3);

    // Draw 2 triangle strips.
    gl_triangle_strip(border1, line_coords, normals1, normals2);
    gl_triangle_strip(line_coords, border2, normals2, normals3);
}

// Draws a triangle grid from two set of coordinates.
void gl_triangle_grid(const vector3f& coords1, const vector3f& coords2)
{
    int size = coords1.size();
    vector3f normals1(size);
    vector3f normals2(size);
    int i, j;

    // Initialize the normals to 0.
    for (i = 0; i < size; i++)
        for (j = 0; j < 3; j++) {
            normals1[i][j] = 0;
            normals2[i][j] = 0;
        }

    // Compute all of the normals and normalize them.
    add_normals(coords1, coords2, normals1, normals2);
    normalize_vectors(normals1);
    normalize_vectors(normals2);

    // Draw 1 triangle strips.
    gl_triangle_strip(coords1, coords2, normals1, normals2);
}

// Draws a strip of cones.
void gl_cone_strip(const vector3f& coords, float radia[], int precision)
{
    //static GLUquadricObj *Cone = NULL;
    //if (!Cone)
    //Cone = gluNewQuadric();
    GLUquadricObj* Cone = gluNewQuadric();
    int slices = 0, stacks = 0;

    // Determine the precision of the rendering.
    switch (precision) {
    case 1:
        gluQuadricDrawStyle(Cone, (GLenum)GLU_LINE);
        slices = 10;
        stacks = 1;
        break;
    case 2:
        gluQuadricDrawStyle(Cone, (GLenum)GLU_FILL);
        slices = 10;
        stacks = 1;
        break;
    case 3:
        gluQuadricDrawStyle(Cone, (GLenum)GLU_FILL);
        slices = 20;
        stacks = 2;
        break;
    }

    // Enable the normals for a smooth lightning.
    gluQuadricNormals(Cone, (GLenum)GLU_SMOOTH);
    glEnable(GL_LIGHTING);

    //Draw each of the cylinders
    for (int i = 0; i < coords.size() - 1; i++) {
        Point3f a = coords[i];
        Point3f b = coords[i + 1];
        Point3f c;
        Point3f nc;
        Point3f axis;
        float axis_norm, angle, c_norm;
        int l;
        glPushMatrix();
        // Move the origin to a
        for (l = 0; l < 3; l++)
            c[l] = b[l] - a[l];
        if (c[2] < 0) {
            glTranslatef(b[0], b[1], b[2]);
            for (l = 0; l < 3; l++)
                c[l] = -c[l];
        }
        else
            glTranslatef(a[0], a[1], a[2]);
        c_norm = c.norm();
        if (c_norm > 0.0001) {
            // Rotate about the cross product of normalized c and (0,0,1)
            // normalize c
            for (int l = 0; l < 3; l++)
                nc[l] = c[l] / c_norm;
            // Compute cross product.  It is easy since one of the vectors
            // is (0,0,1)
            axis[0] = -nc[1];
            axis[1] = nc[0];
            axis[2] = 0;
            axis_norm = axis.norm();
            if (axis_norm < 0.00001)
                angle = 0.0;
            else
                angle = (float)asin((float)axis_norm);
            glRotated((angle * 360) / (2 * M_PI), axis[0], axis[1], axis[2]);
            gluCylinder(Cone, radia[i], radia[i + 1],
                c_norm, slices, stacks);
        }
        glPopMatrix();
    }
    gluDeleteQuadric(Cone);
}

// Draws a strip of cylinders.
void gl_cylinder_strip(const vector3f& coords, float width, int precision)
{
    //static GLUquadricObj *Cylinder = NULL;
    //if (!Cylinder)
    //Cylinder = gluNewQuadric();
    GLUquadricObj* Cylinder = gluNewQuadric();
    int slices = 0, stacks = 0;

    // Determine the precision of the rendering.
    switch (precision) {
    case 1:
        gluQuadricDrawStyle(Cylinder, (GLenum)GLU_LINE);
        slices = 10;
        stacks = 1;
        break;
    case 2:
        gluQuadricDrawStyle(Cylinder, (GLenum)GLU_FILL);
        slices = 10;
        stacks = 1;
        break;
    case 3:
        gluQuadricDrawStyle(Cylinder, (GLenum)GLU_FILL);
        slices = 20;
        stacks = 2;
        break;
    }

    // Enable the normals for a smooth lightning.
    gluQuadricNormals(Cylinder, (GLenum)GLU_SMOOTH);
    glEnable(GL_LIGHTING);

    //Draw each of the cylinders
    for (int i = 0; i < coords.size() - 1; i++) {
        Point3f a = coords[i];
        Point3f b = coords[i + 1];
        Point3f c;
        Point3f nc;
        Point3f axis;
        float axis_norm, angle, c_norm;
        int l;
        glPushMatrix();
        // Move the origin to a
        for (l = 0; l < 3; l++)
            c[l] = b[l] - a[l];
        if (c[2] < 0) {
            glTranslatef(b[0], b[1], b[2]);
            for (l = 0; l < 3; l++)
                c[l] = -c[l];
        }
        else
            glTranslatef(a[0], a[1], a[2]);
        c_norm = c.norm();
        if (c_norm > 0.0001) {
            // Rotate about the cross product of normalized c and (0,0,1)
            // normalize c
            for (int l = 0; l < 3; l++)
                nc[l] = c[l] / c_norm;
            // Compute cross product.  It is easy since one of the vectors
            // is (0,0,1)
            axis[0] = -nc[1];
            axis[1] = nc[0];
            axis[2] = 0;
            axis_norm = axis.norm();
            if (axis_norm < 0.00001)
                angle = 0.0;
            else
                angle = (float)asin((float)axis_norm);
            glRotated((angle * 360) / (2 * M_PI), axis[0], axis[1], axis[2]);
            gluCylinder(Cylinder, width, width,
                c_norm, slices, stacks);
        }
        glPopMatrix();
    }
    gluDeleteQuadric(Cylinder);
}

// Draws one sphere with specified coordinates and axes.
void gl_sphere(GLfloat center_x, GLfloat center_y, GLfloat center_z,
    GLfloat radius, int precision)
{
    //static GLUquadricObj *Sphere = NULL;
    //if (Sphere == NULL)
    //Sphere = gluNewQuadric();
    GLUquadricObj* Sphere = gluNewQuadric();
    int slices = 0, stacks = 0;

    // Determine the precision of the rendering.
    switch (precision) {
    case 1:
        gluQuadricDrawStyle(Sphere, (GLenum)GLU_LINE);
        slices = 10;
        stacks = 10;
        break;
    case 2:
        gluQuadricDrawStyle(Sphere, (GLenum)GLU_FILL);
        slices = 10;
        stacks = 10;
        break;
    case 3:
        gluQuadricDrawStyle(Sphere, (GLenum)GLU_FILL);
        slices = 20;
        stacks = 20;
        break;
    }
    // Enable the normals for a smooth lightning.
    gluQuadricNormals(Sphere, (GLenum)GLU_SMOOTH);
    glEnable(GL_LIGHTING);

    glPushMatrix();
    glTranslatef(center_x, center_y, center_z);
    gluSphere(Sphere, radius, slices, stacks);
    glPopMatrix();
    gluDeleteQuadric(Sphere);
}

// Draws one ellipsoid with specified coordinates and axes.
void gl_ellipsoid(GLfloat center_x, GLfloat center_y, GLfloat center_z,
    GLfloat axis_x, GLfloat axis_y, GLfloat axis_z,
    int precision)
{
    //static GLUquadricObj *Sphere = NULL;
    //if (Sphere == NULL)
    //Sphere = gluNewQuadric();
    GLUquadricObj* Sphere = gluNewQuadric();
    int slices = 0, stacks = 0;

    // Determine the precision of the rendering.
    switch (precision) {
    case 1:
        gluQuadricDrawStyle(Sphere, (GLenum)GLU_LINE);
        slices = 10;
        stacks = 10;
        break;
    case 2:
        gluQuadricDrawStyle(Sphere, (GLenum)GLU_FILL);
        slices = 10;
        stacks = 10;
        break;
    case 3:
        gluQuadricDrawStyle(Sphere, (GLenum)GLU_FILL);
        slices = 20;
        stacks = 20;
        break;
    }

    // Enable the normals for a smooth lightning.
    gluQuadricNormals(Sphere, (GLenum)GLU_SMOOTH);
    glEnable(GL_LIGHTING);

    glPushMatrix();
    glTranslatef(center_x, center_y, center_z);
    glScalef(axis_x, axis_y, axis_z);
    gluSphere(Sphere, 1.0, slices, stacks);
    glPopMatrix();
    gluDeleteQuadric(Sphere);
}

// Mirror the vectors over the z axis.
void mirror_z(vector3f& coord)
{
    for (int i = 0; i < coord.size(); i++)
        coord[i][2] = -coord[i][2];
}

// Copy a mirror over the z axis of the first vector into the second
// one.
void mirror_z(const vector3f& coord, vector3f& coord1)
{
    for (int i = 0; i < coord.size(); i++) {
        coord1[i][0] = coord[i][0];
        coord1[i][1] = coord[i][1];
        coord1[i][2] = -coord[i][2];
    }
}

// Computes the normals based on two sets of points and adds them to
// two sets of normal vectors. These vectors should be normalized
// later.
void add_normals(const vector3f& coords1, const vector3f& coords2,
    vector3f& normals1, vector3f& normals2)
{
    Point3f nv;
    int i, size = coords1.size();
    for (i = 0; i < size - 1; i++) {
        nv.compute_normal_vector(coords1[i], coords2[i], coords1[i + 1]);
        normals1[i] += nv;      //    --
        normals2[i] += nv;      //    |/
        normals1[i + 1] += nv;
        nv.compute_normal_vector(coords1[i + 1], coords2[i], coords2[i + 1]);
        normals1[i + 1] += nv;
        normals2[i] += nv;      //     /|
        normals2[i + 1] += nv;    //     --
    }
}

// Normalizes each vector in the array.
void normalize_vectors(Point3f normals[], int size)
{
    for (int i = 0; i < size; i++)
        normals[i].normalize();
}

// Normalizes each vector in the array.
void normalize_vectors(vector3f& normals)
{
    for (int i = 0; i < normals.size(); i++)
        normals[i].normalize();
}

// Normalizes each vector in the matrix.
void normalize_vectors(matrix3f& normals)
{
    for (int i = 0; i < normals.size(); i++)
        for (int j = 0; j < normals[i].size(); j++)
            normals[i][j].normalize();
}

// this function computes the normaalized cross product of two vectors.
// It is assumed that all three vectors have length three (no checking is done)
void normcrossprod(const Point3f& v1, const Point3f& v2, Point3f& out)
{
    out[0] = v1[1] * v2[2] - v1[2] * v2[1];
    out[1] = v1[2] * v2[0] - v1[0] * v2[2];
    out[2] = v1[0] * v2[1] - v1[1] * v2[0];
    out.normalize();
}

// This function computes the normal to the plane determined by the 
// three points v0,v1,v2 (3 vectors).  I.e. it computes the cross product
// of of (v0-v1) and (v1-v2).
// It is assumed that all four vectors have length three (no checking is done)
void compute_normal_vector(const Point3f& v0, const Point3f& v1,
    const Point3f& v2, Point3f& out)
{
    Point3f d1, d2;
    for (int i = 0; i < 3; i++) {
        d1[i] = v0[i] - v1[i];
        d2[i] = v1[i] - v2[i];
    }
    out.normcrossprod(d1, d2);
}

// This function normalizes a vector of floats--we make it do anysize
// vector since it is easy.The size is 3 by default.
void normalize(GLfloat v[], int size)
{
    int i;
    float d = norm(v, size); // norm of vector
    if (d == 0.0) {
        // cerr << "Error:  Zero length vector in normalize." << endl; return;
        return;
    }
    for (i = 0; i < size; i++)
        v[i] /= d;
}

// Computes the norm of a vector of any size.The size is 3 by default.
float norm(GLfloat v[], int size)
{
    int i;
    float d = 0.0;
    for (i = 0; i < size; i++)
        d += v[i] * v[i];
    d = sqrt(d);
    return d;
}

// Computes the scalar product of two vectors of any size. The size is
// 3 by default.
float scalarprod(GLfloat v1[], GLfloat v2[], int size)
{
    float prod = 0.0;
    int i;

    for (i = 0; i < size; i++)
        prod += v1[i] * v2[i];
    return prod;
}
