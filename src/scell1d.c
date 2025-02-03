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
    arr->scell_ptrs = (SCell **)realloc(arr->scell_ptrs, arr->capacity * sizeof(SCell *));

    if (arr->scell_ptrs == NULL)
    {
        printf("Memory reallocation failed - Reallocating Node1D Memory\n");
        exit(1);
    }

    arr->capacity = new_capacity;
    
}

// ------------------------------------------------------------------------- //

void push_back_scell_ptrs(SCell1D *arr, SCell *scell_ptr)
{
    if(scell_ptr == NULL)
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
        if (arr->size > 0 && arr->size <= arr->capacity / 2 && arr->capacity > LO_RESIZE_THRESHOLD )
        {
            // Resize if the array is less than half full, but not if the capacity is less than the threshold
            resize_scell_ptrs(arr, (arr->capacity) / 2); 
        }
    }
    else
    {
        printf("Array is empty, cannot pop\n");
    }
}

// ------------------------------------------------------------------------- //

SCell * at_scell_ptrs(SCell1D *arr, int index)
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
    if(arr->scell_ptrs != NULL)
    {
        free(arr->scell_ptrs);
        arr->scell_ptrs = NULL;
    }

    // Reseting the size and capacity of the array
    arr->size = 0;
    arr->capacity = 0;  
}

// ------------------------------------------------------------------------- //

Queue_SCell * create_queue(int intial_capacity, Q_EXIT_CODE *exit_code)
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
        printf("Queue is full, cannot enqueue\n");
        *(exit_code) = FULL_QUEUE;
        return;
    }

    q->rear = (q->rear + 1) % q->capacity;
    q->queue[q->rear] = scell_ptr;
    q->size++;

    *(exit_code) = Q_OK;
    return;
}

SCell * dequeue(Queue_SCell *q, Q_EXIT_CODE *exit_code)
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

SCell * front(Queue_SCell *q, Q_EXIT_CODE *exit_code)
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

SCell * rear(Queue_SCell *q, Q_EXIT_CODE *exit_code)
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