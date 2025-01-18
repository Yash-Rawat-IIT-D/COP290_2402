#include <stdio.h>
#include <stdlib.h>

// ------------------------------------------------------------------------- //

typedef struct Cell
{
    int row;
    int col;
    double value;

} Cell;

// ------------------------------------------------------------------------- //

typedef struct Node
{
    Cell *cell; 

} Node;

typedef struct Node1D
{
    Node *data;
    int size;
    int capacity;

} Node1D;

void init_node_arr(Node1D *arr, int init_capacity, int init_size) {
    arr->size = init_size;
    arr->capacity = init_capacity; // Initial capacity

    arr->data = (Node1D *)malloc(arr->capacity * sizeof(Node1D));
    
    if (arr->data == NULL) {
        printf("Memory allocation failed - Allocating Node1D Memory\n");
        exit(1);
    }
}

void resize_node_arr(Node1D *arr, int new_capacity) {
    arr->capacity = new_capacity;
    arr->data = (Node1D *)realloc(arr->data, arr->capacity * sizeof(Node1D));
    
    if (arr->data == NULL) {
        printf("Memory reallocation failed - Reallocating Node1D Memory\n");
        exit(1);
    }
}

void push_back_node_arr(Node1D *arr, Node *value) {
    if (arr->size == arr->capacity) {
        resize_node_arr(arr, arr->capacity * 2);  // Resize if the array is full
    }
    arr->data[arr->size] = *value;
    arr->size++;
}

// ------------------------------------------------------------------------- //


typedef struct Spread_Sheet
{
    Cell ** arr;
    const int SS_ROWS;
    const int SS_COLS;

} Spread_Sheet;

int malloc_Spread_Sheet(Spread_Sheet *ss)
{
    
}

int main()
{
    int rows = 10;
    int cols = 20;
    
    // Setting the const values of the struct 
    // First Step of Spread Sheet Creation

    Spread_Sheet ss_my = {.SS_ROWS = rows, .SS_COLS = cols}; 

    printf("Rows: %d\n", ss_my.SS_ROWS);
    printf("Cols: %d\n", ss_my.SS_COLS);


    return 0;
}
