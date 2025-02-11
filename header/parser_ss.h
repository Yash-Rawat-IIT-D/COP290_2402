// RENDER_SS_H // Start of Header File

#ifndef RENDER_SS_H
#define RENDER_SS_H
#define REGEX_PATTERN \
  "^[ \t]*" \
  "([A-Z]{1,3}[1-9][0-9]{0,2})" \
  "[ \t]*=[ \t]*" \
  "(" \
    /* 1) A pure integer constant */ \
    "([0-9]+)" \
    /* 2) A single cell reference */ \
    "|([A-Z]{1,3}[1-9][0-9]{0,2})" \
    /* 3) An arithmetic expression: (constant|cell) OP (constant|cell) */ \
    "|((([0-9]+)|([A-Z]{1,3}[1-9][0-9]{0,2}))[ \t]*[+*/-][ \t]*" \
       "(([0-9]+)|([A-Z]{1,3}[1-9][0-9]{0,2})))" \
    /* 4) A range function call: MIN|MAX|AVG|SUM|STDEV(...) */ \
    "|((MIN|MAX|AVG|SUM|STDEV)\\([ \t]*" \
       "([A-Z]{1,3}[1-9][0-9]{0,2}:[A-Z]{1,3}[1-9][0-9]{0,2})" \
       "[ \t]*\\))" \
    /* 5) A SLEEP(...) call, which takes either int or cell */ \
    "|(SLEEP\\([ \t]*" \
       "(([0-9]+)|([A-Z]{1,3}[1-9][0-9]{0,2}))" \
       "[ \t]*\\))" \
  ")" \
  "[ \t]*$"


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
void is_cell(char data_buff[], int SS_ROWS, int SS_COLS, int *tcell_row, int *tcell_col, TCU_EXIT_CODE *exit_code);
int safe_render_dim(int rc, int rc_max);
void set_out_buff(char *obuff, char *col_data_buff);
void render_ss(Spread_Sheet *ss, int row, int col);
void terminal_control_unit(Spread_Sheet *ss);



// ------------------------------------------------------------------------- //

#endif
// RENDER_SS_H // End of Header File