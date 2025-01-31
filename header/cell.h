// CELL_H // Start of Header File

// ------------------------------------------------------------------------- //

#ifndef CELL_H
#define CELL_H
#include <stdio.h>
#include <stdlib.h>
#include <parser_ss.h>

// ------------------------------------------------------------------------- //


enum RANGE_TYPE
{
    ZERO_D = 0,
    ONE_D = 1,
    TWO_D = 2,
};

typedef struct Cell
{
    int row;
    int col;
    double value;
    Cell_Range *cell_range_left, *cell_range_right; 

} Cell;

typedef struct Cell_Range
{
    Cell * start_cell;
    Cell * end_cell;
    enum RANGE_TYPE cell_range_type;

} Cell_Range;

// ------------------------------------------------------------------------- //


// Function Prototypes of Cell Struct and Cell Range Struct

void init_cell(Cell *cell, int row, int col, double value);
void set_cell_value(Cell *cell, double value);
double get_cell_value(Cell *cell);
int get_cell_row(Cell *cell);
int get_cell_col(Cell *cell);
Cell_Range * get_cell_left(Cell *cell);
Cell_Range * get_cell_right(Cell *cell);
int is_valid_cell_range(Cell_Range *cell_range);

// ------------------------------------------------------------------------- //

#endif 

// CELL_H // End of Header File