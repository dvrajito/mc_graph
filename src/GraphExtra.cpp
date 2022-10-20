/******************************************************************

  Dana Vrajitoru
  Project MC-Graph.
  Last updated: May 17, 2004.

  GraphExtra.cpp
  Helper functions for the Graph class.

*******************************************************************/

#include "GraphExtra.h"
#include <cstring>
#include "name_table.h"
#include <iostream>
using namespace std;

// Convert a couple of names like AB or AaBb into vertex numbers.
void ArcConvert(const char *input, int &j, int &k)
{
  if (strlen(input) == 2) {
	j = Convert1Char(input[0]);
	k = Convert1Char(input[1]);
  }
  else {
	j = Convert2Char(input[0], input[1]);
	k = Convert2Char(input[2], input[3]);
  }
}

// Converts a name given as one uppercase letter into a vertex number.
int Convert1Char(char c)
{
  return (int)(c - 'A');
}

// Converts a name given as 2 characters into a vertex number. The
// convention is that the first character is an uppercase and the
// second one is a lowercase letter. This covers graphs up to 675
// vertices.
int Convert2Char(char c1, char c2)
{
  return (int)((c1-'A')*26 + c2 - 'a');
}

// Some helping function for printing the graph.
void PreliminaryProcess(int v)
{
  Write_name(v);
  cout << ' ';
}
