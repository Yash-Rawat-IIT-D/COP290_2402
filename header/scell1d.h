// SCELL1D_H // Start of Header File

#ifndef CELL1D_H
#define CELL1D_H

#include "constants.h"  
#include "cell.h"
#include "scell.h"
// #include "spreadsheet.h"
#include <stdbool.h>
// ------------------------------------------------------------------------- //

typedef struct SCell SCell;
typedef struct Spread_Sheet Spread_Sheet;
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


typedef struct Stack_SCell {
    SCell **items;
    int top;
    int capacity;
} Stack_SCell;

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

// Function prototypes for the stack:
int init_stack(Stack_SCell *stack, int capacity);
int push_stack(Stack_SCell *stack, SCell *item);
SCell *pop_stack(Stack_SCell *stack);
void free_stack(Stack_SCell *stack);


// Function prototypes for the stack:
SIM_BOOL is_node_in_target(SCell *node, SCell *target_node_tl, SCell *target_node_br);
void dfs_cycle_check(SCell *node, SCell *target_node_tl, SCell *target_node_br, Stack_SCell *visitedStack, SIM_BOOL *cycle_exists);
void pop_and_unmark(Stack_SCell *visitedStack);

// ------------------------------------------------------------------------- //

// Perform a DFS-based topological sort on the dependency subgraph starting at 'target',
// then update (recalculate) the cell values in topologically sorted order.

void dfs_topological(SCell *node, Stack_SCell *stack);
void remove_scell_ptr(SCell1D *arr, SCell *target);


#endif 
// SCELL1D_H // End of Header File