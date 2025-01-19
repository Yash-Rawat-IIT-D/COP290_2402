// CELL_H // Start of Header File

// ------------------------------------------------------------------------- //

#ifndef CELL_H
#define CELL_H

// ------------------------------------------------------------------------- //
typedef struct Cell
{
    int row;
    int col;
    double value;
} Cell;

// ------------------------------------------------------------------------- //


// Function Prototypes of Cell Struct

void init_cell(Cell *cell, int row, int col, double value);
void set_cell_value(Cell *cell, double value);
double get_cell_value(Cell *cell);
int get_cell_row(Cell *cell);
int get_cell_col(Cell *cell);

// ------------------------------------------------------------------------- //

#endif 

// CELL_H // End of Header File