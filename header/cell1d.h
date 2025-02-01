// CELL1D_H // Start of Header File

#ifndef CELL1D_H
#define CELL1D_H

#include "constants.h"  
#include "cell.h"


// ------------------------------------------------------------------------- //

typedef struct Cell1D
{
    Cell **cell_ptrs;
    int size;
    int capacity;
    
} Cell1D;

// ------------------------------------------------------------------------- //

// Function Prototypes of Cell1D Struct
void init_cell_ptrs(Cell1D *arr, int init_capacity, int init_size,SS_EXIT_CODE *exit_code);
void resize_cell_ptrs(Cell1D *arr, int new_capacity);
void push_back_cell_ptrs(Cell1D *arr, Cell *cell_ptr);
void pop_back_cell_ptrs(Cell1D *arr);
Cell * at_cell_ptrs(Cell1D *arr, int index);
void free_cell_ptrs(Cell1D *arr);

// ------------------------------------------------------------------------- //

#endif 
// CELL1D_H // End of Header File