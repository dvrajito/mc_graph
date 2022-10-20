/*************************************************************************

   Project: MC-Graph, a C++ implementation of genetic algorithms
            for graph drawing and visualization
   License: Creative Commons, Attribution
   Author:  Dana Vrajitoru
   File:    pq_vertex.cc
   Update:  October 2022

   Implementation of a priority queue containing vertices.

*************************************************************************/


#include "pq_vertex.h"

// constructor
vertex_weight::vertex_weight(int v, float w)
{
    vertex = v;
    weight = w;
}

// reassigning the values
void vertex_weight::set_data(int v, float w)
{
    vertex = v;
    weight = w;
}


// if we need the assignment at any point.
vertex_weight& vertex_weight::operator=(const vertex_weight& other)
{
    if (this != &other) {
        vertex = other.vertex;
        weight = other.weight;
    }
    return *this;
}


// Comparison operators so that we can declare a priority queue
// class containingit.
bool vertex_weight::operator<(const vertex_weight& other)
{
    return (weight < other.weight);
}

bool vertex_weight::operator>(const vertex_weight& other)
{
    return (weight > other.weight);
}

bool vertex_weight::operator<=(const vertex_weight& other)
{
    return (weight <= other.weight);
}

bool vertex_weight::operator>=(const vertex_weight& other)
{
    return (weight >= other.weight);
}

bool vertex_weight::operator==(const vertex_weight& other)
{
    return (vertex == other.vertex && weight == other.weight);
}

bool vertex_weight::operator!=(const vertex_weight& other)
{
    return (vertex != other.vertex || weight != other.weight);
}
