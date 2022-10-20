/*******************************************************************

   Project: MC-Graph, a C++ implementation of genetic algorithms
			for graph drawing and visualization
   License: Creative Commons, Attribution
   Author:  Dana Vrajitoru
   File:    List.h
   Updated: October 2022

   List class definition, list container for a cdoubly-linked list.

********************************************************************/

#ifndef List_h
#define List_h

#include "ListNode.h"

class List {
public:
    ListNode *head;
    ListNode *tail;
    int size;

    // Constructors
    List();

    // Copy constructor
    List(List *data);

    // Copy constructor
    List(List &data);

    // Init functions corresponding to each constructor
    void Init();

    // Copy the list data
    void Init(List *data);

    // Copy the list data
    void Init(List &data);

    // Desctructor
    ~List();

    // Delete everything in the list
    void Empty();

    // Converts the list to Boolean. It returns true if the list is not
    // empty, and false if it is.
    operator bool();

    // List manipulation

    // Add a value to the front of the list
    void AddHead(int val);

    // Add a value to the front of the list
    void AddTail(int val);

    // Remove the front of the list and return the value it contained
    bool RemoveHead();

    // Remove the back of the list and return false if the list is empty.
    bool RemoveTail();

    // concatenate the list data to the target object
    void Concatenate(List &data);

    // Output to cout
    void Print();

    // Output to a file
    void FPrint(FILE *aFile);

    // cout output operator
    friend ostream &operator<<(ostream &out, List &data);
};

#endif
