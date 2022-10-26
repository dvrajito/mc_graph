/*******************************************************************

   Project: MC-Graph, a C++ implementation of genetic algorithms
            for graph drawing and visualization
   License: Creative Commons, Attribution
   Author:  Dana Vrajitoru
   File:    ListW.cc
   Updated: October 2022

   ListW class definition, ListW container for a doubly-linked ListW.

********************************************************************/

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <fstream>
using namespace std;
#include "ListNodeW.h"
#include "ListW.h"

// Constructors
ListW::ListW()
{
    Init();
}

// Copy constructor
ListW::ListW(ListW &data)
{
    Init(data);
}

// Copy constructor
ListW::ListW(ListW *data)
{
    Init(data);
}

// Init functions corresponding to each constructor
void ListW::Init()
{
    head = NULL;
    tail = NULL;
    size = 0;
}

// Deep copy 
void ListW::Init(ListW &data)
{
    head = new ListNodeW(data.head); // copies the whole list
    tail = head;
    if (tail != NULL)
        while (tail->next != NULL)
            tail = tail->next;
    size = data.size;
}

// Deep copy
void ListW::Init(ListW *data)
{
    head = new ListNodeW(data->head); // copies the whole list
    tail = head;
    if (tail != NULL)
        while (tail->next != NULL)
            tail = tail->next;
    size = data->size;
}

// Desctructor
ListW::~ListW()
{
    Empty();
}

// Delete everything in the list
void ListW::Empty()
{
    if (head != NULL)
        DeleteList(head);
    head = NULL;
    tail = NULL;
    size = 0;
}

// Returns the value of the first node
int ListW::Front()
{
    if (head)
        return head->value;
    else {
        cerr << "Accessing node of an empty list" << endl;
        return 0;
    }
}

// Returns the value of the last node
int ListW::Back()
{
    if (tail)
        return tail->value;
    else {
        cerr << "Accessing node of an empty list" << endl;
        return 0;
    }
}

// ListW manipulation
void ListW::InsertFront(int val, float cost)
{
    ListNodeW* node;

    node = new ListNodeW(val, cost);
    if (head == NULL)
        head = tail = node;
    else {
        node->next = head;
        head->prev = node;
        head = node;
    }
    size++;
}

void ListW::InsertBack(int val, float cost)
{
    ListNodeW* node;

    node = new ListNodeW(val, cost);
    if (tail == NULL)
        head = tail = node;
    else {
        tail->next = node;
        node->prev = tail;
        tail = node;
    }
    size++;
}

// Remove the first node in the list
bool ListW::RemoveFront()
{
    ListNodeW* node;

    if (head != NULL) {
        node = head;
        head = head->next;
        head->prev = NULL;
        size--;
        if (size == 0)
            tail = NULL;
        delete node;
        return true;
    }
    else
        return false;
}

// Remove the back node in the list
bool ListW::RemoveBack()
{
    ListNodeW* node;

    if (tail != NULL) {
        node = tail;
        tail = tail->prev;
        tail->next = NULL;
        size--;
        if (size == 0)
            head = NULL;
        delete node;
        return true;
    }
    else
        return false;
}

// concatenate the list data to the target object
void ListW::Concatenate(ListW& data)
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

// Checks if the list is empty
bool ListW::IsEmpty()
{
    return (size == 0);
}


// Converts the list to Boolean. It returns true if the list is not
// empty, and false if it is.
ListW::operator bool()
{
    return (size == 0);
}

// Checks if the list contains the value
bool ListW::Contains(int val)
{
    return head->Search(val) != NULL;
}

// Searches for the value in the list and returns a pointer 
// to the node containing it
ListNodeW* ListW::Search(int val)
{
    return head->Search(val);
}

// Output to the console
void ListW::Print()
{
    cout << "The ListW has " << size << "nodes:" << endl;
    if (head != NULL)
        head->Print();
}

// Output to a file
void ListW::FPrint(FILE* aFile)
{
    fprintf(aFile, "The ListW has %d nodes:\n", size);
    if (head != NULL)
        head->FPrint(aFile);
}

// Output to a file
void ListW::FPrint(ofstream &aFile)
{
    aFile << "The ListW has " << size << " nodes :" << endl;
    if (head != NULL)
        head->FPrint(aFile);
}

// cout output operator
ostream& operator<<(ostream& out, ListW& data)
{
    ListNodeW *node = data.head;
    while (node) {
        out << *node;
    }
    out << endl;
    return out;
}