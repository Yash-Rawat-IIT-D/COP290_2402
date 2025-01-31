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

typedef enum 
{
    FALSE = 0,
    TRUE = 1
} SIM_BOOL;

typedef enum 
{
    CONTROL_IN = 0,
    FORMULA_IN = 1,
    INVALID_IN = 2
} USER_INPUTS;

typedef enum 
{
    VALUE = 0,
    VALUE_OP_VALUE = 1,
    FUNCT_ON_RANGE = 2
} VALID_EXP;

typedef enum 
{
    ADDITION = 0,
    SUBTRACTION = 1,
    MULTIPLICATION = 2,
    DIVISION = 3
} ARITHMETIC_OP;

typedef enum 
{
    MIN = 0,
    MAX = 1,
    AVG = 2,
    SUM = 3,
    STDEV = 4,
    SLEEP = 5
} FUNCTION;

typedef enum 
{
    OK = 0,
    INVALID_INPUT = 1,
    OUT_OF_RANGE = 2,
    MALLOC_FAILED = 3,
    UNKNOWN_ERROR = 4
} ERROR_CODE;


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