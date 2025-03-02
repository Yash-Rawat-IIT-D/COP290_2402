// SPREADHEET_H // Start of Header File

#ifndef SPREADSHEET_H   
#define SPREADSHEET_H

#include <unistd.h>
#include "constants.h"
#include <math.h>
#include <stdbool.h>

// ------------------------------------------------------------------------- //

// SCell : Structure to store the data of a cell in the spreadsheet
// Pair : (row, col) coordinates of the cell in the spreadsheet
typedef struct CELL_FORMULA CELL_FORMULA;
typedef struct SCell SCell;
typedef struct Pair Pair;
typedef struct Spread_Sheet Spread_Sheet;
typedef struct Stack_SCell Stack_SCell;


// Pair : Structure to store the coordinates of a cell in the spreadsheet

typedef struct Pair
{
    short int x;
    short int y;
} Pair;

Pair make_pair(int x, int y);

// SCell : Structure to store the data of a cell in the spreadsheet

typedef struct SCell
{
    int value;

    CELL_FORMULA *cell_formula;

    Pair * dependent_scells;
    int dependent_scells_size;
    int dependent_scells_capacity;
    
    
    char visited_err_flag;

} SCell;

// Spread_Sheet : Structure to store the data of the spreadsheet

typedef struct Spread_Sheet
{
    SCell *arr;
    int SS_ROWS;
    int SS_COLS;    
} Spread_Sheet;

// ------------------------------------------------------------------------- //

// Function Prototypes for the SCell Structure

char init_scell(SCell * scell);
void debug_print_scell(Spread_Sheet *ss, int row, int col);

// ------------------------------------------------------------------------- //

// Function Prototypes for the Spread_Sheet Structure

Spread_Sheet * init_spread_sheet(int rows, int cols);

SCell * get_scell_by_coordinates(Spread_Sheet *ss, int row, int col);


// ------------------------------------------------------------------------- //

// Cell_Formula : Structure to store the formula of a cell in the spreadsheet

typedef struct FVALUE_CONS
{
    int value;

} FVALUE_CONS;

typedef struct FVALUE_CELL
{
    short int cell_row;
    short int cell_col;

} FVALUE_CELL;

typedef struct FARITHMETIC_CONS_CONS
{
    char arithmetic_op;
    int left_value, right_value;

} FARITHMETIC_CONS_CONS;

typedef struct FARITHMETIC_CONS_CELL
{
    char arithmetic_op;
    int left_value;
    short int right_cell_row;
    short int right_cell_col;

} FARITHMETIC_CONS_CELL;

typedef struct FARITHMETIC_CELL_CONS
{
    char arithmetic_op;
    short int left_cell_row;
    short int left_cell_col;
    int right_value;

} FARITHMETIC_CELL_CONS;

typedef struct FARITHMETIC_CELL_CELL
{
    char arithmetic_op;
    short int left_cell_row;
    short int left_cell_col;
    short int right_cell_row;
    short int right_cell_col;

} FARITHMETIC_CELL_CELL;


typedef struct FSLEEP_CONS
{
    int sleep_time;
} FSLEEP_CONS;

typedef struct FSLEEP_CELL
{
    short int sleep_cell_row;
    short int sleep_cell_col;

} FSLEEP_CELL;

typedef struct FFUNCTION
{
    char function;
    short int start_row;
    short int start_col;
    short int end_row;
    short int end_col;

} FFUNCTION;

typedef struct CELL_FORMULA
{
    char valid_exp_type; // 0, 1, 2, 3, 4, 5, 6, 7, 8
    union
    {
        FVALUE_CONS fvcons; // '0'
        FVALUE_CELL fvcell; // '1'
        FARITHMETIC_CONS_CONS farith_cons_cons; // '2'
        FARITHMETIC_CONS_CELL farith_cons_cell; // '3'
        FARITHMETIC_CELL_CONS farith_cell_cons; // '4'
        FARITHMETIC_CELL_CELL farith_cell_cell; // '5'
        FSLEEP_CONS fsleep_cons; // '6'
        FSLEEP_CELL fsleep_cell; // '7'
        FFUNCTION ffunc;    // '8'
    };

} CELL_FORMULA;


// Function Prototypes for the Cell_Formula Structure

char init_cell_formula(CELL_FORMULA * cf);
void debug_print_cell_formula(Spread_Sheet *ss, Pair p);

// ------------------------------------------------------------------------- //

// Methods to deal with the dependencies of a cell

char resize_dependency_list(Spread_Sheet *ss, Pair target, int new_capacity);
char add_dependency_to_cell(Spread_Sheet *ss, Pair target, Pair dep);
char remove_dependency_from_cell(Spread_Sheet *ss, Pair target, Pair dep);

// ------------------------------------------------------------------------- //

// Stack_SCell : Structure to Aid in the implementation of the DFS algorithm

typedef struct Stack_SCell 
{
    Pair* items;
    int top;
    int capacity;
} Stack_SCell;

char init_stack(Stack_SCell *stack, int capacity);

char push_stack(Stack_SCell *stack, Pair item);

Pair pop_stack(Stack_SCell *stack);

void free_stack(Stack_SCell *stack);

// ------------------------------------------------------------------------- //

// Function Prototypes for the DFS Algorithm and the Update Logic

void dfs_topological(Spread_Sheet *ss, Pair data, Stack_SCell *stack);

char dfs_cycle_check(Spread_Sheet *ss, Pair data_node, Pair data_tl, Pair data_br);

char update_cell_value(Spread_Sheet *ss, Pair node);

char topological_sort_and_update(Spread_Sheet *ss, Pair start);

char remove_old_dependencies(Spread_Sheet *ss, Pair target);

char add_new_dependencies(Spread_Sheet *ss, Pair node, Pair data_tl, Pair data_br);

char update_logic_unit(Spread_Sheet *ss, Pair target, CELL_FORMULA *new_formula);

// ------------------------------------------------------------------------- //

#endif

// SPREADSHEET_H // End of Header File