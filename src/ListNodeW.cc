/*
  Project GAD
  Author: Dana Vrajitoru
  ListNode class methods
*/

#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <iostream>
using namespace std;
#include "ListNode.h"

// Constructors
ListNode::ListNode(int val, float cost)
{
  Init(val, cost);
}

ListNode::ListNode(ListNode *data)
{
  Init(data);
}

// Destructor
ListNode::~ListNode()
{
  if (next != NULL)
    delete next;
}

// Init corresponding to each constructor
void ListNode::Init(int val, float cost)
{
  next=NULL;
  pred=NULL;
  elem=val;
  weight = cost;
}

void ListNode::Init(ListNode *data)
{
  pred=data->pred;
  if (data->next != NULL)
    next = new ListNode(data->next);
  else
    next=NULL;
  elem=data->elem;
  weight=data->weight;
}

// List operations
void ListNode::AddHead(ListNode *node)
{
  if (pred == NULL) {
    pred=node;
    if (node != NULL)
      node->next = this;
  }
  else
    pred->AddHead(node);
}

void ListNode::AddTail(ListNode *node)
{
  if (next == NULL) {
    next=node;
    if (node != NULL)
      node->pred = this;
  }
  else
    next->AddTail(node);
}

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


// Output
void ListNode::Print()
{
  cout << elem << ' ';
  if (next == NULL)
    cout << endl;
  else
    next->Print();
}

void ListNode::FPrint(FILE *aFile)
{
  fprintf(aFile, "%d ", elem);
  if (next == NULL)
    fprintf(aFile, "\n");
  else
    next->FPrint(aFile);
}










