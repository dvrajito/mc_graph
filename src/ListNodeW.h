/*************************************************************************

   Project: MC-Graph, a C++ implementation of genetic algorithms
			for graph drawing and visualization
   License: Creative Commons, Attribution
   Author:  Dana Vrajitoru
   File:    ListNodeW.h
   Updated: October 2022

   ListNodeW class definition, a doubly-linked list node

  ************************************************************************/

#ifndef ListNodeW_Weight_h
#define ListNodeW_Weight_h

#include <ostream>
using namespace std;

class ListNodeW {
public:
    int value;
    float weight;
    ListNodeW *next, *prev;

    // Constructors
    ListNodeW(int val = 0);
    ListNodeW(int val, float w);

    // Deep copy constructor
    ListNodeW(ListNodeW *data);

    // Destructor
    ~ListNodeW();

    // Init corresponding to each constructor
    void Init(int val = 0);
    void Init(int val, float w);

    // Initialize as a deep copy of another node
    void Init(ListNodeW *data);

    // List operations

    // Add the parameter node as head of the list that *this belongs to.
    void AddHead(ListNodeW *node);

    // Add the parameter node as tail of the list that *this belongs to.
    void AddTail(ListNodeW *node);

    // Remove the head of the list that *this belongs to and return it.
    ListNodeW *RemoveHead();

    // Remove the tail of the list that *this belongs to and return it.
    ListNodeW *RemoveTail();

    // returns a pointer to the last node in the list that *this is in.
    ListNodeW *LastNode();

    // Concatenate a whole list at the end of the list starting with the
    // target object.
    void Concatenate(ListNodeW *link);

    // Find the node containing a value if it's there
    ListNodeW* Search(int val);

    // Output the whole list to cout
    void Print();

    // Output the whole list to cout
    void FPrint(FILE *aFile);

    // Output to a file
    void FPrint(ofstream& aFile);

    // Deletes a list starting from the node
    friend void DeleteList(ListNodeW*& head);

    // cout output operator
    friend ostream &operator<<(ostream &out, ListNodeW &data);
};

#endif
