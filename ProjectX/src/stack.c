#include "stack.h"
#include "constants.h"  // if you need any constants

char init_stack(Stack_SCell *stack, int capacity) {
    if (capacity <= 0) {
        return '1';
    }
    stack->items = (SCell **)malloc(capacity * sizeof(SCell *));
    if (stack->items == NULL) {
        return '1';
    }
    stack->capacity = capacity;
    stack->top = -1;
    return '0';
}

char push_stack(Stack_SCell *stack, SCell *item) {
    // Resize if the stack is full
    if (stack->top >= stack->capacity - 1) {
        int new_capacity = stack->capacity * 2;
        SCell **new_items = (SCell **)realloc(stack->items, new_capacity * sizeof(SCell *));
        if (new_items == NULL) {
            return '1';
        }
        stack->items = new_items;
        stack->capacity = new_capacity;
    }
    stack->items[++(stack->top)] = item;
    return '0';
}

SCell *pop_stack(Stack_SCell *stack) {
    if (stack->top < 0) {
        return NULL;
    }
    SCell *item = stack->items[stack->top];
    stack->top--;
    return item;
}

void free_stack(Stack_SCell *stack) {
    if (stack->items != NULL) {
        free(stack->items);
        stack->items = NULL;
    }
    stack->top = -1;
    stack->capacity = 0;
}
