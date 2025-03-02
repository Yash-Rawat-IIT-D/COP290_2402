#include <stdio.h>
#include <stdlib.h>

#include "../header/spreadsheet.h"
#include "../header/parser.h"
#include "../header/constants.h"

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


    Spread_Sheet * ss_my = init_spread_sheet(rows, cols); 

    if(ss_my == NULL)
    {
        printf("Error in Spread Sheet Initialization\n");
        return 1;
    }

    printf("Rows: %d, Cols: %d\n", ss_my->SS_ROWS ,ss_my->SS_COLS);
    // printf("%lu\n",sizeof(SCell));
    terminal_control_unit(ss_my);
    return 0;
    // return 0;
}