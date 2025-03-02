// CONSTANTS_H // Start of Header File

// ------------------------------------------------------------------------- //
#define LONG_SPACER "----------------------------------------------------------------"
#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#define MAX_SS_ROWS 999
#define MAX_SS_COLS 18278 // 26^3 + 26^2 + 26
#define MAX_RENDER_DIM 10
#define MIN_COL_WIDTH 14
#define IN_BUFF_SIZE 100
#define COL_HEADER_BUFF_SIZE 4
#define COL_DATA_BUFF_SIZE 12
#define SPACER ""
#define SPACER_00 "              "
#define COL_SPACER ' '
#define LO_RESIZE_THRESHOLD 32
#define INIT_DEPENDENCY_SIZE 4
#define INIT_SCELL_SIZE 5

#define REGEX_PATTERN \
  "^[ \t]*" \
  "([A-Z]{1,3}[1-9][0-9]{0,2})" \
  "[ \t]*=[ \t]*" \
  "(" \
    /* 1) A pure integer constant with optional sign */ \
    "([+-]?[0-9]+)" \
    /* 2) A single cell reference */ \
    "|([A-Z]{1,3}[1-9][0-9]{0,2})" \
    /* 3) An arithmetic expression: (constant|cell) OP (constant|cell) */ \
    "|((([+-]?[0-9]+)|([A-Z]{1,3}[1-9][0-9]{0,2}))[ \t]*[+*/-][ \t]*" \
       "(([+-]?[0-9]+)|([A-Z]{1,3}[1-9][0-9]{0,2})))" \
    /* 4) A range function call: MIN|MAX|AVG|SUM|STDEV(...) */ \
    "|((MIN|MAX|AVG|SUM|STDEV)\\([ \t]*" \
       "([A-Z]{1,3}[1-9][0-9]{0,2}:[A-Z]{1,3}[1-9][0-9]{0,2})" \
       "[ \t]*\\))" \
    /* 5) A SLEEP(...) call, which takes either a positive int or cell */ \
    "|(SLEEP\\([ \t]*" \
       "((([0-9]+)|([A-Z]{1,3}[1-9][0-9]{0,2})))" \
       "[ \t]*\\))" \
  ")" \
  "[ \t]*$"

// ------------------------------------------------------------------------- //

#define NORMAL '0'
#define INVALID_CELL_BUFF '1'

// ------------------------------------------------------------------------- //

#endif 
// CONSTANTS_H // End of Header File