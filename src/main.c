#include <stdio.h>
#include <stdlib.h>

#include "../header/cell.h"
#include "../header/spreadsheet.h"
#include "../header/parser_ss.h"

// ------------------------------------------------------------------------- //

int main(int argc, char *argv[])
{
    // Later we will take the input from the user

    int rows = 50;
    int cols = 50;
    SS_EXIT_CODE exit_code_ss_init;

    // Setting the const values of the struct
    // Expected First Step of Spread Sheet Creation

    if(rows <= 0 || cols <= 0 || rows > MAX_SS_ROWS || cols > MAX_SS_COLS)
    {
        if(rows <= 0)
        {
            printf("Error in Rows - Number of Rows must be positive\n");
        }
        else if(cols <= 0)
        {
            printf("Error in Columns - Number of Columns must be positive\n");
        }
        else if(rows > MAX_SS_ROWS)
        {
            printf("Error in Rows - Number of Rows must be less than %d\n",MAX_SS_ROWS);
        }
        else if(cols > MAX_SS_COLS)
        {
            printf("Error in Columns - Number of Columns must be less than %d\n",MAX_SS_COLS);
        }

        return 1;
    }


    Spread_Sheet ss_my = {.SS_ROWS = rows, .SS_COLS = cols};
    init_spread_sheet(&ss_my,&exit_code_ss_init);

    if(exit_code_ss_init != SS_OK)
    {
        printf("Error in Spread Sheet Initialization\n");
        return 1;
    }

    // printf("Rows: %d, Cols: %d\n", ss_my.SS_ROWS ,ss_my.SS_COLS);
    terminal_control_unit(&ss_my);

    return 0;
}

// ------------------------------------------------------------------------- //