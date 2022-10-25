/********************************************************************

   Project: MC-Graph, a C++ implementation of genetic algorithms
            for graph drawing and visualization
   License: Creative Commons, Attribution
   Authors: Dana Vrajitoru
            Jason DeBoni
   File:    ListNodeW.cc
   Updated: October 2022

   Nodes of a weighted linked list.

*********************************************************************/


#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <fstream>
using namespace std;
#include "ListNodeW.h"

// Constructors
ListNodeW::ListNodeW(int val)
{
    Init(val, 0);
}

ListNodeW::ListNodeW(int val, float cost)
{
    Init(val, cost);
}

ListNodeW::ListNodeW(ListNodeW* data)
{
    Init(data);
}

// Destructor
ListNodeW::~ListNodeW()
{
}

// Init corresponding to each constructor
void ListNodeW::Init(int val)
{
    next = NULL;
    prev = NULL;
    value = val;
    weight = 0;
}

// Init corresponding to each constructor
void ListNodeW::Init(int val, float cost)
{
    next = NULL;
    prev = NULL;
    value = val;
    weight = cost;
}

// Deep copy of the list
void ListNodeW::Init(ListNodeW* data)
{
    prev = data->prev;
    if (data->next != NULL)
        next = new ListNodeW(data->next);
    else
        next = NULL;
    value = data->value;
    weight = data->weight;
}

////////////////////// List operations /////////////////

// Add the node (param) at the front of the list containing the node this
void ListNodeW::AddHead(ListNodeW* node)
{
    if (prev == NULL) {
        prev = node;
        if (node != NULL)
            node->next = this;
    }
    else
        prev->AddHead(node);
}

// Add the node (param) at the end of the list starting 
// from the node this.
void ListNodeW::AddTail(ListNodeW* node)
{
    if (next == NULL) {
        next = node;
        if (node != NULL)
            node->prev = this;
    }
    else
        next->AddTail(node);
}

// Remove the first node in the list containing the node this. 
// Returns the first node in the new list.
ListNodeW* ListNodeW::RemoveHead()
{
    ListNodeW* node;

    if (prev != NULL)
        node = prev->RemoveHead();
    else {
        node = next;
        if (next != NULL) {
            next->prev = NULL;
            next = NULL;
        }
    }
    return node;
}

// Removes the last node in the list containing this.
// Returns the last node in the new list.
ListNodeW* ListNodeW::RemoveTail()
{
    ListNodeW* node;

    if (next != NULL)
        node = next->RemoveTail();
    else {
        node = prev;
        if (prev != NULL) {
            prev->next = NULL;
            prev = NULL;
        }
    }
    return node;
}

// returns a pointer to the last node in the list that *this is in.
ListNodeW* ListNodeW::LastNode()
{
    ListNodeW* n = this;
    while (n->next)
        n = n->next;
    return n;
}

// Concatenate a whole list at the end of the list starting with the
// target object.
void ListNodeW::Concatenate(ListNodeW* link)
{

    // check if we're trying to concatenate a list to itself
    if (this == link)
    {
        cout << "Attempt to concatenate a list to itself; operation aborted."
            << endl;
        return;
    }

    // go to the end of this list
    ListNodeW* p = LastNode(); // calling this from the target object.
    if (p) { // add the node here if the current list is not empty
        p->next = link;
        if (link)
            link->prev = p;
    }
}

// Find the node containing a value if it's there
ListNodeW* ListNodeW::Search(int val)
{
    ListNodeW* node = this;
    while (node && node->value != val)
        node = node->next;
    return node;
}

// Output the whole list
void ListNodeW::Print()
{
    ListNodeW* n = this;
    while (n) {
        cout << value << ' ';
        n = n->next;
    }
    cout << endl;
}

// Output the whole list to a file
void ListNodeW::FPrint(FILE* aFile)
{
    ListNodeW* n = this;
    while (n) {
        fprintf(aFile, "%d ", value);
        n = n->next;
    }
    fprintf(aFile, "\n");
}

// Output the whole list to a file
void ListNodeW::FPrint(ofstream &aFile)
{
    ListNodeW *n = this;
    while (n) {
        aFile << value << ' ';
        n = n->next;
    }
    aFile << endl;
}

// Deletes a list starting from the node
void DeleteList(ListNodeW *&head)
{
    ListNodeW *n = head, *temp;
    while (n) {
        temp = n->next;
        delete n;
        n = temp;
    }
    head = NULL;
}

// cout output operator
ostream& operator<<(ostream& out, ListNodeW& data)
{
    out << data.value << ' ' << data.weight << ' ';
    return out;
}

