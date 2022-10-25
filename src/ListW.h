/*******************************************************************

   Project: MC-Graph, a C++ implementation of genetic algorithms
			for graph drawing and visualization
   License: Creative Commons, Attribution
   Author:  Dana Vrajitoru
   File:    ListW.h
   Updated: October 2022

   List class definition, list container for a doubly-linked list.

********************************************************************/

#ifndef List_Weight_h
#define List_Weight_h

#include "ListNodeW.h"

class ListW {
public:
    ListNodeW *head;
    ListNodeW *tail;
    int size;

    // Constructors
    ListW();

    // Copy constructor
    ListW(ListW *data);

    // Copy constructor
    ListW(ListW &data);

    // Init functions corresponding to each constructor
    void Init();

    // Copy the list data
    void Init(ListW *data);

    // Copy the list data
    void Init(ListW &data);

    // Desctructor
    ~ListW();

    // Delete everything in the list
    void Empty();

    // Converts the list to Boolean. It returns true if the list is not
    // empty, and false if it is.
    operator bool();

    // Returns the value of the first node
    int Front();

    // Returns the value of the last node
    int Back();

    // Checks if the list is empty
    bool IsEmpty();

    // List manipulation

    // Add a value to the front of the list
    void InsertFront(int val, float cost = 0);

    // Add a value to the front of the list
    void InsertBack(int val, float cost = 0);

    // Remove the front of the list and return the value it contained
    bool RemoveFront();

    // Remove the back of the list and return false if the list is empty.
    bool RemoveBack();

    // concatenate the list data to the target object
    void Concatenate(ListW &data);

    // Checks if the list contains the value
    bool Contains(int val);
    
    // Searches for the value in the list and returns a pointer 
    // to the node containing it
    ListNodeW* Search(int val);

    // Output to cout
    void Print();

    // Output to a file
    void FPrint(FILE *aFile);

    // Output to a file
    void FPrint(ofstream& aFile);

    // cout output operator
    friend ostream &operator<<(ostream &out, ListW &data);
};

#endif
