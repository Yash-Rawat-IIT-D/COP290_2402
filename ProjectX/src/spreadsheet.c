#include "../header/spreadsheet.h"
// ------------------------------------------------------------------------- //

// Function for SCell

char init_scell(SCell * scell)
{
    if(scell == NULL)
    {
        printf("Error: Malloc SCell\n");
        return '1';
    }

    scell->value = 0;
    char cf_exit_code = '0'; 
    scell->cell_formula = (CELL_FORMULA *)malloc(sizeof(CELL_FORMULA));
    cf_exit_code = init_cell_formula(scell->cell_formula);

    if(cf_exit_code == '1')
    {
        printf("Error: Malloc Cell_Formula\n");
        free(scell);
        return '1';
    }


    scell->dependent_scells = NULL;
    scell->dependent_scells_size = 0;
    scell->dependent_scells_capacity = 0;

    scell->visited_err_flag = '0';

    return '0';
}


// ------------------------------------------------------------------------- // 

// Function for SCell_Formula

char init_cell_formula(CELL_FORMULA * cf)
{
    if(cf == NULL)
    {
        printf("Error: Malloc Cell_Formula\n");
        return '1';
    }

    cf->valid_exp_type = '0';
    cf->fvcons.value = 0;

    return '0';
} 

// ------------------------------------------------------------------------- //

// Function for Spreadsheet

Spread_Sheet * init_spread_sheet(int rows, int cols)
{
    Spread_Sheet *ss = (Spread_Sheet *)malloc(sizeof(Spread_Sheet));
    if(ss == NULL)
    {
        printf("Error: Malloc Spread_Sheet\n");
        return NULL;
    }

    ss->SS_ROWS = rows;
    ss->SS_COLS = cols;

    ss->arr = (SCell *)malloc(rows * cols * sizeof(SCell));
    if(ss->arr == NULL)
    {
        printf("Error: Malloc SCells * Array\n");
        free(ss);
        return NULL;
    }

    char exit_code = '0';
    for(int i = 0; i < rows; i++)
    {
        for(int j = 0; j < cols; j++)
        {
            exit_code = init_scell(&(ss->arr[i * cols + j]));
            if(exit_code == '1')
            {
                printf("Error: Malloc SCell\n");
                free(ss->arr);
                free(ss);
                return NULL;
            }
        }
    }

    return ss;
}

// ------------------------------------------------------------------------- //

// Functions for SCell Stack

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

        free(stack->items);
        stack->items = new_items;
        stack->capacity = new_capacity;
    }
    stack->items[++(stack->top)] = item;
    return '0';
}

