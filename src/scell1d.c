// ------------------------------------------------------------------------- //

// Cell1D Struct Definition and Functions

#include "../header/scell1d.h"

// ------------------------------------------------------------------------- //

void init_scell_ptrs(SCell1D *arr, int init_capacity, int init_size, SS_EXIT_CODE *exit_code)
{
    arr->size = init_size;
    arr->capacity = init_capacity;

    arr->scell_ptrs = (SCell **)malloc(arr->capacity * sizeof(SCell *));

    if (arr->scell_ptrs == NULL)
    {
        // printf("Malloc Failed during Cell1D Initialization\n");
        // exit(1);
        *(exit_code) = MALLOC_CELL1D_CELL_PTR;
        return;
    }

    *(exit_code) = SS_OK;
    return;
}

// ------------------------------------------------------------------------- //

void resize_scell_ptrs(SCell1D *arr, int new_capacity)
{
    
    SCell **new_scell_ptrs = (SCell **)realloc(arr->scell_ptrs, new_capacity * sizeof(SCell *));
    

    if (new_scell_ptrs == NULL)
    {
        printf("Memory reallocation failed - Reallocating Node1D Memory\n");
        exit(1);
    }
    arr->scell_ptrs = new_scell_ptrs;
    arr->capacity = new_capacity;
}

// ------------------------------------------------------------------------- //

void push_back_scell_ptrs(SCell1D *arr, SCell *scell_ptr)
{
    if (scell_ptr == NULL)
    {
        printf("Invalid : Cell Pointer is NULL\n");
        return;
    }

    if (arr->size == arr->capacity)
    {
        resize_scell_ptrs(arr, arr->capacity * 2); // Resize if the array is full
    }

    arr->scell_ptrs[arr->size] = scell_ptr;
    arr->size++;
}

// ------------------------------------------------------------------------- //

void pop_back_scell_ptrs(SCell1D *arr)
{
    if (arr->size > 0)
    {
        arr->size--;
        if (arr->size > 0 && arr->size <= arr->capacity / 4 && arr->capacity > LO_RESIZE_THRESHOLD)
        {
            // Resize if the array is less than half full, but not if the capacity is less than the threshold
            resize_scell_ptrs(arr, (arr->capacity) / 2);
        }
    }
    else
    {
        fprintf(stderr,"Array is empty, cannot pop\n");
    }


}

// ------------------------------------------------------------------------- //

SCell *at_scell_ptrs(SCell1D *arr, int index)
{
    if (index < 0 || index >= arr->size)
    {
        printf("Index out of bounds\n");
        return NULL;
    }

    return arr->scell_ptrs[index];
}

// ------------------------------------------------------------------------- //

void free_scell_ptrs(SCell1D *arr)
{
    // Freeing the memory allocated to the array, if it is not NULL
    if (arr->scell_ptrs != NULL)
    {
        free(arr->scell_ptrs);
        arr->scell_ptrs = NULL;
    }

    // Reseting the size and capacity of the array
    arr->size = 0;
    arr->capacity = 0;
}

// ------------------------------------------------------------------------- //

Queue_SCell *create_queue(int intial_capacity, Q_EXIT_CODE *exit_code)
{
    Queue_SCell *q = (Queue_SCell *)malloc(sizeof(Queue_SCell));

    if (q == NULL)
    {
        // printf("Memory allocation failed - Allocating Queue Memory\n");
        // exit(1);
        *(exit_code) = MALLOC_QUEUE;
        return NULL;
    }

    q->size = 0;
    q->front = 0;
    q->rear = -1;
    q->capacity = intial_capacity;

    q->queue = (SCell **)malloc(q->capacity * sizeof(SCell *));

    if (q->queue == NULL)
    {
        // printf("Memory allocation failed - Allocating Queue Memory\n");
        // exit(1);
        *(exit_code) = MALLOC_QUEUE;
        return NULL;
    }

    *(exit_code) = Q_OK;
    return q;
}

void enqueue(Queue_SCell *q, SCell *scell_ptr, Q_EXIT_CODE *exit_code)
{
    if (q->size == q->capacity)
    {
        // printf("Queue is full, cannot enqueue\n");
        // *(exit_code) = FULL_QUEUE;
        resize_q(q, q->capacity * 2, exit_code);
        return;
    }

    q->rear = (q->rear + 1) % q->capacity;
    q->queue[q->rear] = scell_ptr;
    q->size++;

    *(exit_code) = Q_OK;
    return;
}

SCell *dequeue(Queue_SCell *q, Q_EXIT_CODE *exit_code)
{
    if (q->size == 0)
    {
        printf("Queue is empty, cannot dequeue\n");
        *(exit_code) = EMPTY_QUEUE;
        return NULL;
    }

    SCell *scell_ptr = q->queue[q->front];
    q->front = (q->front + 1) % q->capacity;
    q->size--;

    *(exit_code) = Q_OK;
    return scell_ptr;
}

SCell *front(Queue_SCell *q, Q_EXIT_CODE *exit_code)
{
    if (q->size == 0)
    {
        // printf("Queue is empty, cannot return front\n");
        *(exit_code) = EMPTY_QUEUE;
        return NULL;
    }

    *(exit_code) = Q_OK;
    return q->queue[q->front];
}

