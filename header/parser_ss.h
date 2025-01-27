// RENDER_SS_H // Start of Header File

#ifndef RENDER_SS_H
#define RENDER_SS_H

#include "cell.h"
#include "spreadsheet.h"
#define MAX_SS_ROWS 999
#define MAX_SS_COLS 18278 // 26^3 + 26^2 + 26
#define MAX_RENDER_DIM 10
#define MIN_COL_WIDTH 14
#define IN_BUFF_SIZE 100
#define COL_HEADER_BUFF_SIZE 4
#define COL_DATA_BUFF_SIZE 12
#define SPACER "|"
#define COL_SPACER ' '

// ------------------------------------------------------------------------- //

enum SIM_BOOL
{
    FALSE = 0,
    TRUE = 1
};

enum USER_INPUTS
{
    CONTROL_IN = 0,
    FORMULA_IN = 1,
    INVALID_IN = 2
};

enum VALID_EXP
{
    VALUE = 0,
    VALUE_OP_VALUE = 1,
    FUNCTION = 2,
};

enum ARITHMETIC_OP
{
    ADDITION = 0,
    SUBTRACTION = 1,
    MULTIPLICATION = 2,
    DIVISION = 3
};

enum FUNCTION
{
    MIN = 0,
    MAX = 1,
    AVG = 2,
    SUM = 3,
    STDEV = 4,
    SLEEP = 5,
};

typedef struct Cell_Formula
{
    enum VALID_EXP valid_exp_type;
    
    // valid_exp_type = VALUE
    enum ARITHMETIC_OP arithmetic_op;
    enum SIM_BOOL is_left_value_constant, is_right_value_constant;
    double left_value, right_value;
    Cell *left_cell, *right_cell;

    // valid_exp_type = FUNCTION
    enum FUNCTION function;
    Cell_Range *cell_range;

} Cell_Formula;



// Function Prototypes

void col_encoder(int col_num, char *col_data_buff);
int col_decoder(char *col_data_buff);
int is_cell(char data_buff[], int SS_ROWS, int SS_COLS);
int safe_render_dim(int rc, int rc_max);
void set_out_buff(char *obuff, char *col_data_buff);
void render_ss(Spread_Sheet *ss, int row, int col);
void terminal_control_unit(Spread_Sheet *ss);

// ------------------------------------------------------------------------- //

#endif
// RENDER_SS_H // End of Header File