SCell * pop_stack(Stack_SCell *stack) {
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

// ------------------------------------------------------------------------- //

// Functions for DFS Algorithm

void dfs_topological(SCell *node, Stack_SCell *stack)
{
    // If already visited (flag '2' or '3'), do nothing.
    if (node->visited_err_flag == '2' || node->visited_err_flag == '3')
    {
        return;
    }
    else
    {
        // Mark as visited, preserving the error bit.
        if (node->visited_err_flag == '0')
        {
            node->visited_err_flag = '2';
        }
        else if (node->visited_err_flag == '1')
        {
            node->visited_err_flag = '3';
        }

        // Process each dependency.
        for (int i = 0; i < node->dependent_scells_size; i++)
        {
            dfs_topological(node->dependent_scells[i], stack);
        }

        // Push the node onto the stack (postorder).
        push_stack(stack, node);
    }
}

/*
 * DFS for Cycle Detection:
 * This function checks whether 'target' is reachable from 'node'.
 * If node equals target, return '1' indicating a cycle.
 * Otherwise, if the node is not yet visited, mark it (adding '2' while preserving error state)
 * and recursively search its dependencies.
 * After processing, restore the node’s original flag.
 */
char dfs_cycle_check(SCell *node, SCell *target)
{
    // If we have reached the target, cycle detected.
    if (node == target)
    {
        return '1';
    }

    // If already visited in this DFS (flag '2' or '3'), skip processing.
    if (node->visited_err_flag == '2' || node->visited_err_flag == '3')
    {
        return '0';
    }

    // Save the original flag ('0' or '1').
    char original_flag = node->visited_err_flag;
    // Mark the node as visited without changing error status.
    if (original_flag == '0')
    {
        node->visited_err_flag = '2';
    }
    else if (original_flag == '1')
    {
        node->visited_err_flag = '3';
    }

    // Recurse on each dependent cell.
    for (int i = 0; i < node->dependent_scells_size; i++)
    {
        if (dfs_cycle_check(node->dependent_scells[i], target) == '1')
        {
            // Restore original flag before returning.
            if (node->visited_err_flag == '2')
                node->visited_err_flag = '0';
            else if (node->visited_err_flag == '3')
                node->visited_err_flag = '1';
            return '1';
        }
    }

    // Restore the original flag after processing.
    if (node->visited_err_flag == '2')
        node->visited_err_flag = '0';
    else if (node->visited_err_flag == '3')
        node->visited_err_flag = '1';

    return '0';
}

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

// ------------------------------------------------------------------------- //

// Functions for Update Logic

/*
 * remove_old_dependencies:
 * Removes target from the dependency lists of cells referenced in its current (old) formula.
 */
void remove_old_dependencies(Spread_Sheet *ss, SCell *target) {
    if (target->cell_formula == NULL) return;
    char type = target->cell_formula->valid_exp_type;
    SCell *dep;
    if (type == '1') {  // Single cell reference.
        dep = get_scell_by_coordinates(ss, target->cell_formula->fvcell.cell_row,
                                            target->cell_formula->fvcell.cell_col);
        if (dep) {
            remove_dependency_from_cell(dep, target);
        }
    } else if (type == '5') {  // Arithmetic expression involving two cell references.
        dep = get_scell_by_coordinates(ss, target->cell_formula->farith_cell_cell.left_cell_row,
                                            target->cell_formula->farith_cell_cell.left_cell_col);
        if (dep) remove_dependency_from_cell(dep, target);
        dep = get_scell_by_coordinates(ss, target->cell_formula->farith_cell_cell.right_cell_row,
                                            target->cell_formula->farith_cell_cell.right_cell_col);
        if (dep) remove_dependency_from_cell(dep, target);
    } else if (type == '8') {  // Function operating on a range.
        for (int r = target->cell_formula->ffunc.start_row; r <= target->cell_formula->ffunc.end_row; r++) {
            for (int c = target->cell_formula->ffunc.start_col; c <= target->cell_formula->ffunc.end_col; c++) {
                dep = get_scell_by_coordinates(ss, r, c);
                if (dep) remove_dependency_from_cell(dep, target);
            }
        }
    }
    // For types that do not reference other cells, nothing needs to be removed.
}

/*
 * add_new_dependencies:
 * Adds target to the dependency lists of cells referenced in the new formula.
 */
void add_new_dependencies(SCell *target, CELL_FORMULA *new_formula, Spread_Sheet *ss) {
    char type = new_formula->valid_exp_type;
    SCell *dep;
    if (type == '1') {
        dep = get_scell_by_coordinates(ss, new_formula->fvcell.cell_row,
                                            new_formula->fvcell.cell_col);
        if (dep) add_dependency_to_cell(dep, target);
    } else if (type == '5') {
        dep = get_scell_by_coordinates(ss, new_formula->farith_cell_cell.left_cell_row,
                                            new_formula->farith_cell_cell.left_cell_col);
        if (dep) add_dependency_to_cell(dep, target);
        dep = get_scell_by_coordinates(ss, new_formula->farith_cell_cell.right_cell_row,
                                            new_formula->farith_cell_cell.right_cell_col);
        if (dep) add_dependency_to_cell(dep, target);
    } else if (type == '8') {
        for (int r = new_formula->ffunc.start_row; r <= new_formula->ffunc.end_row; r++) {
            for (int c = new_formula->ffunc.start_col; c <= new_formula->ffunc.end_col; c++) {
                dep = get_scell_by_coordinates(ss, r, c);
                if (dep) add_dependency_to_cell(dep, target);
            }
        }
    }
    // Other types (e.g., constants) have no dependencies.
}

/*
 * check_cycle_for_formula:
 * Checks for cycles by, for each dependency in the new formula, performing DFS
 * starting at the target cell and checking if that dependency is already present
 * in the dependency graph.
 *
 * For non-range formulas, we check each dependency (for type '1' or '5').
 * For range formulas (type '8'), we iterate over all cells in the range and
 * check if any cell is encountered during DFS from the target.
 *
 * Returns '1' if a cycle is detected, '0' otherwise.
 */
char check_cycle_for_formula(SCell *target, CELL_FORMULA *formula, Spread_Sheet *ss) {
    char type = formula->valid_exp_type;
    SCell *dep;
    if (type == '1') {  // Single cell dependency.
        dep = get_scell_by_coordinates(ss, formula->fvcell.cell_row, formula->fvcell.cell_col);
        // DFS starting at target, checking for dep.
        if (dep && dfs_cycle_check(target, dep) == '1') {
            return '1';
        }
    } else if (type == '5') {  // Arithmetic expression with two cell dependencies.
        dep = get_scell_by_coordinates(ss, formula->farith_cell_cell.left_cell_row,
                                            formula->farith_cell_cell.left_cell_col);
        if (dep && dfs_cycle_check(target, dep) == '1') {
            return '1';
        }
        dep = get_scell_by_coordinates(ss, formula->farith_cell_cell.right_cell_row,
                                            formula->farith_cell_cell.right_cell_col);
        if (dep && dfs_cycle_check(target, dep) == '1') {
            return '1';
        }
    } else if (type == '8') {  // Range function dependency.
        for (int r = formula->ffunc.start_row; r <= formula->ffunc.end_row; r++) {
            for (int c = formula->ffunc.start_col; c <= formula->ffunc.end_col; c++) {
                dep = get_scell_by_coordinates(ss, r, c);
                // DFS starting at target, checking if any cell in the range is reached.
                if (dep && dfs_cycle_check(target, dep) == '1') {
                    return '1';
                }
            }
        }
    }
    return '0';
}

/*
 * update_logic_unit:
 * Main update logic function.
 * - First, it checks for cycles using DFS (with parameters swapped so that DFS starts at target).
 * - If a cycle is found, it returns '5' (cycle detected) and aborts.
 * - Otherwise, it removes target from the dependency lists of cells referenced in its old formula,
 *   adds target to the dependency lists of cells referenced by the new formula,
 *   updates the target's formula pointer, and then triggers a topological sort–based update.
 */
char update_logic_unit(Spread_Sheet *ss, SCell *target, CELL_FORMULA *new_formula) {
    char exit_code = '0';
    
    // Check for cycle: for each dependency in the new formula, perform DFS starting at the target.
    if (check_cycle_for_formula(target, new_formula, ss) == '1') {
        printf("Cycle Found\n");
        return '5';
    }
    
    // Remove target from dependency lists of cells referenced in its old formula.
    remove_old_dependencies(ss, target);
    
    // Add target to dependency lists of cells referenced in the new formula.
    add_new_dependencies(target, new_formula, ss);
    
    // Update the target's formula pointer.
    target->cell_formula = new_formula;
    
    // Now perform a topological sort starting from the target and update cells in order.
    topological_sort_and_update(target, ss);
    
    return exit_code;
}

// ------------------------------------------------------------------------- //