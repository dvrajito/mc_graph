/********************************************************************

   Project: MC-Graph, a C++ implementation of genetic algorithms
            for graph drawing and visualization
   License: Creative Commons, Attribution
   Author:  Dana Vrajitoru
   File:    ListNode.cc
   Updated: October 2022

   ListNode class methods, a doubly-linked list node

  *******************************************************************/

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
using namespace std;

#include "ListNode.h"

// Constructors
ListNode::ListNode(int val)
{
    Init(val);
}

// Deep copy constructor 
ListNode::ListNode(ListNode *data)
{
    Init(data);
}

// Destructor
ListNode::~ListNode()
{ ; }

// Init corresponding to each constructor
void ListNode::Init(int val)
{
    next = NULL;
    pred = NULL;
    elem = val;
}

// Initialize as a deep copy of another node
void ListNode::Init(ListNode *data)
{
    elem = data->elem;
    pred = data->pred;
    if (data->next != NULL)
        next = new ListNode(data->next); // copy the rest of the list
    else
        next = NULL;
}

// List operations

// Add the parameter node as head of the list that *this belongs to.
void ListNode::AddHead(ListNode *node)
{
    if (pred == NULL) {
        pred = node;
        if (node != NULL)
            node->next = this;
    }
    else
        pred->AddHead(node);
}

// Add the parameter node as tail of the list that *this belongs to.
void ListNode::AddTail(ListNode *node)
{
    if (next == NULL) {
        next = node;
        if (node != NULL)
            node->pred = this;
    }
    else
        next->AddTail(node);
}

// Remove the head of the list that *this belongs to and return it.
ListNode *ListNode::RemoveHead()
{
    ListNode *node;

    if (pred != NULL)
        node = pred->RemoveHead();
    else {
        node = next;
        if (next != NULL) {
            next->pred = NULL;
            next = NULL;
        }
    }
    return node;
}

// Remove the tail of the list that *this belongs to and return it.
ListNode *ListNode::RemoveTail()
{
    ListNode *node;

    if (next != NULL)
        node = next->RemoveTail();
    else {
        node = pred;
        if (pred != NULL) {
            pred->next = NULL;
            pred = NULL;
        }
    }
    return node;
}


// returns a pointer to the last node in the list that *this is in.
ListNode *ListNode::LastNode()
{
    ListNode *nd = this;
    while (nd->next)
        nd = nd->next;
    return nd;
}

// Concatenate a whole list at the end of the list starting from the
// target object.
void ListNode::Concatenate(ListNode *link)
{
    // check if we're trying to concatenate a list to itself
    if (this == link)
    {
        cout << "Attempt to concatenate a list to itself; operation aborted."
            << endl;
        return;
    }

    // go to the end of this list
    ListNode *p = LastNode(); // calling this from the target object.
    if (p) { // add the node here if the current list is not empty
        p->next = link;
        if (link)
            link->pred = p;
    }
} // ListNode::concatenate()

// Output the whole list to cout
void ListNode::Print()
{
    cout << elem << ' ';
    if (next == NULL)
        cout << endl;
    else
        next->Print();
}

// Output the whole list to a file
void ListNode::FPrint(FILE *aFile)
{
    fprintf(aFile, "%d ", elem);
    if (next == NULL)
        fprintf(aFile, "\n");
    else
        next->FPrint(aFile);
}

// cout output operator
ostream &operator<<(ostream &out, ListNode &data)
{
    out << data.elem << ' ';
    return out;
}








