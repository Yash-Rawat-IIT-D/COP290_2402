// SCELL1D_H // Start of Header File

#ifndef CELL1D_H
#define CELL1D_H

#include "constants.h"  
#include "cell.h"
#include "scell.h"

// ------------------------------------------------------------------------- //

typedef struct SCell SCell;

typedef struct SCell1D
{
    SCell **scell_ptrs;
    int size;
    int capacity;
    
} SCell1D;

typedef struct Queue_SCell
{
    int front;
    int rear;
    int capacity;
    int size; 
    struct SCell** queue;

} Queue_SCell;

// ------------------------------------------------------------------------- //

// Function Prototypes of Cell1D Struct
void init_scell_ptrs(SCell1D *arr, int init_capacity, int init_size,SS_EXIT_CODE *exit_code);
void resize_scell_ptrs(SCell1D *arr, int new_capacity);
void push_back_scell_ptrs(SCell1D *arr, SCell *scell_ptr);
void pop_back_scell_ptrs(SCell1D *arr);
SCell * at_scell_ptrs(SCell1D *arr, int index);
void free_scell_ptrs(SCell1D *arr);

// Function Prototypes of Queue_SCell_Ptr

Queue_SCell * create_queue(int intial_capacity, Q_EXIT_CODE *exit_code);
void enqueue(Queue_SCell *q, SCell *scell_ptr, Q_EXIT_CODE *exit_code);
SCell * dequeue(Queue_SCell *q, Q_EXIT_CODE *exit_code);
SCell * front(Queue_SCell *q, Q_EXIT_CODE *exit_code);
SCell * rear(Queue_SCell *q, Q_EXIT_CODE *exit_code);
void free_queue(Queue_SCell *q, Q_EXIT_CODE *exit_code);





// ------------------------------------------------------------------------- //

#endif 
// SCELL1D_H // End of Header File