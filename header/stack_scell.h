#ifndef STACK_SCELL_H
#define STACK_SCELL_H

#include "scell.h"
#include "constants.h"

typedef struct Stack_SCell {
    SCell **items;
    int top;
    int capacity;
} Stack_SCell;

// Function prototypes for the stack:
int init_stack(Stack_SCell *stack, int capacity);
int push_stack(Stack_SCell *stack, SCell *item);
SCell *pop_stack(Stack_SCell *stack);
void free_stack(Stack_SCell *stack);

#endif // STACK_SCELL_H
