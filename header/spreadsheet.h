// SPREADHEET_H // Start of Header File

#ifndef SPREADSHEET_H
#define SPREADSHEET_H
#include "cell.h"
#include "scell1d.h"
#include "scell.h"
#include "constants.h"
// ------------------------------------------------------------------------- //

typedef struct Spread_Sheet

{
    SCell *arr;
    const int SS_ROWS;
    const int SS_COLS;    
} Spread_Sheet;


// Function Prototypes

void init_spread_sheet(Spread_Sheet *ss, SS_EXIT_CODE *exit_code);
SCell *get_scell(Spread_Sheet *ss, int row, int col);

    
// ------------------------------------------------------------------------- //

#endif 
// SPREADSHEET_H // End of Header File