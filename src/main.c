#include <stdio.h>
#include <stdlib.h>

#include "../header/cell.h"
#include "../header/cell1d.h"
#include "../header/node.h"
#include "../header/spreadsheet.h"
#include "../header/parser_ss.h"

// ------------------------------------------------------------------------- //

int main()
{
    int rows = 10;
    int cols = 10;

    // Setting the const values of the struct
    // Expected First Step of Spread Sheet Creation

    Spread_Sheet ss_my = {.SS_ROWS = rows, .SS_COLS = cols};
    init_spread_sheet(&ss_my);

    printf("Rows: %d\n", ss_my.SS_ROWS);
    printf("Cols: %d\n", ss_my.SS_COLS);

    // Rendering the Spread Sheet
    render_ss(&ss_my, 0, 0);

    return 0;
}

// ------------------------------------------------------------------------- //