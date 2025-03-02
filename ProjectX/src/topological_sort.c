#include "topological_sort.h"
#include "dfs.h"
#include "stack.h"
#include "constants.h"  // for INIT_SCELL_SIZE
#include <stdio.h>
#include <stdlib.h>

// Placeholder for cell update logic.
// You should replace this with your actual formula evaluation and cell update code.
void update_cell_value(Spread_Sheet *ss, SCell *node) {
    // For example: node->value = evaluate_formula(node->cell_formula, ss);
    // Here we'll just print that we're updating the cell.
    // (Remove or replace this with your actual update logic.)
    printf("Updating cell at (row=%d, col=%d) with current value %d\n",
           node->cell_formula ? node->cell_formula->fvcons.value : 0,
           0,  // replace with actual column if available
           node->value);
}

void topological_sort_and_update(SCell *start, Spread_Sheet *ss) {
    // Create and initialize a stack for DFS topological sort.
    Stack_SCell stack;
    if (init_stack(&stack, INIT_SCELL_SIZE) != '0') {
        printf("Error: Failed to initialize topological sort stack.\n");
        return;
    }
    
    // Perform DFS starting at the given node; dfs_topological will push nodes
    // onto the stack in postorder.
    dfs_topological(start, &stack);
    
    // Process the nodes in topologically sorted order.
    // As we pop each node, unmark the visited portion of its flag.
    while (stack.top >= 0) {
        SCell *node = pop_stack(&stack);
        
        // Unmark the visited bit while preserving the error flag:
        if (node->visited_err_flag == '2') {
            node->visited_err_flag = '0';
        } else if (node->visited_err_flag == '3') {
            node->visited_err_flag = '1';
        }
        
        // Update the cell value (or trigger recalculation) for this node.
        update_cell_value(ss, node);
    }
    
    // Optionally, free any resources associated with the stack.
    // (Our stack implementation frees its internal array with free_stack, if needed.)
    free_stack(&stack);
}
