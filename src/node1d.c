// ------------------------------------------------------------------------- //

#include <stdio.h>
#include <stdlib.h>
#include "../header/node.h"

// ------------------------------------------------------------------------- //

typedef struct Node1D
{
    Node *data;
    int size;
    int capacity;

} Node1D;

// ------------------------------------------------------------------------- //

void init_node_arr(Node1D *arr, int init_capacity, int init_size)
{
    arr->size = init_size;
    arr->capacity = init_capacity; // Initial capacity of the array

    arr->data = (Node *)malloc(arr->capacity * sizeof(Node));

    if (arr->data == NULL)
    {
        printf("Memory allocation failed - Allocating Node1D Memory\n");
        exit(1);
    }
}

// ------------------------------------------------------------------------- //

void resize_node_arr(Node1D *arr, int new_capacity)
{
    arr->capacity = new_capacity;
    arr->data = (Node *)realloc(arr->data, arr->capacity * sizeof(Node));

    if (arr->data == NULL)
    {
        printf("Memory reallocation failed - Reallocating Node1D Memory\n");
        exit(1);
    }
}

// ------------------------------------------------------------------------- //

void push_back_node_arr(Node1D *arr, Node *value)
{
    if (arr->size == arr->capacity)
    {
        resize_node_arr(arr, arr->capacity * 2); // Resize if the array is full
    }
    arr->data[arr->size] = *value;
    arr->size++;
}

// ------------------------------------------------------------------------- //

void pop_back_node_arr(Node1D *arr)
{
    if (arr->size > 0)
    {
        arr->size--;
        if (arr->size > 0 && arr->size <= arr->capacity / 2)
        {
            resize_node_arr(arr, arr->capacity / 2); // Resize if the array is less than half full
        }
    }
    else
    {
        printf("Array is empty, cannot pop\n");
    }
}

// ------------------------------------------------------------------------- //

Node at_index(Node1D *arr, int index)
{
    if (index >= 0 && index < arr->size)
    {
        return arr->data[index];
    }
    else
    {
        printf("Index out of bounds\n");
        exit(1); // Or return some error value - Implement this after discussing with the team
    }
}

// ------------------------------------------------------------------------- //

void free_node_arr(Node1D *arr)
{
    free(arr->data);
    arr->data = NULL;
    arr->size = 0;
    arr->capacity = 0;
}