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
    int rows = MAX_SS_ROWS;
    int cols = MAX_SS_COLS;

    // Setting the const values of the struct
    // Expected First Step of Spread Sheet Creation

    Spread_Sheet ss_my = {.SS_ROWS = rows, .SS_COLS = cols};
    // init_spread_sheet(&ss_my);

    // printf("Rows: %d\n", ss_my.SS_ROWS);
    // printf("Cols: %d\n", ss_my.SS_COLS);

    // Rendering the Spread Sheet
    // render_ss(&ss_my, 0, 0);

    // char *testcells[] = {"A1","B4","DDF11","H9"};
    char *testcells[] = {"A1", "B4", "A135", "H9", "C3", "E5", "G7", "I2", "J10", "K12"};

    for (int i = 0; i < 10; i++)
    {
        if (is_cell(testcells[i], rows, cols))
        {
            printf("Is a valid Cell : %s\n", testcells[i]);
        }
        else
        {
            printf("Is an invalid Cell : %s\n",testcells[i]);
        }
    }
    return 0;
}

// ------------------------------------------------------------------------- //