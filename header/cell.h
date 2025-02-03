// CELL_H // Start of Header File

// ------------------------------------------------------------------------- //

#ifndef CELL_H
#define CELL_H
#include "constants.h"

// ------------------------------------------------------------------------- //

typedef struct Cell
{
    int row;
    int col;
    int value;

} Cell;

typedef struct Cell_Range
{
    Cell * start_cell;
    Cell * end_cell;
    CELL_RANGE_TYPE cell_range_type;

} Cell_Range;

typedef struct Cell_Formula
{
    VALID_EXP valid_exp_type;
    

    // valid_exp_type = VALUE
    SIM_BOOL is_constant;
    double value;
    Cell *cell;


    // valid_exp_type = VALUE_OP_VALUE
    ARITHMETIC_OP arithmetic_op;
    SIM_BOOL is_left_value_constant, is_right_value_constant;
    double left_value, right_value;
    Cell *left_cell, *right_cell;

    // valid_exp_type = FUNCTION
    FUNCTION function;
    Cell_Range *cell_range;

} Cell_Formula;



// ------------------------------------------------------------------------- //

// Function Prototypes of Cell Struct and Cell Range Struct

void init_cell(Cell *cell, int row, int col, int value);
void set_cell_value(Cell *cell, double value);
double get_cell_value(Cell *cell);
int get_cell_row(Cell *cell);
int get_cell_col(Cell *cell);
SIM_BOOL is_valid_cell_range(Cell_Range *cell_range);

// ------------------------------------------------------------------------- //

#endif 

// CELL_H // End of Header File