/**************************************************************

   Project: MC-Graph, a C++ implementation of genetic algorithms
            for graph drawing and visualization
   License: Creative Commons, Attribution
   Author:  Dana Vrajitoru
   File:    List.cc
   Updated: October 2022

   List class methods, list container for a doubly-linked list.

***************************************************************/

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
using namespace std;
#include "ListNode.h"
#include "List.h"

  // Constructors
List::List()
{
    Init();
}

// Copy constructor
List::List(List *data)
{
    Init(data);
}

// Copy constructor
List::List(List &data)
{
    Init(data);
}

// Init functions corresponding to each constructor
void List::Init()
{
    head = NULL;
    tail = NULL;
    size = 0;
}

// Copy the list data
void List::Init(List *data)
{
    Init(*data);
}

// Copy the list data
void List::Init(List &data)
{
    head = new ListNode(data.head); // deep copy
    tail = head->LastNode();
    size = data.size;
}

// Desctructor
List::~List()
{
    if (head != NULL)
        delete head;
    head = NULL;
    tail = NULL;
    size = 0;
}

// Delete everything in the list
void List::Empty()
{
    if (head != NULL) {
        ListNode *p = head;
        while (head) {
            head = head->next;
            delete p;
            p = head;
        }
    }
    tail = NULL;
    size = 0;
}

// Converts the list to Boolean. It returns true if the list is not
// empty, and false if it is.
List::operator bool()
{
    return size != 0;
}

// List manipulation

// Add a value to the front of the list
void List::AddHead(int val)
{
    ListNode *node;

    node = new ListNode(val);
    if (head == NULL)
        head = tail = node;
    else {
        head->AddHead(node);
        head = node;
    }
    size++;
}

// Add a value to the back of the list
void List::AddTail(int val)
{
    ListNode *node;

    node = new ListNode(val);
    if (tail == NULL)
        head = tail = node;
    else {
        tail->AddTail(node);
        tail = node;
    }
    size++;
}

// Remove the front of the list and return false if the list is empty.
bool List::RemoveHead()
{
    ListNode *node;

    if (head != NULL) {
        node = head;
        head = head->next;
        delete node;
        size--;
        if (size == 0)
            tail = NULL;
        return true;
    }
    else
        return false;
}

// Remove the back of the list and return false if the list is empty.
bool List::RemoveTail()
{
    ListNode *node;

    if (tail != NULL) {
        node = tail;
        tail = tail->prev;
        delete node;
        size--;
        if (size == 0)
            head = NULL;
        return true;
    }
    else
        return false;
}

// concatenate the list data to the target object
void List::Concatenate(List &data)
{
    if (head != data.head) // if the two lists are not the same
    {
        if (head) // if the first list is not empty
        {
            // call the ListNode function to add the second list 
            // to the tail of the first
            tail->Concatenate(data.head);
            size += data.size; // we also have to update the size
            if (data.tail)     // and the tail
                tail = data.tail;
        }
        else // if the first list is empty
        {
            // move the list from the second one into it
            head = data.head;
            size = data.size;
            tail = data.tail;
        }
        if (data) // if the second list was not empty, unlink/empty it
        {
            data.size = 0;
            data.head = NULL;
            data.tail = NULL;
        }
    }
    else // error
        cout << "Attepmt to concatenate a list to itself. "
             << "Operation ignored." << endl;
}

// Output to cout
void List::Print()
{
    cout << "The list has " << size << "nodes:" << endl;
    if (head != NULL)
        head->Print();
}

// Output to a file
void List::FPrint(FILE *aFile)
{
    fprintf(aFile, "The list has %d nodes:\n", size);
    if (head != NULL)
        head->FPrint(aFile);
}


// cout output operator
ostream &operator<<(ostream &out, List &data)
{
    out << "The list has " << data.size << " nodes:" << endl;
    ListNode *node = data.head;
    while (node) {
        out << node;
        node = node->next;
    }
    out << endl;
    return out;
}