#include <stdio.h>
#include <stdlib.h>

#include "../header/cell.h"
#include "../header/node.h"
#include "../header/node1d.h"
#include "../header/spreadsheet.h"


// ------------------------------------------------------------------------- //

int main()
{
    int rows = 10;
    int cols = 20;
    
    // Setting the const values of the struct 
    // Expected First Step of Spread Sheet Creation

    Spread_Sheet ss_my = {.SS_ROWS = rows, .SS_COLS = cols}; 
    init_Spread_Sheet(&ss_my);
    
    printf("Rows: %d\n", ss_my.SS_ROWS);
    printf("Cols: %d\n", ss_my.SS_COLS);


    return 0;
}

// ------------------------------------------------------------------------- //