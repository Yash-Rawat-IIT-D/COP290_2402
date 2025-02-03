// RENDER_SS_H // Start of Header File

#ifndef RENDER_SS_H
#define RENDER_SS_H
#define REGEX_PATTERN "^[ \t]*([A-Z]{1,3}[1-9][0-9]{0,2})[ \t]*=[ \t]*(.*?)[ \t]*$"
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <regex.h>
#include "cell.h"
#include "spreadsheet.h"
#include "constants.h"

// ------------------------------------------------------------------------- //

// Function Prototypes

void col_encoder(int col_num, char *col_data_buff);
int col_decoder(char *col_data_buff);
void is_cell(char data_buff[], int SS_ROWS, int SS_COLS, int *tcell_row, int *tcell_col, TCU_EXIT_CODE *exit_code);
int safe_render_dim(int rc, int rc_max);
void set_out_buff(char *obuff, char *col_data_buff);
void render_ss(Spread_Sheet *ss, int row, int col);
void terminal_control_unit(Spread_Sheet *ss);

// ------------------------------------------------------------------------- //

#endif
// RENDER_SS_H // End of Header File