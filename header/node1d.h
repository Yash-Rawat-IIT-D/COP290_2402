// NODE1D_H // Header Begins

// ------------------------------------------------------------------------- // 

#ifndef NODE1D_H
#define NODE1D_H
#include "node.h"

// ------------------------------------------------------------------------- //

// Node1D Struct
typedef struct Node1D
{
    Node *data;
    int size;
    int capacity;
} Node1D;

// ------------------------------------------------------------------------- //

// Function Prototypes for Node1D

void init_node_arr(Node1D *arr, int init_capacity, int init_size);
void resize_node_arr(Node1D *arr, int new_capacity);
void push_back_node_arr(Node1D *arr, Node *value);
void pop_back_node_arr(Node1D *arr);
Node at_index(Node1D *arr, int index);
void free_node_arr(Node1D *arr);

// ------------------------------------------------------------------------- //

#endif 
// NODE1D_H // Header Ends