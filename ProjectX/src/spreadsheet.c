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

// Function for Cell_Formula

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