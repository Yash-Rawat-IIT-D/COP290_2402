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
    if (col_num < 1 || col_num > MAX_SS_COLS)
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

    if (col_dbuff_size < 1 || col_dbuff_size > 4)
    {
        printf("Column Name is out of bounds\n");
        exit(1);
    }
    else
    {
        for (int i = 0; i < col_dbuff_size; i++)
        {
            if (col_data_buff[i] == '\0')
            {
                break;
            }
            if (!(isupper(col_data_buff[i])))
            {
                printf("Column Name is not Invalid\n");
                exit(1);
            }
        }
    }

    int res_col_num = 0;

    for (int i = 0; i < col_dbuff_size; i++)
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

    for (int i = 0; i < left_padding; i++)
    {
        obuff[i] = ' ';
    }

    for (int i = 0; i < col_dbuff_size; i++)
    {
        obuff[left_padding + i] = col_data_buff[i];
    }

    for (int i = 0; i < right_padding; i++)
    {
        obuff[left_padding + col_dbuff_size + i] = COL_SPACER;
    }
}

// ------------------------------------------------------------------------- //

// Return Value = 1 ---> Is a Valid Cell
// Return Value = 0 ---> Invalid Cell

int is_cell(char data_buff[], int SS_ROWS, int SS_COLS)
{
    int len_dbuff = strlen(data_buff), i = 0;
    if ((len_dbuff < 2) || (len_dbuff > 6))
    {
        printf("Error : Not a valid Cell Input");
        return -1;
    }

    int col_part_processed = 0;
    // int row_part_processed = 0;

    char col_buff[7], row_buff[7];
    int cbuff_p = 0, rbuff_p = 0;

    while (i < len_dbuff)
    {
        if (isupper(data_buff[i]))
        {
            if (col_part_processed)
            {
                printf("Error: Invalid Cell Input\n");
                return 0;
            }
            col_buff[cbuff_p++] = data_buff[i];
        }
        else if (isdigit(data_buff[i]))
        {
            if (!col_part_processed)
            {
                col_part_processed = 1;
            }

            row_buff[rbuff_p++] = data_buff[i];
        }
        else
        {
            printf("Error: Invalid Cell Input\n");
            return 0;
        }
        i++;
    }

    col_buff[cbuff_p] = '\0';
    row_buff[rbuff_p] = '\0';

    // printf("Row Buff=%s, Col Buff=%s\n",row_buff,col_buff);

    if (cbuff_p == 0 || rbuff_p == 0)
    {
        printf("Error: Invalid Cell Input\n");
        return 0;
    }

    int col_num = col_decoder(col_buff);
    int row_num = atoi(row_buff);

    if (col_num < 1 || col_num > SS_COLS || row_num < 1 || row_num > SS_ROWS)
    {
        printf("Error: Cell Out of Range\n");
        return 0;
    }

    return 1;
}

// ------------------------------------------------------------------------- //
// void parse_input(int *user_input, int *target_cell)
// {
//     char in_buff[200];
//     if (fgets(in_buff, sizeof(in_buff), stdin) != NULL)
//     {
//         printf("You entered %s", in_buff);
//     }
//     else
//     {
//         printf("Error while reading input !\n");
//         exit(-1);
//     }
// }
// ------------------------------------------------------------------------- //

int safe_render_dim(int rc, int rc_max)
{
    if(rc + MAX_RENDER_DIM > rc_max)
    {
        return rc_max;
    }
    else
    {
        return rc+MAX_RENDER_DIM;
    }
}


// Need to take a look at this function's error handling
// Will implement generic error handling later I guess


int next_render_dim(int rc, int rc_max, int step_size)
{
    return (rc+step_size)%rc_max;
}

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

    printf("%s", SPACER);
    printf("%*s%s", MIN_COL_WIDTH, "==============", SPACER);

    for (int j = col; j < safe_render_dim(col,ss->SS_COLS); j++)
    {
        col_encoder(j + 1, col_header_buff);
        set_out_buff(obuff, col_header_buff);
        printf("%*s%s", MIN_COL_WIDTH, obuff, SPACER);
    }

    // Printing the Rows

    for (int i = row; i < safe_render_dim(row,ss->SS_ROWS); i++)
    {
        printf("\n");

        sprintf(col_data_buff, "%d", i + 1);
        printf("%s", SPACER);
        set_out_buff(obuff, col_data_buff);
        printf("%*s%s", MIN_COL_WIDTH, obuff, SPACER);

        // printf("Hi printing row [%d]",i);
        for (int j = col; j < safe_render_dim(col,ss->SS_COLS); j++)
        {
            sprintf(col_data_buff, "%.2f", (((ss->arr[i][j]).cell).value));
            // printf("Value at [%d][%d] = %.2f\n",i,j,(((ss->arr[i][j]).cell).value));
            set_out_buff(obuff, col_data_buff);
            printf("%*s%s", MIN_COL_WIDTH, obuff, SPACER);
        }
    }

    printf("\n");
}

void terminal_control_unit(Spread_Sheet *ss)
{
    int row_render = 0, col_render = 0;
    int error_flag = 0;
    char command_buff[100];
    char error_buff[100];

    while (1)
    {
        render_ss(ss, row_render, col_render);
        if (error_flag)
        {
            printf("[0.1] (%s) > ", error_buff);
            error_flag = 0;
        }

        else
        {
            printf("[0.0] (ok) > ");
        }

        if (fgets(command_buff, sizeof(command_buff), stdin) != NULL)
        {
            if (strcmp(command_buff, "q\n") == 0)
            {
                printf("Quitting the Spreadsheet Program\n");
                break;
            }
            else if(strcmp(command_buff, "w\n") == 0)
            {
                row_render = next_render_dim(row_render, ss->SS_ROWS, -MAX_RENDER_DIM);
            }
            else if(strcmp(command_buff, "s\n") == 0)
            {
                row_render = next_render_dim(row_render, ss->SS_ROWS, MAX_RENDER_DIM);
            }
            else if(strcmp(command_buff, "a\n") == 0)
            {
                col_render = next_render_dim(col_render, ss->SS_COLS, -MAX_RENDER_DIM);
            }
            else if(strcmp(command_buff, "d\n") == 0)
            {
                col_render = next_render_dim(col_render, ss->SS_COLS, MAX_RENDER_DIM);
            }
            else
            {
                printf("You entered %s", command_buff);
            }
        }
        else
        {
            printf("Error while reading input !\n");
            exit(-1);
        }
    }

    return;
}
// ------------------------------------------------------------------------- //