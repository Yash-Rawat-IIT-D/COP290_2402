// Cell Struct Definition and Functions

#include <stdlib.h>
#include "../header/cell.h"  // Include the header file for declaration

// ------------------------------------------------------------------------- //

typedef struct Cell
{
    int row;
    int col;
    double value;

} Cell;

// ------------------------------------------------------------------------- //

void init_cell(Cell *cell, int row, int col, double value)
{
    cell->row = row;
    cell->col = col;
    cell->value = value;
}

void set_cell_value(Cell *cell, double value)
{
    cell->value = value;
}

double get_cell_value(Cell *cell)
{
    return cell->value;
}

int get_cell_row(Cell *cell)
{
    return cell->row;
}

int get_cell_col(Cell *cell)
{
    return cell->col;
}

// ------------------------------------------------------------------------- //