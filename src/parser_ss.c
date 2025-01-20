#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "../header/spreadsheet.h"
#include "../header/parser_ss.h"

// ------------------------------------------------------------------------- //


// Accepts one-based indexing of Column
// So supply it as such

void col_encoder(int col_num, char *col_header_buff)
{
    if(col_num < 1 || col_num > MAX_SS_COLS)
    {
        printf("Column Is Out of Range\n");
        exit(1);
    }

    int ind = 0;
    char col_hbuff_temp[COL_HEADER_BUFF_SIZE]; 

    while (col_num > 0)
    {
        // printf("Col_num: %d || ind : %d\n", col_num, ind);
        col_num -= 1;
        col_hbuff_temp[ind++] = (col_num % 26) + 'A';
        col_num /= 26;
    }

    // printf("col_dbuff_temp: %s\n", col_dbuff_temp);

    // printf("Ind: %d\n", ind);

    for (int i = 0; i < ind; i++)
    {
        // printf("i: %d , cdbufftemp : %d\n", i, col_dbuff_temp[ind - i - 1]);
        col_header_buff[i] = col_hbuff_temp[ind - i - 1];
    }

    // Terminating with NULL Character
    col_header_buff[ind] = '\0'; 
}

// ------------------------------------------------------------------------- //

// Gives one-based indexing of Column
// For implementation purposes , use zero-based indexing

int col_decoder(char *col_data_buff)
{
    int col_dbuff_size = strlen(col_data_buff);

    // Error Checking

    if(col_dbuff_size < 1 || col_dbuff_size > 4)
    {
        printf("Column Name is out of bounds\n");
        exit(1);
    }
    else
    {
        for(int i = 0; i < col_dbuff_size; i++)
        {
            if(col_data_buff[i] == '\0')
            {
                break;
            }
            if(!(isupper(col_data_buff[i])))
            {
                printf("Column Name is not Invalid\n");
                exit(1);
            }   
        }
    }

    int res_col_num = 0;

    for(int i = 0; i < col_dbuff_size; i++)
    {
        res_col_num = res_col_num * 26 + (col_data_buff[i] - 'A' + 1);
    }

    return res_col_num;
}

// ------------------------------------------------------------------------- //

// Sets the output buffer for a single cell using the data in the cell
// Correctness ensured by the input data parameters

void set_out_buff(char *obuff, char *col_data_buff)
{      
    int col_dbuff_size = strlen(col_data_buff);
    int left_padding = (MIN_COL_WIDTH - col_dbuff_size) / 2;
    int right_padding = MIN_COL_WIDTH - col_dbuff_size - left_padding;

    for(int i = 0; i < left_padding; i++)
    {
        obuff[i] = ' ';
    }

    for(int i = 0; i < col_dbuff_size; i++)
    {
        obuff[left_padding + i] = col_data_buff[i];
    }

    for(int i = 0; i < right_padding; i++)
    {
        obuff[left_padding + col_dbuff_size + i] = COL_SPACER;
    }

}

// ------------------------------------------------------------------------- //

// Renders the Spread_Sheet , with the cell at row and col as the top left cell
// The Spread_Sheet is rendered in the terminal with 
void render_ss(Spread_Sheet *ss, int row, int col)
{   
    // Output Buffer, Column Header and Data Buffer
    
    // ? Might add a check for row and column later

    char obuff[MIN_COL_WIDTH];
    char col_header_buff[COL_HEADER_BUFF_SIZE];
    char col_data_buff[COL_DATA_BUFF_SIZE];

    // Printing the Column Names

    printf("%s",SPACER);
    printf("%*s%s",MIN_COL_WIDTH, "==============", SPACER);

    for (int j = col; j < ss->SS_COLS; j++)
    {
        col_encoder(j + 1, col_header_buff);
        set_out_buff(obuff, col_header_buff);
        printf("%*s%s", MIN_COL_WIDTH, obuff, SPACER);
    }    

    // Printing the Rows

    for(int i = row; i < ss->SS_ROWS; i++)
    {
        printf("\n");

        sprintf(col_data_buff,"%d",i+1);
        printf("%s",SPACER);
        set_out_buff(obuff, col_data_buff);
        printf("%*s%s",MIN_COL_WIDTH,obuff,SPACER);

        for (int j = col; j < ss->SS_COLS; j++)
        {
            sprintf(col_data_buff,"%.2f",(((ss->arr[i][j]).cell).value));
            set_out_buff(obuff, col_data_buff);
            printf("%*s%s", MIN_COL_WIDTH, obuff, SPACER);
        }
    }

    printf("\n");
}

// ------------------------------------------------------------------------- //