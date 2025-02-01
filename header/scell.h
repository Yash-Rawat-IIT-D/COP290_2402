// SCELL_H // Start of Header File

// ------------------------------------------------------------------------- //

#ifndef SCELL_H
#define SCELL_H
#include "cell.h"
#include "cell1d.h"
#include "constants.h"  

// ------------------------------------------------------------------------- //

typedef struct SCell
{
    Cell * cell;
    Cell1D *dependent_cells;
    Cell_Formula * cell_formula;

} SCell;


void init_scell(SCell *scell, int row, int col, double value, SS_EXIT_CODE *exit_code);

// ------------------------------------------------------------------------- //

#endif

// SCELL_H // End of Header File