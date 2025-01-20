// SPREADHEET_H // Start of Header File

#ifndef SPREADSHEET_H
#define SPREADSHEET_H

#include "node.h"

// ------------------------------------------------------------------------- //

typedef struct Spread_Sheet
{
    Node **arr;
    const int SS_ROWS;
    const int SS_COLS;
} Spread_Sheet;

// Function Prototypes

void init_spread_sheet(Spread_Sheet *ss);

// ------------------------------------------------------------------------- //

#endif 
// SPREADSHEET_H // End of Header File