#include <stdio.h>
#include <stdlib.h>

#include "../header/cell.h"
#include "../header/spreadsheet.h"
#include "../header/parser_ss.h"

// ------------------------------------------------------------------------- //

int main(int argc, char *argv[])
{
    // Later we will take the input from the user

    if(argc != 3)
    {
        printf("Error - Usage of form : %s <rows> <cols>\n",argv[0]);
        return 1;
    }

    int rows = (int) strtol(argv[1], NULL, 10);
    int cols = (int) strtol(argv[2], NULL, 10);
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
    // terminal_control_unit(&ss_my);

    printf("Value of Cell at 0,0 : %d\n", (ss_my.arr[0*ss_my.SS_COLS+0].cell)->value);

    ss_my.arr[0*ss_my.SS_COLS+0].dependent_scells->scell_ptrs[0] = &(ss_my.arr[1*ss_my.SS_COLS+0]);
    ss_my.arr[0*ss_my.SS_COLS+0].dependent_scells->scell_ptrs[1] = &(ss_my.arr[1*ss_my.SS_COLS+1]);
    ss_my.arr[0*ss_my.SS_COLS+0].dependent_scells->scell_ptrs[2] = &(ss_my.arr[1*ss_my.SS_COLS+2]);
    ss_my.arr[0*ss_my.SS_COLS+0].dependent_scells->scell_ptrs[3] = &(ss_my.arr[2*ss_my.SS_COLS+1]);
    ss_my.arr[0*ss_my.SS_COLS+0].dependent_scells->scell_ptrs[3] = &(ss_my.arr[2*ss_my.SS_COLS+1]);
    ss_my.arr[0*ss_my.SS_COLS+0].dependent_scells->scell_ptrs[3] = &(ss_my.arr[2*ss_my.SS_COLS+1]);
    ss_my.arr[0*ss_my.SS_COLS+0].dependent_scells->scell_ptrs[3] = &(ss_my.arr[2*ss_my.SS_COLS+1]);


    for(int i = 1; i < 20; i++)
    {
        push_back_scell_ptrs(ss_my.arr[0*ss_my.SS_COLS+0].dependent_scells, &(ss_my.arr[i*ss_my.SS_COLS+0]));
    }
    // ss_my.arr[0*ss_my.SS_COLS+0].dependent_scells->size = 4;

    // debug_print_scell();

    SIM_BOOL cycle_exists = FALSE;
    Stack_SCell visitedStack;
    init_stack(&visitedStack, 10);

    dfs_cycle_check(&ss_my.arr[0*ss_my.SS_COLS+0],&ss_my.arr[4*ss_my.SS_COLS+4],&ss_my.arr[5*ss_my.SS_COLS+5],&visitedStack,&cycle_exists);
    // (ss_my.arr[0*ss_my.SS_COLS+0].cell)->value ;
    printf("\n");
    pop_and_unmark(&visitedStack);

    return 0;
}

// ------------------------------------------------------------------------- //