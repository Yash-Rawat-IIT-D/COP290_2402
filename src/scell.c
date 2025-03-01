// ------------------------------------------------------------------------- //
// File: src/scell.c
// ------------------------------------------------------------------------- //

#include "../header/scell.h"
#include "scell.h"
#include "scell1d.h"
// #include "stack_scell.h"
// #include "queue_scell.h" // assuming you have this from scell1d.h
#include <stdlib.h>
#include <stdio.h>

// ------------------------------------------------------------------------- //

void init_scell_formula(SCell *scell)
{
    scell->cell_formula->valid_exp_type = VALUE;
    scell->cell_formula->is_constant = TRUE;
    scell->cell_formula->value = 0;

    scell->cell_formula->left_cell = NULL;
    scell->cell_formula->right_cell = NULL;
    scell->cell_formula->cell_range = NULL;
    return;
}

void init_scell(SCell *scell, int row, int col, int value, SS_EXIT_CODE *exit_code)
{
    scell->cell = (Cell *)malloc(sizeof(Cell));
    if (scell->cell == NULL)
    {
        // printf("Memory allocation failed - Allocating Cell Memory\n");
        // exit(1);
        *(exit_code) = MALLOC_CELL;
        return;
    }

    init_cell(scell->cell, row, col, value);

    scell->dependent_scells = (SCell1D *)malloc(sizeof(SCell1D));

    if (scell->dependent_scells == NULL)
    {
        // printf("Memory allocation failed - Allocating Cell1D Memory\n");
        // exit(1);
        *(exit_code) = MALLOC_CELL1D;
        return;
    }

    SS_EXIT_CODE cptrs_exit_code;

    init_scell_ptrs(scell->dependent_scells, 4, 0, &cptrs_exit_code);

    if (cptrs_exit_code != SS_OK)
    {
        *exit_code = cptrs_exit_code;
        return;
    }

    // scell->precedent_scells = (SCell1D *)malloc(sizeof(SCell1D));

    // SS_EXIT_CODE cptrs_exit_code2;
    // init_scell_ptrs(scell->precedent_scells, 10, 0, &cptrs_exit_code2);

    // if (cptrs_exit_code2 != SS_OK)
    // {
    //     *exit_code = cptrs_exit_code2;
    //     return;
    // }

    scell->cell_formula = (Cell_Formula *)malloc(sizeof(Cell_Formula));

    if (scell->cell_formula == NULL)
    {
        // printf("Memory allocation failed - Allocating Cell_Formula Memory\n");
        // exit(1);
        *(exit_code) = MALLOC_CELL_FORMULA;
        return;
    }

    scell->visited = FALSE;
    scell->err_flag = FALSE;
    init_scell_formula(scell);

    *(exit_code) = SS_OK;
    return;
}

// ------------------------------------------------------------------------- //


void swap_scell_ptrs(SCell1D *arr, int index1, int index2)
{
    SCell *temp = arr->scell_ptrs[index1];
    arr->scell_ptrs[index1] = arr->scell_ptrs[index2];
    arr->scell_ptrs[index2] = temp;
    return;
}


SIM_BOOL check_for_cycle(SCell *start, SCell *target)
{
    // Create a temporary stack for visited nodes.
    Stack_SCell visitedStack;
    if (init_stack(&visitedStack, 10) != Q_OK)
    {
        fprintf(stderr, "Error: Could not initialize visited stack.\n");
        return TRUE; // Fail safe: assume cycle exists.
    }

    // Create a queue for BFS. (Re-use your Queue_SCell from scell1d)
    Q_EXIT_CODE q_exit;
    Queue_SCell *q = create_queue(10, &q_exit);
    if (q == NULL)
    {
        fprintf(stderr, "Error: Could not create BFS queue.\n");
        free_stack(&visitedStack);
        return TRUE;
    }

    // Enqueue the starting cell (e.g., operand cell such as A2)
    enqueue(q, start, &q_exit);
    if (q_exit != Q_OK)
    {
        fprintf(stderr, "Error: Enqueue failed in cycle check.\n");
        free_stack(&visitedStack);
        free_queue(q, &q_exit);
        return TRUE;
    }

    while (q->size > 0)
    {
        SCell *current = dequeue(q, &q_exit);
        if (q_exit != Q_OK)
        {
            fprintf(stderr, "Error: Dequeue failed in cycle check.\n");
            break;
        }
        // If we find the target cell in the dependency chain, then a cycle exists.
        if (current == target)
        {
            // Before returning, unmark visited nodes.
            while (visitedStack.top >= 0)
            {
                SCell *node = pop_stack(&visitedStack);
                node->visited = FALSE;
            }
            free_stack(&visitedStack);
            free_queue(q, &q_exit);
            return TRUE;
        }

        // If current is not yet marked, mark it and record it in our stack.
        if (!current->visited)
        {
            current->visited = TRUE;
            if (push_stack(&visitedStack, current) != Q_OK)
            {
                fprintf(stderr, "Error: Push to visited stack failed.\n");
                // Clean up before returning.
                while (visitedStack.top >= 0)
                {
                    SCell *node = pop_stack(&visitedStack);
                    node->visited = FALSE;
                }
                free_stack(&visitedStack);
                free_queue(q, &q_exit);
                return TRUE;
            }
        }
        // Enqueue all dependent cells (i.e. cells that depend on current)
        for (int i = 0; i < current->dependent_scells->size; i++)
        {
            SCell *dep = current->dependent_scells->scell_ptrs[i];
            if (!dep->visited)
            {
                enqueue(q, dep, &q_exit);
                if (q_exit != Q_OK)
                {
                    fprintf(stderr, "Error: Enqueue failed during dependency traversal.\n");
                    // Clean up before returning.
                    while (visitedStack.top >= 0)
                    {
                        SCell *node = pop_stack(&visitedStack);
                        node->visited = FALSE;
                    }
                    free_stack(&visitedStack);
                    free_queue(q, &q_exit);
                    return TRUE;
                }
            }
        }
    }

    // No cycle detected. Unmark all visited nodes.
    while (visitedStack.top >= 0)
    {
        SCell *node = pop_stack(&visitedStack);
        node->visited = FALSE;
    }
    free_stack(&visitedStack);
    free_queue(q, &q_exit);
    return FALSE;
}