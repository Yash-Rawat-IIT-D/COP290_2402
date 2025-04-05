// RENDER_SS_H // Start of Header File

#ifndef RENDER_SS_H
#define RENDER_SS_H


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <regex.h>
#include "cell.h"
#include "scell.h"  
#include "scell1d.h"
#include "spreadsheet.h"
#include "constants.h"
#include <stdbool.h>

     

// ------------------------------------------------------------------------- //

// Function Prototypes

void col_encoder(int col_num, char *col_data_buff);
int col_decoder(char *col_data_buff);
void set_out_buff(char *obuff, char *col_data_buff);

void is_cell(char data_buff[], int SS_ROWS, int SS_COLS, int *tcell_row, int *tcell_col, TCU_EXIT_CODE *exit_code);
bool is_valid_cell(char data_buff[], int SS_ROWS, int SS_COLS, int *tcell_row, int *tcell_col, TCU_EXIT_CODE *exit_code);
bool is_range(char data_buff[], int SS_ROWS, int SS_COLS, TCU_EXIT_CODE *exit_code);


bool is_function(char data_buff[], int SS_ROWS, int SS_COLS, TCU_EXIT_CODE *exit_code);
void trim_whitespace(char *str);

void parse_command(char command_buff[], char target_cell_buff[], char exp_buff[], Spread_Sheet *ss, TCU_EXIT_CODE *exit_code);
void parse_cell_name(const char *cell_str, int *row, int *col);
void parse_operand(const char *operand, SIM_BOOL *is_constant, int *value, Cell **cell, Spread_Sheet *ss, TCU_EXIT_CODE *exit_code);
void parse_value(const char *exp, Cell_Formula *formula, Spread_Sheet *ss, TCU_EXIT_CODE *exit_code);
void parse_arithmetic(const char *exp, Cell_Formula *formula, Spread_Sheet *ss, TCU_EXIT_CODE *exit_code);


Cell_Range * parse_range(const char *range_str, Spread_Sheet *ss, TCU_EXIT_CODE *exit_code);

void parse_function(const char *data_buff, Cell_Formula *formula, Spread_Sheet *ss, TCU_EXIT_CODE *exit_code);

double my_fabs(double x);
double sqrt(double x);

int evaluate_formula(Cell_Formula *formula, Spread_Sheet *ss,SIM_BOOL sleep_over_ride, TCU_EXIT_CODE *exit_code);

void parse_expression(char target_cell_buff[], char exp_buff[], Spread_Sheet *ss, TCU_EXIT_CODE *exit_code);
void update_logic_unit(Spread_Sheet *ss, SCell* node, Cell_Formula * cformula, TCU_EXIT_CODE *exit_code);



//sharma bhai start

// Remove the target cell from all precedent cells’ dependent lists.

void debug_print_scell(Spread_Sheet *ss, SCell *scell);
void remove_old_dependencies(Spread_Sheet *ss,  SCell *target);


// Add new dependency links based on the new formula.
// new_precedents is a dynamic array (SCell1D) containing pointers to the new precedent cells.
void add_new_dependencies(SCell *target, SCell *new_precedent_tl, SCell *new_precedent_br, Spread_Sheet *ss);

void pop_and_update(Stack_SCell * topo_sort_st, Spread_Sheet *ss, TCU_EXIT_CODE *exit_code);
void pop_and_unmark(Spread_Sheet *ss, Stack_SCell *visitedStack);

int safe_render_dim(int rc, int rc_max);

void render_ss(Spread_Sheet *ss, int row, int col);
void terminal_control_unit(Spread_Sheet *ss);

// ------------------------------------------------------------------------- //

#endif
// RENDER_SS_H // End of Header File