// ------------------------------------------------------------------------- //

// Node Struct Definition and Functions

#include "../header/cell.h"
#include "../header/cell1d.h"
#include "../header/node.h"

//   ------------------------------------------------------------------------- //

// Initializing the Node
void init_node(Node *node, int row, int col, double value)
{
    // Initialize the values of the cell
    init_cell(&(node->cell), row, col, value);

    // Initialize the 1D array of cells
    init_cell_ptrs(&(node->cell1D), DEFAULT_1D_CAPACITY, DEFAULT_1D_SIZE);
}

// ------------------------------------------------------------------------- //

void add_dependent_cell(Node *node, Cell *cell)
{
    // Add the cell to the 1D array of cells
    push_back_cell_ptrs(&(node->cell1D), cell);
}

// ------------------------------------------------------------------------- //

// For freeing the memory of the 1D array of cells
void free_node_dependents(Node *node)
{
    // Free the 1D array of cells
    free_cell_ptrs(&(node->cell1D));
}

// ------------------------------------------------------------------------- //

// Debugging by printing the Node

void print_node(Node *node)
{
    printf("Node Cell -> Row: %d, Col: %d, Value: %.4f\n",
            node->cell.row, node->cell.col, node->cell.value);
    printf("Node's Cell1D Array Size: %d, Capacity: %d\n", 
            node->cell1D.size, node->cell1D.capacity);

    for(int i = 0; i < node->cell1D.size; i++)
    {
        Cell * cptr = (Cell *)at_cell_ptrs(&(node->cell1D),i);
        printf("Cell : %d || x = %d || y = %d || Value = %.4f\n",i+1,get_cell_row(cptr),get_cell_col(cptr),get_cell_value(cptr));
    }
    
}

// ------------------------------------------------------------------------- //
