/*
  c215 fall 2001
  Dana Vrajitoru
  A small program that deals with vertex names .
  It supposes that the names are made of just one letter.
*/

#include <iostream>
using namespace std;
#include "name_table.h"
#include <stdlib.h>
#include <ctype.h>

// Returns the sequence number corresponding to the name
int Sequence_number(char *name)
{
  if (name != NULL)
    return toupper(name[0]) - 'A';
  else
    return -1;
}

// Returns the name corresponding to the sequence number
char *Vertex_name(int vertex_nr)
{
  if (vertex_nr < 0)
    return NULL;
  else {
    char *name = new char[2];
    name[0] = 'A' + vertex_nr;
    name[1] = '\0';
    return name;
  }
}

// Writes the name without allocating memory space for it
void Write_name(int sequence_nr)
{
  cout << (char) ('A' + sequence_nr);
}
