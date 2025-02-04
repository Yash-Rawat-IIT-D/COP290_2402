// ------------------------------------------------------------------------- //
                            // File: src/scell.c
// ------------------------------------------------------------------------- //

#include "../header/scell.h"

// ------------------------------------------------------------------------- //

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
    init_scell_ptrs(scell->dependent_scells, 10, 0, &cptrs_exit_code);

    if(cptrs_exit_code != SS_OK)
    {
        *exit_code = cptrs_exit_code;
        return;
    }
    

    scell->cell_formula = (Cell_Formula *)malloc(sizeof(Cell_Formula));

    if (scell->cell_formula == NULL)
    {
        // printf("Memory allocation failed - Allocating Cell_Formula Memory\n");
        // exit(1);
        *(exit_code) = MALLOC_CELL_FORMULA;
        return;
    }

    scell->cell_formula->cell = NULL;
    scell->cell_formula->left_cell = NULL;
    scell->cell_formula->right_cell = NULL;
    scell->cell_formula->cell_range = NULL;
    *(exit_code) = SS_OK;
    return;
}

// ------------------------------------------------------------------------- //

void debug_print_scell(SCell *scell)
{
    printf("Cell - Row: %d, Col: %d, Value: %d\n", get_cell_row(scell->cell), get_cell_col(scell->cell), get_cell_value(scell->cell));
    for (int i = 0; i < scell->dependent_scells->size; i++)
    {
        printf("Dependent Cell %d: Row: %d, Col: %d, Value: %d\n", i, get_cell_row(scell->dependent_scells->scell_ptrs[i]->cell), get_cell_col(scell->dependent_scells->scell_ptrs[i]->cell), get_cell_value(scell->dependent_scells->scell_ptrs[i]->cell));
    }
    return;
}