// CONSTANTS_H // Start of Header File

// ------------------------------------------------------------------------- //

#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
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
    SS_OK = 0,
    MALLOC_SCELL_PTR = 1,
    MALLOC_SCELL = 2,
    MALLOC_CELL = 3,

    MALLOC_CELL1D = 4,
    MALLOC_CELL1D_CELL_PTR = 5,    
    MALLOC_CELL_FORMULA = 6,

    ROW_OUT_OF_BOUNDS = 7,
    COL_OUT_OF_BOUNDS = 8
} SS_EXIT_CODE;

typedef enum 
{
    FALSE = 0,
    TRUE = 1,
    SBERROR = 2,
} SIM_BOOL;

typedef enum 
{
    ZERO_D = 0,
    ONE_D = 1,
    TWO_D = 2,
} CELL_RANGE_TYPE;

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
    TCU_OK = 0,
    INVALID_INPUT = 1,
    OUT_OF_RANGE = 2,
    MALLOC_FAILED = 3,
    UNKNOWN_ERROR = 4
} TCU_EXIT_CODE;



#define LO_RESIZE_THRESHOLD 16
// ------------------------------------------------------------------------- //

#endif 
// CONSTANTS_H // End of Header File