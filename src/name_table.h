#ifndef NAME_TABLE_H
#define NAME_TABLE_H

// Returns the sequence number corresponding to the name
int SequenceNumber(char *name);

// Returns the name corresponding to the sequence number
char *VertexName(int vertex_nr);

// Writes the name without allocating memory space for it
void WriteName(int sequence_nr);

#endif