SCell *rear(Queue_SCell *q, Q_EXIT_CODE *exit_code)
{
    if (q->size == 0)
    {
        // printf("Queue is empty, cannot return rear\n");
        *(exit_code) = EMPTY_QUEUE;
        return NULL;
    }

    *(exit_code) = Q_OK;
    return q->queue[q->rear];
}

void resize_q(Queue_SCell *q, int new_capacity, Q_EXIT_CODE *exit_code)
{
    // q->queue = (SCell **)realloc(q->queue, new_capacity * sizeof(SCell *));

    if (q->queue == NULL)
    {
        printf("Memory reallocation failed - Reallocating Queue Memory\n");
        exit(1);
    }

    int qu_old_size = q->size;

    SCell **new_queue = (SCell **)malloc(new_capacity * sizeof(SCell *));

    if (new_queue == NULL)
    {

        // printf("Memory allocation failed - Allocating New Queue Memory\n");
        // exit(1);
        *(exit_code) = MALLOC_QUEUE;
        return;
    }

    for (int i = 0; i < qu_old_size; i++)
    {
        new_queue[i] = q->queue[(q->front + i) % q->capacity];
    }

    free(q->queue);

    q->queue = new_queue;
    q->front = 0;
    q->rear = qu_old_size - 1;
    q->capacity = new_capacity;
    return;
}

void free_queue(Queue_SCell *q, Q_EXIT_CODE *exit_code)
{
    if (q->queue != NULL)
    {
        free(q->queue);
        q->queue = NULL;
    }

    free(q);
    q = NULL;

    *(exit_code) = Q_OK;
    return;
}

// ------------------------------------------------------------------------- //

int init_stack(Stack_SCell *stack, int capacity)
{
    if (capacity <= 0)
    {
        fprintf(stderr, "Error: Invalid stack capacity.\n");
        return MALLOC_QUEUE; // or another error code defined in constants.h
    }
    stack->items = (SCell**)malloc(capacity * sizeof(SCell *));
    if (stack->items == NULL)
    {
        fprintf(stderr, "Error: Memory allocation failed for stack.\n");
        return MALLOC_QUEUE;
    }
    stack->top = -1;
    stack->capacity = capacity;
    return Q_OK;
}

int push_stack(Stack_SCell *stack, SCell *item)
{
    if (stack->top >= stack->capacity - 1)
    {
        // Resize the stack if needed
        int new_capacity = stack->capacity * 2;
        SCell **new_items = (SCell **)realloc(stack->items, new_capacity * sizeof(SCell *));
        if (new_items == NULL)
        {
            fprintf(stderr, "Error: Failed to resize stack.\n");
            return MALLOC_QUEUE;
        }
        stack->items = new_items;
        stack->capacity = new_capacity;
    }
    stack->items[++stack->top] = item;
    return Q_OK;
}

SCell *pop_stack(Stack_SCell *stack)
{
    if (stack->top < 0)
    {
        fprintf(stderr, "Error: Attempted to pop from an empty stack.\n");
        return NULL;
    }
    return stack->items[stack->top--];
}

void free_stack(Stack_SCell *stack)
{
    if (stack->items != NULL)
    {
        free(stack->items);
        stack->items = NULL;
    }
    stack->top = -1;
    stack->capacity = 0;
}

// ------------------------------------------------------------------------- //



// ------------------------------------------------------------------------- //

SIM_BOOL is_node_in_target(SCell *node, SCell *target_node_tl, SCell *target_node_br)
{
    Cell * node_cell = node->cell;
    Cell * tnode_tl_cell = target_node_tl->cell;
    Cell * tnode_br_cell = target_node_br->cell;
    
    if((node_cell->row >= tnode_tl_cell->row) && (node_cell->row <= tnode_br_cell->row))
    {
        if((node_cell->col >= tnode_tl_cell->col) && (node_cell->col <= tnode_br_cell->col))
        {
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }
    else
    {
        return FALSE;
    }
}

void dfs_cycle_check(SCell *node, SCell *target_node_tl, SCell *target_node_br, Stack_SCell *visitedStack, SIM_BOOL *cycle_exists)
{
    if(node->visited == TRUE)
    {
        return;
    }

    node->visited = TRUE;

    printf("Visiting Node: ");
    debug_print_scell(node);

    if(is_node_in_target(node, target_node_tl, target_node_br) == TRUE)
    {
        push_stack(visitedStack, node);
        *cycle_exists = TRUE;
        return;
    }
    else if(node->dependent_scells->size == 0)
    {
        push_stack(visitedStack, node);
        return;
    }


    for(int i = 0; i < node->dependent_scells->size; i++)
    {
        dfs_cycle_check(node->dependent_scells->scell_ptrs[i], target_node_tl, target_node_br, visitedStack, cycle_exists);

        if(*cycle_exists == TRUE)
        {
            push_stack(visitedStack, node);
            return;
        }
    }


    push_stack(visitedStack, node);

    return;
}

void pop_and_unmark(Stack_SCell *visitedStack)
{
    while(visitedStack->top >= 0)
    {
        SCell *node = pop_stack(visitedStack);
        node->visited = FALSE;
        debug_print_scell(node);
    }
    return;
}




// ------------------------------------------------------------------------------------------- //