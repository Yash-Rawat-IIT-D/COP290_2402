// SPREADSHEET_H // Header Starts

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


// ------------------------------------------------------------------------- //

// Function Prototypes for Spread_Sheet
void init_Spread_Sheet(Spread_Sheet *ss);

// ------------------------------------------------------------------------- //

#endif 
// SPREADSHEET_H // Header Ends 