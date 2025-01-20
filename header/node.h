// NODE_H // Start of Header File
#ifndef NODE_H
#define NODE_H

// ------------------------------------------------------------------------- //
#include <stdio.h>
#include <stdlib.h>
#include "cell.h"
#include "cell1d.h"
#define DEFAULT_1D_CAPACITY 10
#define DEFAULT_1D_SIZE 0
#define DEFAULT_CELL_VALUE 0

// ------------------------------------------------------------------------- //

// Node Struct Definition

typedef struct Node
{
    Cell cell;
    Cell1D cell1D;

} Node;

// ------------------------------------------------------------------------- //

// Function Prototypes

void init_node(Node *node, int row, int col, double value);
void add_dependent_cell(Node *node, Cell *cell);
void free_node_dependents(Node *node);
void print_node(Node *node);

// ------------------------------------------------------------------------- //

#endif 
// NODE_H // End of Header File
