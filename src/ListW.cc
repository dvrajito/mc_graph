/*
  Project GAD
  Author: Dana Vrajitoru
  List class methods
*/

#include <stdio.h>
#include <stdlib.h>
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

List::List(List *data)
{
  Init(data);
}


// Init functions corresponding to each constructor
void List::Init()
{
  head=NULL;
  tail=NULL;
  nodeNr = 0;
}

void List::Init(List *data)
{
  head = new ListNode(data->head);
  tail = head;
  if (tail != NULL)
    while (tail->next != NULL)
      tail = tail->next;
  nodeNr = data->nodeNr;
}

  
// Desctructor
List::~List()
{
  if (head != NULL)
    delete head;
  head=NULL;
  tail = NULL;
  nodeNr = 0;
}


// List manipulation
void List::AddHead(int val, float cost)
{
  ListNode *node;

  node = new ListNode(val, cost);
  if (head == NULL)
    head=tail=node;
  else {
    head->AddHead(node);
    head=node;
  }
  nodeNr++;
}

void List::AddTail(int val, float cost)
{
  ListNode *node;

  node = new ListNode(val, cost);
  if (tail == NULL)
    head=tail=node;
  else {
    tail->AddTail(node);
    tail=node;
  }
  nodeNr++;
}

int List::RemoveHead()
{
  ListNode *node;
  int val;

  if (head != NULL) {
    node = head;
    head=head->RemoveHead();
    val = node->elem;
    if (nodeNr == 1)
      tail = NULL;
    delete node;
    nodeNr--;
    return val;
  }
  else
    return 0;
}

int List::RemoveTail()
{
  ListNode *node;
  int val;

  if (tail != NULL) {
    node = tail;
    tail=tail->RemoveTail();
    val = node->elem;
    if (nodeNr == 1)
      head = NULL;
    delete node;
    nodeNr--;
    return val;
  }
  else
    return 0;
}

// Some useful methods
int List::IsEmpty()
{
  return (nodeNr == 0);
}

int List::Find(int val)
{
  return FindNode(val) != NULL;
}

ListNode *List::FindNode(int val)
{
  ListNode *node=head;
  while ((node != NULL) && (node->elem != val))
	node = node->next;
  return node;
}

// Output
void List::Print()
{
  cout << "The list has " << nodeNr << "nodes:" << endl;
  if (head != NULL)
    head->Print();
}

void List::FPrint(FILE *aFile)
{
  fprintf(aFile, "The list has %d nodes:\n", nodeNr);
  if (head != NULL)
    head->FPrint(aFile);
}

