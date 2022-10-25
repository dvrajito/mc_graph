/*************************************************************************

   Project: MC-Graph, a C++ implementation of genetic algorithms
			for graph drawing and visualization
   License: Creative Commons, Attribution
   Author:  Dana Vrajitoru
   File:    ListNode.h
   Updated: October 2022

   ListNode class definition, a doubly-linked list node

  ************************************************************************/

#ifndef ListNode_h
#define ListNode_h

#include <ostream>
using namespace std;

class ListNode {
public:
    int value;
    ListNode *next, *prev;

    // Constructors
    ListNode(int val = 0);

    // Deep copy constructor
    ListNode(ListNode *data);

    // Destructor
    ~ListNode();

    // Init corresponding to each constructor
    void Init(int val = 0);

    // Initialize as a deep copy of another node
    void Init(ListNode *data);

    // List operations

    // Add the parameter node as head of the list that *this belongs to.
    void AddHead(ListNode *node);

    // Add the parameter node as tail of the list that *this belongs to.
    void AddTail(ListNode *node);

    // Remove the head of the list that *this belongs to and return it.
    ListNode *RemoveHead();

    // Remove the tail of the list that *this belongs to and return it.
    ListNode *RemoveTail();

    // returns a pointer to the last node in the list that *this is in.
    ListNode *LastNode();

    // Concatenate a whole list at the end of the list starting with the
    // target object.
    void Concatenate(ListNode *link);

    // Output the whole list to cout
    void Print();

    // Output the whole list to cout
    void FPrint(FILE *aFile);

    // cout output operator
    friend ostream &operator<<(ostream &out, ListNode &data);
};

#endif
