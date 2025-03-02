// RENDER_SS_H // Start of Header File

#ifndef RENDER_SS_H
#define RENDER_SS_H


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <regex.h>
#include <stdbool.h>

#define _POSIX_C_SOURCE 200809L
#include <time.h>


#include "spreadsheet.h"
#include "constants.h"


// ------------------------------------------------------------------------- //

// Function Prototypes for rendering and utilities

void col_encoder(int col_num, char *col_header_buff);
int col_decoder(char *col_data_buff);
void set_out_buff(char *obuff, char *col_data_buff);
int safe_render_dim(int rc, int rc_max);
int next_render_dim(int rc, int rc_max, int step_size);
void render_ss(Spread_Sheet *ss, int row, int col);
void set_error_message(char exit_code, char error_buff[]);
void terminal_control_unit(Spread_Sheet *ss);


// ------------------------------------------------------------------------- //

// Function Prototypes for parsing and utilities

bool is_valid_cell(char data_buff[], int SS_ROWS, int SS_COLS, int *tcell_row, int *tcell_col, char *exit_code);
bool is_range(char data_buff[], int SS_ROWS, int SS_COLS, char *exit_code);
void trim_whitespace(char *str);
void parse_command(char command_buff[], char target_cell_buff[], char exp_buff[], Spread_Sheet *ss, char *exit_code);
void parse_expression(char exp_buff[], char target_cell_buff[], Spread_Sheet *ss, char *exit_code);
void parse_operand_expr(const char *operand, CELL_FORMULA *cf, Spread_Sheet *ss, char *exit_code);
void parse_arithmetic_expr(const char *arithmetic_exp, CELL_FORMULA *cf, Spread_Sheet *ss, char *exit_code);
void parse_function_expr(const char *function_exp, CELL_FORMULA *cf, Spread_Sheet *ss, char *exit_code);



void parse_cell_name(const char *cell_str, int *row, int *col);

// ------------------------------------------------------------------------- //

#endif
// RENDER_SS_H // End of Header File