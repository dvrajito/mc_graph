/*************************************************************************

   Project: MC-Graph, a C++ implementation of genetic algorithms
            for graph drawing and visualization
   License: Creative Commons, Attribution
   Author:  Dana Vrajitoru
   File:    pq_vertex.h
   Update:  October 2022

   Implementation of a priority queue containing vertices.

*************************************************************************/

#ifndef PQ_VERTEX_H
#define PQ_VERTEX_H

#include "pqueue.h"

class vertex_weight {
public:
    int vertex;
    float weight;

    // constructor
    vertex_weight(int v = 0, float w = 0.0);
    // reassigning the values
    void set_data(int v = 0, float w = 0.0);

    // if we need the assignment at any point.
    vertex_weight& operator=(const vertex_weight& other);

    // Comparison operators so that we can declare a priority queue
    // class containingit.
    bool operator<(const vertex_weight& other);
    bool operator>(const vertex_weight& other);
    bool operator<=(const vertex_weight& other);
    bool operator>=(const vertex_weight& other);
    bool operator==(const vertex_weight& other);
    bool operator!=(const vertex_weight& other);
};

typedef priority_queue<vertex_weight> pq_vertex;

#endif
