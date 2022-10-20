/*************************************************************************

   Project: MC-Graph, a C++ implementation of genetic algorithms
            for graph drawing and visualization
   License: Creative Commons, Attribution
   Author:  Dana Vrajitoru
   File:    vis_interface.cc
   Update:  October 2022

   Some functions to manage the interaction with the OpenGL window.

*************************************************************************/


#include <cstdlib>
#include <GL/glut.h>
#include <iostream>
using namespace std;

#include "vis_interface.h"
#include "trackball.h"

extern int winWidth, winHeight;

static GLfloat theta[] = { 0.0,0.0,0.0 };
static GLint axis = 2;
static int direction = 0;
GLfloat trans[] = { 0.0,0.0,1 };

int graph_id;
float radius = 0.1, scalef = 1;
extern Graph* gr;
extern Point* points;
extern Point b1, b2, mid;
bool ortho = false;

// Main OpenGL function that is called whenever an event occurs.
void display()
{
    /* display callback, clear frame buffer and z buffer,
       rotate object and draw, swap buffers */
       // Apply the trackball transformation.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glLoadIdentity();
    if (!ortho)
        //gluLookAt(b1[0], mid[1]+0.1*scalef, mid[2]-1.5*scalef, b1[0], mid[1]+0.1*scalef, b1[2], 0.0, 1.0, 0.0);
        gluLookAt(b1[0], mid[1] + 0.1 * scalef, mid[2] - 1.5 * scalef, 0, mid[1] + 0.1 * scalef, b1[2], 0.0, 1.0, 0.0);
    //gluLookAt(0, 0, b2[2]-10, 0, 0, mid[2], 0.0, 1.0, 0.0);
    glPopMatrix();
    applyRotation();
    glPushMatrix();
    // The translation of the whole scene.
    glTranslatef(trans[0], trans[1], 0);
    glScalef(trans[2], trans[2], trans[2]);
    glPushMatrix();

    glCallList(graph_id);
    glPopMatrix();
    glutSwapBuffers();
}

// Centers the graph.
void center_graph()
{
    trans[0] -= mid[0];
    trans[1] -= mid[1];
    //trans[2] -= b1[2]+1;
    scalef = b2[0] - b1[0];
    if (b2[1] - b1[1] > scalef)
        scalef = b2[1] - b1[1];
    if (b2[2] - b1[2] > scalef)
        scalef = b2[2] - b1[2];
    if (radius < scalef / 100.0)
        radius = scalef / 100.0;
    cout << "scalef: " << scalef << " radius " << radius << endl;
}

// Mouse callback, selects an axis about which to rotate. When the
// appropriate mouse button is pushed, it starts to spin the scene and
// when the mouse button is released, the spinning stops.
/*
void mouse(int btn, int state, int x, int y)
{
  if (state != GLUT_DOWN)
    glutIdleFunc(NULL);
  else {
    glutIdleFunc(spin_objects);
    switch (btn) {
    case GLUT_LEFT_BUTTON:
      axis = 0;
      break;
    case GLUT_MIDDLE_BUTTON:
      axis = 1;
      break;
    case GLUT_RIGHT_BUTTON:
      axis = 2;
    }

    int key_state = glutGetModifiers();
    if (key_state == GLUT_ACTIVE_SHIFT)
      direction = 1;
    else
      direction = 0;
  }
}
*/

// A good view of the graph.
void default_view()
{
    theta[0] = 2.40001;
    theta[1] = 71.4001;
    theta[2] = 0;
    trans[0] = 0;
    trans[1] = -1.2;
    trans[2] = 0.45;
}

