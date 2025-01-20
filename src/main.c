#include <stdio.h>
#include <stdlib.h>

#include "../header/cell.h"
#include "../header/cell1d.h"
#include "../header/node.h"
#include "../header/spreadsheet.h"


// ------------------------------------------------------------------------- //

int main()
{
    // int rows = 10;
    // int cols = 20;
    
    // // Setting the const values of the struct 
    // // Expected First Step of Spread Sheet Creation

    // Spread_Sheet ss_my = {.SS_ROWS = rows, .SS_COLS = cols}; 
    // init_Spread_Sheet(&ss_my);
    
    // printf("Rows: %d\n", ss_my.SS_ROWS);
    // printf("Cols: %d\n", ss_my.SS_COLS);

    Node node;

    // Initialize a node with a primary cell at (0, 0) with value 1.0
    init_node(&node, 0, 0, 1.0);

    // Add dependent cells
    Cell dep1 = {1, 1, 2.5};
    Cell dep2 = {2, 2, 3.5};
    add_dependent_cell(&node, &dep1);
    add_dependent_cell(&node, &dep2);

    // Print node details
    print_node(&node);

    // Free resources
    free_node_dependents(&node);

    return 0;

}

// ------------------------------------------------------------------------- //