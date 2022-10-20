#ifndef NAME_TABLE_H
#define NAME_TABLE_H

// Returns the sequence number corresponding to the name
int Sequence_number(char *name);

// Returns the name corresponding to the sequence number
char *Vertex_name(int vertex_nr);

// Writes the name without allocating memory space for it
void Write_name(int sequence_nr);

#endif
