#include "stack_scell.h"
#include <stdlib.h>
#include <stdio.h>

int init_stack(Stack_SCell *stack, int capacity) {
    if (capacity <= 0) {
        fprintf(stderr, "Error: Invalid stack capacity.\n");
        return MALLOC_QUEUE; // or another error code defined in constants.h
    }
    stack->items = (SCell **)malloc(capacity * sizeof(SCell *));
    if (stack->items == NULL) {
        fprintf(stderr, "Error: Memory allocation failed for stack.\n");
        return MALLOC_QUEUE;
    }
    stack->top = -1;
    stack->capacity = capacity;
    return Q_OK;
}

int push_stack(Stack_SCell *stack, SCell *item) {
    if (stack->top >= stack->capacity - 1) {
        // Resize the stack if needed
        int new_capacity = stack->capacity * 2;
        SCell **new_items = (SCell **)realloc(stack->items, new_capacity * sizeof(SCell *));
        if (new_items == NULL) {
            fprintf(stderr, "Error: Failed to resize stack.\n");
            return MALLOC_QUEUE;
        }
        stack->items = new_items;
        stack->capacity = new_capacity;
    }
    stack->items[++stack->top] = item;
    return Q_OK;
}

SCell *pop_stack(Stack_SCell *stack) {
    if (stack->top < 0) {
        fprintf(stderr, "Error: Attempted to pop from an empty stack.\n");
        return NULL;
    }
    return stack->items[stack->top--];
}

void free_stack(Stack_SCell *stack) {
    if (stack->items != NULL) {
        free(stack->items);
        stack->items = NULL;
    }
    stack->top = -1;
    stack->capacity = 0;
}
