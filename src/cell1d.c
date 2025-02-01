// ------------------------------------------------------------------------- //

// Cell1D Struct Definition and Functions

#include "../header/cell1d.h"

// ------------------------------------------------------------------------- //

void init_cell_ptrs(Cell1D *arr, int init_capacity, int init_size, SS_EXIT_CODE *exit_code)
{
    arr->size = init_size;
    arr->capacity = init_capacity; 

    arr->cell_ptrs = (Cell **)malloc(arr->capacity * sizeof(Cell *));

    if (arr->cell_ptrs == NULL)
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

void resize_cell_ptrs(Cell1D *arr, int new_capacity)
{
    arr->cell_ptrs = (Cell **)realloc(arr->cell_ptrs, arr->capacity * sizeof(Cell *));

    if (arr->cell_ptrs == NULL)
    {
        printf("Memory reallocation failed - Reallocating Node1D Memory\n");
        exit(1);
    }

    arr->capacity = new_capacity;
    
}

// ------------------------------------------------------------------------- //

void push_back_cell_ptrs(Cell1D *arr, Cell *cell_ptr)
{
    if(cell_ptr == NULL)
    {
        printf("Invalid : Cell Pointer is NULL\n");
        return;
    }

    if (arr->size == arr->capacity)
    {
        resize_cell_ptrs(arr, arr->capacity * 2); // Resize if the array is full
    }

    arr->cell_ptrs[arr->size] = cell_ptr;
    arr->size++;
}

// ------------------------------------------------------------------------- //

void pop_back_cell_ptrs(Cell1D *arr)
{
    if (arr->size > 0)
    {
        arr->size--;
        if (arr->size > 0 && arr->size <= arr->capacity / 2 && arr->capacity > LO_RESIZE_THRESHOLD )
        {
            // Resize if the array is less than half full, but not if the capacity is less than the threshold
            resize_cell_ptrs(arr, (arr->capacity) / 2); 
        }
    }
    else
    {
        printf("Array is empty, cannot pop\n");
    }
}

// ------------------------------------------------------------------------- //

Cell * at_cell_ptrs(Cell1D *arr, int index)
{
    if (index < 0 || index >= arr->size)
    {
        printf("Index out of bounds\n");
        return NULL;
    }

    return arr->cell_ptrs[index];
}

// ------------------------------------------------------------------------- //

void free_cell_ptrs(Cell1D *arr)
{
    // Freeing the memory allocated to the array, if it is not NULL
    if(arr->cell_ptrs != NULL)
    {
        free(arr->cell_ptrs);
        arr->cell_ptrs = NULL;
    }

    // Reseting the size and capacity of the array
    arr->size = 0;
    arr->capacity = 0;  
}

// ------------------------------------------------------------------------- //