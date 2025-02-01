#include "../header/spreadsheet.h"
// Allocate memory for the Spread_Sheet and its array of Nodes.
// Assumes that SS_ROWS and SS_COLS are already set.

void init_spread_sheet(Spread_Sheet *ss, SS_EXIT_CODE * exit_code)
{
    ss->arr = (SCell **)malloc(ss->SS_ROWS * sizeof(SCell *));

    if (ss->arr == NULL)
    {
        // printf("Memory allocation failed - Allocating Spread_Sheet Memory\n");
        // exit(1); // Need to discuss this implementation with the team
        *exit_code = MALLOC_SCELL_PTR;
        return;
    }

    for (int i = 0; i < ss->SS_ROWS; i++)
    {
        ss->arr[i] = (SCell *)malloc(ss->SS_COLS * sizeof(SCell));

        if (ss->arr[i] == NULL)
        {
           *exit_code = MALLOC_SCELL;
            return;
        }
        else
        {
            SS_EXIT_CODE ex_cell_mem;
            for (int j = 0; j < ss->SS_COLS; j++)
            {
                init_scell(&(ss->arr[i][j]), i, j, 0.0, &ex_cell_mem);
                if(ex_cell_mem != SS_OK)
                {
                    *exit_code = ex_cell_mem;
                    return;
                }
            }
        }
    }
    
    *exit_code = SS_OK;
    return;
}

SCell * get_scell(Spread_Sheet *ss, int row, int col)
{
    if(row < 0 || row >= ss->SS_ROWS)
    {
        return NULL;
    }
    if(col < 0 || col >= ss->SS_COLS)
    {
        return NULL;
    }
    return &(ss->arr[row][col]);
}   

 