// Callback for ASCII keyboard characters.
void skey(unsigned char key, int x, int y)
{
    switch (key) {
    case 'v':
        cout << "angles: " << theta[0] << ' ' << theta[1] << ' ' << theta[2]
            << " trans: " << trans[0] << ' ' << trans[1] << ' ' << trans[2]
            << endl;
        break;
    case 'd':
        default_view();
        break;
    case 's':
        theta[0] = 2.40001;
        theta[1] = 71.4001;
        theta[2] = 20;
        trans[0] = 0;
        trans[1] = -1.2;
        trans[2] = 0.45;
        break;
    case 'e':
        theta[0] = 11.4;
        theta[1] = 71.4001;
        theta[2] = 0;
        trans[0] = 0.8;
        trans[1] = -1.2;
        trans[2] = 0.15;
        break;
    case 'b':
        theta[0] = 25.4;
        theta[1] = 51.4001;
        theta[2] = 0;
        break;
    case 'q':
    case 'Q':
        exit(0);
        break;
    case '\t':
        ortho = !ortho;
        my_reshape(winWidth, winHeight);
        //light_init();
        break;
    }
    glutPostRedisplay();
}

// Callback for the keyboard. The arrow keys move the entire scene on
// x and y. The page up and page down keys move the scene on z.
void key(int key, int x, int y)
{
    int key_state = glutGetModifiers();
    if (key_state == GLUT_ACTIVE_SHIFT) {
        switch (key) {
        case GLUT_KEY_UP:
            resize_ball(1);
            break;
        case GLUT_KEY_DOWN:
            resize_ball(-1);
            break;
        }
    }
    //  else if(key_state == GLUT_ACTIVE_CTRL){
    else {
        switch (key) {
        case GLUT_KEY_LEFT:
            trans[0] += 0.1;
            break;
        case GLUT_KEY_RIGHT:
            trans[0] -= 0.1;
            break;
        case GLUT_KEY_UP:
            trans[1] += 0.1;
            break;
        case GLUT_KEY_DOWN:
            trans[1] -= 0.1;
            break;
        case GLUT_KEY_PAGE_UP:
            trans[2] += 0.05;
            break;
        case GLUT_KEY_PAGE_DOWN:
            trans[2] -= 0.05;
            break;
        case GLUT_KEY_HOME:
            break;
        case GLUT_KEY_END:
            break;
        case GLUT_KEY_INSERT:
            break;
        }
    }
    glutPostRedisplay();
}

// To be called whenever the window is resized.
void my_reshape(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //gluPerspective(60.0, w/h, b1[2]-scalef, b2[2]+scalef); 
    /*if (w<=h)
      glFrustum(b1[0]-0.5, b2[0]+0.5, b1[1] * (GLfloat) h/ (GLfloat) w-0.5,
            b2[1] * (GLfloat) h / (GLfloat) w+0.5, b1[2]-2.5, b2[2]+2.5);
    else
      glFrustum(b1[0]-0.5, b2[0], b1[1] * (GLfloat) w/ (GLfloat) h-0.5,
            b2[1] * (GLfloat) w / (GLfloat) h+0.5, b1[2]-2.5, b2[2]+2.5);
    */

    if (ortho) {
        if (w <= h) //glFrustum
            glOrtho(-0.6 * scalef, 0.6 * scalef, -0.6 * scalef * (GLfloat)h / (GLfloat)w,
                0.6 * scalef * (GLfloat)h / (GLfloat)w, -0.6 * scalef, 0.6 * scalef);
        else
            glOrtho(-0.6 * scalef * (GLfloat)w / (GLfloat)h,
                0.6 * scalef * (GLfloat)w / (GLfloat)h, 0.0, 0.6 * scalef, -0.6 * scalef, 0.6 * scalef);
    }
    else {
        /*if (w <= h) //glFrustum
          glFrustum(-0.6*scalef, 0.6*scalef, -0.6*scalef * (GLfloat) h / (GLfloat) w,
                    0.6*scalef * (GLfloat) h / (GLfloat) w, -0.6*scalef, 0.6*scalef);
        else
          glFrustum(-0.6*scalef * (GLfloat) w / (GLfloat) h,
                    0.6*scalef * (GLfloat) w / (GLfloat) h, 0.0, 0.6*scalef, -0.6*scalef, 0.6*scalef);
        */
        //gluPerspective(60.0, w/h, b1[2]-scalef, b2[2]+scalef); 
        gluPerspective(60.0, w / h, 1.0, b2[2] + 2 * scalef);
    }

    glMatrixMode(GL_MODELVIEW);
    winWidth = w;
    winHeight = h;
}

