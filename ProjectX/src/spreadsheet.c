#include "../header/spreadsheet.h"
// ------------------------------------------------------------------------- //

// Function for SCell

char init_scell(SCell *scell)
{
    if (scell == NULL)
    {
        printf("Error: Malloc SCell\n");
        return '1';
    }

    scell->value = 0;
    char cf_exit_code = '0';
    scell->cell_formula = (CELL_FORMULA *)malloc(sizeof(CELL_FORMULA));
    cf_exit_code = init_cell_formula(scell->cell_formula);

    if (cf_exit_code == '1')
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

char init_cell_formula(CELL_FORMULA *cf)
{
    if (cf == NULL)
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

Spread_Sheet *init_spread_sheet(int rows, int cols)
{
    Spread_Sheet *ss = (Spread_Sheet *)malloc(sizeof(Spread_Sheet));
    if (ss == NULL)
    {
        printf("Error: Malloc Spread_Sheet\n");
        return NULL;
    }

    ss->SS_ROWS = rows;
    ss->SS_COLS = cols;

    ss->arr = (SCell *)malloc(rows * cols * sizeof(SCell));
    if (ss->arr == NULL)
    {
        printf("Error: Malloc SCells * Array\n");
        free(ss);
        return NULL;
    }

    char exit_code = '0';
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            exit_code = init_scell(&(ss->arr[i * cols + j]));
            if (exit_code == '1')
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

// Functions for Dependency List of SCell

char add_dependency_to_cell(Spread_Sheet *ss, Pair target, Pair dep)
{
    SCell *scell = &(ss->arr[target.x * ss->SS_COLS + target.y]);

    if (scell->dependent_scells == NULL)
    {
        scell->dependent_scells = (Pair *)malloc(INIT_DEPENDENCY_SIZE * sizeof(Pair));
        
        if (scell->dependent_scells == NULL)
        {
            printf("Error: Malloc for Dependency List\n");
            return '1';
        }

        scell->dependent_scells_capacity = INIT_DEPENDENCY_SIZE;
        scell->dependent_scells_size = 0;
    }

    if (scell->dependent_scells_size >= scell->dependent_scells_capacity)
    {
        int new_capacity = scell->dependent_scells_capacity * 2;
        Pair *new_deps = (Pair *)realloc(scell->dependent_scells, new_capacity * sizeof(Pair));

        if (new_deps == NULL)
        {
            printf("Error: Realloc for Dependency List\n");
            return '1';
        }

        free(scell->dependent_scells);
        
        scell->dependent_scells = new_deps;
        scell->dependent_scells_capacity = new_capacity;
    }

    scell->dependent_scells[scell->dependent_scells_size].x = dep.x;
    scell->dependent_scells[scell->dependent_scells_size].y = dep.y;
    scell->dependent_scells_size++;

    return '0';
}

char remove_dependency_from_cell(Spread_Sheet *ss, Pair target, Pair dep)
{
    SCell *scell = &(ss->arr[target.x * ss->SS_COLS + target.y]);
    if(scell->dependent_scells == NULL)
    {
        printf("Error: Dependency List is NULL\n");
        return '1';
    }

    int i = 0;
    for (i = 0; i < scell->dependent_scells_size; i++)
    {
        if (scell->dependent_scells[i].x == dep.x && scell->dependent_scells[i].y == dep.y)
        {
            break;
        }
    }

    if (i == scell->dependent_scells_size)
    {
        printf("Error: Dependency not found\n");
        return '1';
    }

    scell->dependent_scells[i].x = scell->dependent_scells[scell->dependent_scells_size - 1].x;
    scell->dependent_scells[i].y = scell->dependent_scells[scell->dependent_scells_size - 1].y;
    scell->dependent_scells_size--;
    return '0';
}

char resize_dependency_list(Spread_Sheet *ss, Pair target, int new_capacity)
{
    SCell *scell = &(ss->arr[target.x * ss->SS_COLS + target.y]);

    if (scell->dependent_scells == NULL)
    {
        printf("Error: Dependency List is NULL\n");
        return '1';
    }

    Pair *new_deps = (Pair *)realloc(scell->dependent_scells, new_capacity * sizeof(Pair));

    if (new_deps == NULL)
    {
        printf("Error: Realloc for Dependency List\n");
        return '1';
    }

    scell->dependent_scells = new_deps;
    scell->dependent_scells_capacity = new_capacity;

    return '0';
}

// ------------------------------------------------------------------------- //

// Functions for SCell Stack

char init_stack(Stack_SCell *stack, int capacity)
{
    if (capacity <= 0)
    {
        return '1';
    }

    stack->items = (Pair *)malloc(capacity * sizeof(Pair));

    if (stack->items == NULL)
    {
        return '1';
    }

    stack->capacity = capacity;
    stack->top = -1;
    return '0';
}

char push_stack(Stack_SCell *stack, Pair item)
{
    // Resize if the stack is full
    if (stack->top >= stack->capacity - 1)
    {
        int new_capacity = stack->capacity * 2;
        Pair *new_items = (Pair *)realloc(stack->items, new_capacity * sizeof(Pair));

        if (new_items == NULL)
        {
            return '1';
        }

        free(stack->items);
        stack->items = new_items;
        stack->capacity = new_capacity;
    }

    stack->items[++(stack->top)] = item;
    return '0';
}

Pair pop_stack(Stack_SCell *stack)
{
    if (stack->top < 0)
    {
        Pair p = {-1, -1};
        return p;
    }

    Pair item = stack->items[stack->top];
    stack->top--;
    return item;
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

// Functions for DFS Algorithm and utilities

void dfs_topological(Spread_Sheet *ss, Pair data, Stack_SCell *stack)
{
    // If already visited (flag '2' or '3'), do nothing.
    SCell *node = &(ss->arr[data.x * ss->SS_COLS + data.y]);

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
            dfs_topological(ss, node->dependent_scells[i], stack);
        }

        // Push the node onto the stack (postorder).
        push_stack(stack, data);
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
char dfs_cycle_check(Spread_Sheet *ss, Pair data_node, Pair data_tl, Pair data_br)
{
    // If we have reached the target, cycle detected.

    if ((data_node.x >= data_tl.x) && (data_node.y >= data_tl.y) && (data_node.x <= data_br.x) && (data_node.y <= data_br.y))
    {
        return '1';
    }

    SCell *node = &(ss->arr[data_node.x * ss->SS_COLS + data_node.y]);

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
        if (dfs_cycle_check(ss, node->dependent_scells[i], data_tl, data_br) == '1')
        {
            // Restore original flag before returning.
            if (node->visited_err_flag == '2')
            {
                node->visited_err_flag = '0';
            }
            else if (node->visited_err_flag == '3')
            {
                node->visited_err_flag = '1';
            }
            return '1';
        }
    }

    // Restore the original flag after processing.

    if (node->visited_err_flag == '2')
    {
        node->visited_err_flag = '0';
    }
    else if (node->visited_err_flag == '3')
    {
        node->visited_err_flag = '1';
    }

    return '0';
}

void update_cell_value(Spread_Sheet *ss, Pair node)
{
    // // For example: node->value = evaluate_formula(node->cell_formula, ss);
    // // Here we'll just print that we're updating the cell.
    // // (Remove or replace this with your actual update logic.)
    // printf("Updating cell at (row=%d, col=%d) with current value %d\n",
    //        node->cell_formula ? node->cell_formula->fvcons.value : 0,
    //        0, // replace with actual column if available
    //        node->value);
    SCell *scell = &(ss->arr[node.x * ss->SS_COLS + node.y]);
    if (scell->cell_formula == NULL)
    {
        return;
    }

    switch (scell->cell_formula->valid_exp_type)
    {
    case '0':
        scell->value = scell->cell_formula->fvcons.value;
        break;
    case '1':
        scell->value = ss->arr[scell->cell_formula->fvcell.cell_row * ss->SS_COLS + scell->cell_formula->fvcell.cell_col].value;
        break;
    case '2':
        int vl = scell->cell_formula->farith_cons_cons.left_value;
        int vr = scell->cell_formula->farith_cons_cons.right_value;

        switch (scell->cell_formula->farith_cons_cons.arithmetic_op)
        {
        case '+':
            scell->value = vl + vr;
            break;
        case '-':
            scell->value = vl - vr;
            break;
        case '*':
            scell->value = vl * vr;
            break;
        case '/':
            scell->value = vl / vr;
            break;
        default:
            break;
        }

        break;

    case '3':
        int vl = scell->cell_formula->farith_cons_cell.left_value;
        int vr = ss->arr[scell->cell_formula->farith_cons_cell.right_cell_row * ss->SS_COLS + scell->cell_formula->farith_cons_cell.right_cell_col].value;

        switch (scell->cell_formula->farith_cons_cell.arithmetic_op)
        {
        case '+':
            scell->value = vl + vr;
            break;
        case '-':
            scell->value = vl - vr;
            break;
        case '*':
            scell->value = vl * vr;
            break;
        case '/':
            scell->value = vl / vr;
            break;
        default:
            break;
        }
        break;

    case '4':
        int vl = ss->arr[scell->cell_formula->farith_cell_cons.left_cell_row * ss->SS_COLS + scell->cell_formula->farith_cell_cons.left_cell_col].value;
        int vr = scell->cell_formula->farith_cell_cons.right_value;

        switch (scell->cell_formula->farith_cell_cons.arithmetic_op)
        {
        case '+':
            scell->value = vl + vr;
            break;
        case '-':
            scell->value = vl - vr;
            break;
        case '*':
            scell->value = vl * vr;
            break;
        case '/':
            scell->value = vl / vr;
            break;
        default:
            break;
        }
        break;

    case '5':
        int vl = ss->arr[scell->cell_formula->farith_cell_cell.left_cell_row * ss->SS_COLS + scell->cell_formula->farith_cell_cell.left_cell_col].value;
        int vr = ss->arr[scell->cell_formula->farith_cell_cell.right_cell_row * ss->SS_COLS + scell->cell_formula->farith_cell_cell.right_cell_col].value;

        switch (scell->cell_formula->farith_cell_cell.arithmetic_op)
        {
        case '+':
            scell->value = vl + vr;
            break;
        case '-':
            scell->value = vl - vr;
            break;
        case '*':
            scell->value = vl * vr;
            break;
        case '/':
            scell->value = vl / vr;
            break;
        default:
            break;
        }
        break;

    case '6':
        scell->value = scell->cell_formula->fsleep_cons.sleep_time;
        sleep(scell->value);
        break;

    case '7':
        scell->value = ss->arr[scell->cell_formula->fsleep_cell.sleep_cell_row * ss->SS_COLS + scell->cell_formula->fsleep_cell.sleep_cell_col].value;
        sleep(scell->value);
        break;

    case '8':
        short int start_row = scell->cell_formula->ffunc.start_row;
        short int start_col = scell->cell_formula->ffunc.start_col;
        short int end_row = scell->cell_formula->ffunc.end_row;
        short int end_col = scell->cell_formula->ffunc.end_col;

        int sum, sum_sq, count, min, max;
        double mean, stdev;
        sum = 0;
        sum_sq = 0;
        count = 0;
        mean = 0.0;
        stdev = 0.0;

        for (int r = start_row; r <= end_row; r++)
        {
            for (int c = start_col; c <= end_col; c++)
            {
                int val = ss->arr[r * ss->SS_COLS + c].value;

                sum += val;
                sum_sq += val * val;

                if(count == 0)
                {
                    min = val;
                    max = val;
                }
                else
                {
                    if(val < min)
                    {
                        min = val;
                    }
                    if(val > max)
                    {
                        max = val;
                    }
                }
                count++;
            }
        }

        mean = (double) 1.0 * sum / count;
        stdev = sqrt((double) 1.0* sum_sq / count - 1.0 * mean * mean);

        switch (scell->cell_formula->ffunc.function)
        {
        case 'M':
            scell->value = min;
            break;
        case 'X':
            scell->value = max;
            break;
        case 'A':
            scell->value = mean;
            break;
        case 'S':
            scell->value = sum;
            break;
        case 'D':
            scell->value = stdev;
            break;
        default:
            break;
        }

        break;
    
    default:

        break;
    }
    
    return;
    
}

void topological_sort_and_update(Spread_Sheet *ss, Pair start)
{
    // Create and initialize a stack for DFS topological sort.

    Stack_SCell stack;

    if (init_stack(&stack, INIT_SCELL_SIZE) != '0')
    {
        printf("Error: Failed to initialize topological sort stack.\n");
        return;
    }

    // Perform DFS starting at the given node; dfs_topological will push nodes
    // onto the stack in postorder.

    dfs_topological(ss, start, &stack);

    // Process the nodes in topologically sorted order.
    // As we pop each node, unmark the visited portion of its flag.

    while (stack.top >= 0)
    {
        Pair node_pair = pop_stack(&stack);
        SCell *node = &(ss->arr[node_pair.x * ss->SS_COLS + node_pair.y]);

        // Unmark the visited bit while preserving the error flag:

        if (node->visited_err_flag == '2')
        {
            node->visited_err_flag = '0';
        }
        else if (node->visited_err_flag == '3')
        {
            node->visited_err_flag = '1';
        }

        // Update the cell value (or trigger recalculation) for this node.
        update_cell_value(ss, node_pair);
    }

    // Optionally, free any resources associated with the stack.
    // (Our stack implementation frees its internal array with free_stack, if needed.)
    free_stack(&stack);
    return;
}

// ------------------------------------------------------------------------- //

// Functions for Update Logic

/*
 * remove_old_dependencies:
 * Removes target from the dependency lists of cells referenced in its current (old) formula.
 */

void remove_old_dependencies(Spread_Sheet *ss, Pair target)
{
   SCell * scell = &(ss->arr[target.x * ss->SS_COLS + target.y]);

   if(scell->cell_formula == NULL)
   {
       return;
   }

   switch ( scell->cell_formula->valid_exp_type)
   {

   }
}

/*
 * add_new_dependencies:
 * Adds target to the dependency lists of cells referenced in the new formula.
 */
void add_new_dependencies(SCell *target, CELL_FORMULA *new_formula, Spread_Sheet *ss)
{
    char type = new_formula->valid_exp_type;
    SCell *dep;
    if (type == '1')
    {
        dep = get_scell_by_coordinates(ss, new_formula->fvcell.cell_row,
                                       new_formula->fvcell.cell_col);
        if (dep)
            add_dependency_to_cell(dep, target);
    }
    else if (type == '5')
    {
        dep = get_scell_by_coordinates(ss, new_formula->farith_cell_cell.left_cell_row,
                                       new_formula->farith_cell_cell.left_cell_col);
        if (dep)
            add_dependency_to_cell(dep, target);
        dep = get_scell_by_coordinates(ss, new_formula->farith_cell_cell.right_cell_row,
                                       new_formula->farith_cell_cell.right_cell_col);
        if (dep)
            add_dependency_to_cell(dep, target);
    }
    else if (type == '8')
    {
        for (int r = new_formula->ffunc.start_row; r <= new_formula->ffunc.end_row; r++)
        {
            for (int c = new_formula->ffunc.start_col; c <= new_formula->ffunc.end_col; c++)
            {
                dep = get_scell_by_coordinates(ss, r, c);
                if (dep)
                    add_dependency_to_cell(dep, target);
            }
        }
    }
    // Other types (e.g., constants) have no dependencies.
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
char update_logic_unit(Spread_Sheet *ss, SCell *target, CELL_FORMULA *new_formula)
{
    char exit_code = '0';

    // Check for cycle: for each dependency in the new formula, perform DFS starting at the target.
    if (check_cycle_for_formula(target, new_formula, ss) == '1')
    {
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