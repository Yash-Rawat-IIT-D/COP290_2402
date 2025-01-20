// RENDER_SS_H // Start of Header File

#ifndef RENDER_SS_H
#define RENDER_SS_H

#include "spreadsheet.h"
#define MAX_SS_ROWS 999
#define MAX_SS_COLS 18278 // 26^3 + 26^2 + 26
#define MIN_COL_WIDTH 14
#define COL_HEADER_BUFF_SIZE 4
#define COL_DATA_BUFF_SIZE 12
#define SPACER "|"
#define COL_SPACER ' '
// ------------------------------------------------------------------------- //

// Function Prototypes

void col_encoder(int col_num, char *col_data_buff);
int col_decoder(char *col_data_buff);
void set_out_buff(char *obuff, char *col_data_buff);
void render_ss(Spread_Sheet *ss, int row, int col);

// ------------------------------------------------------------------------- //

#endif
// RENDER_SS_H // End of Header File