#ifndef STACK_H
#define STACK_H

#include "spreadsheet.h"  // for the definition of SCell

typedef struct Stack_SCell {
    SCell **items;
    int top;
    int capacity;
} Stack_SCell;

// Initializes the stack with the given initial capacity.
// Returns '0' on success, '1' on failure.
char init_stack(Stack_SCell *stack, int capacity);

// Pushes an SCell pointer onto the stack.
// Automatically resizes if needed.
// Returns '0' on success, '1' on failure.
char push_stack(Stack_SCell *stack, SCell *item);

// Pops and returns the top SCell pointer from the stack.
// Returns NULL if the stack is empty.
SCell *pop_stack(Stack_SCell *stack);

// Frees the memory allocated for the stack.
void free_stack(Stack_SCell *stack);

#endif // STACK_H
