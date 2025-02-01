// ------------------------------------------------------------------------- //
                            // File: src/scell.c
// ------------------------------------------------------------------------- //

#include "../header/scell.h"

// ------------------------------------------------------------------------- //

void init_scell(SCell *scell, int row, int col, double value, SS_EXIT_CODE *exit_code)
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

    scell->dependent_cells = (Cell1D *)malloc(sizeof(Cell1D));

    if (scell->dependent_cells == NULL)
    {
        // printf("Memory allocation failed - Allocating Cell1D Memory\n");
        // exit(1);
        *(exit_code) = MALLOC_CELL1D;
        return;
    }

    SS_EXIT_CODE cptrs_exit_code;
    init_cell_ptrs(scell->dependent_cells, 10, 0,&cptrs_exit_code);

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