void light_init()
{
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_diffuse[] = { 0.0, 0.0, 1.0, 1.0 };
    GLfloat mat_ambient[] = { 0.0, 0.5, 1.0, 1.0 };
    GLfloat mat_shininess = { 100.0 };
    GLfloat light_ambient[] = { 0.5, 0.5, 0.5, 1.0 };
    GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };

    GLfloat light_position[] = { 0.0, 5.0, -2.0, 1.0 };

    /* set up ambient, diffuse, and specular components for light 0 */

    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    /* define material proerties for front face of all polygons */

        //glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
        //glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
        //glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
        //glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);

    glShadeModel(GL_SMOOTH); /*enable smooth shading */
    glEnable(GL_LIGHTING); /* enable lighting */
    glEnable(GL_LIGHT0);  /* enable light 0 */
    glEnable(GL_DEPTH_TEST); /* enable z buffer */
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    glClearColor(0.0, 0.0, 0.0, 1.0);
    glColor3f(1, 0.0, 0.0);
}

// Initialize all of the objects to be drawn.
void scene_init(Graph* gr, Point* points)
{
    graph_id = glGenLists(1);
    glNewList(graph_id, GL_COMPILE);
    draw_vertices(gr, points);
    draw_edges(gr, points);
    glEndList();
}

// Draws all the edges as line segments.
void draw_edges(Graph* gr, Point* points)
{
    int i, j, size = gr->vertexNr;

    glColor3f(0.5, 0.5, 1.0);
    glBegin(GL_LINES);

    for (i = 0; i < size; i++) {
        ListNode* start = gr->edgeList[i].head;
        while (start != NULL) {
            j = start->elem;
            if (i < j) {
                point_vert(points[i]);
                point_vert(points[j]);
            }
            start = start->next;
        }
    }
    glEnd();
}

// Draws all the vertices as spheres.
void draw_vertices(Graph* gr, Point* points)
{
    int i, size = gr->vertexNr;
    GLUquadricObj* Sphere = gluNewQuadric();
    int slices = 10, stacks = 10;

    // Enable the normals for a smooth lightning.
    gluQuadricNormals(Sphere, (GLenum)GLU_SMOOTH);
    //  glEnable(GL_LIGHTING);
    glColor3f(1.0, 1.0, 0.0);

    for (i = 0; i < size; i++) {
        glPushMatrix();
        glTranslatef(points[i].x, points[i].y, points[i].z);
        gluSphere(Sphere, radius, slices, stacks);
        glPopMatrix();
    }
    gluDeleteQuadric(Sphere);
}

// Call a gl vertex command using the point's coordinates.
void point_vert(Point& p)
{
    glVertex3f(p.x, p.y, p.z);
}

// Make the ball smaller or bigger. The parameter should be 1 to
// increase, -1 to decrease.
void resize_ball(int how)
{
    if (how == 1) {
        if (radius > 1)
            radius += 0.5;
        else if (radius > 0.1)
            radius += 0.2;
        else
            radius += 0.05;
    }
    else {
        if (radius > 2)
            radius -= 0.5;
        else if (radius > 0.4)
            radius -= 0.2;
        else
            radius *= 0.7;
    }
    cout << radius << endl;
    glDeleteLists(graph_id, 1);
    glNewList(graph_id, GL_COMPILE);
    draw_vertices(gr, points);
    draw_edges(gr, points);
    glEndList();
}
