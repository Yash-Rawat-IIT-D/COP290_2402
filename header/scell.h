// SCELL_H // Start of Header File

// ------------------------------------------------------------------------- //

#ifndef SCELL_H
#define SCELL_H
#include "cell.h"
#include "scell1d.h"
#include "constants.h"  
#include <stdbool.h>

// ------------------------------------------------------------------------- //

typedef struct SCell1D SCell1D;

typedef struct SCell
{
    Cell * cell;
    SCell1D *dependent_scells;
    SCell1D *precedent_scells;

    Cell_Formula * cell_formula;
    SIM_BOOL visited;
} SCell;


void init_scell(SCell *scell, int row, int col, int value, SS_EXIT_CODE *exit_code);
void debug_print_scell(SCell *scell);
// ------------------------------------------------------------------------- //

#endif

// SCELL_H // End of Header File