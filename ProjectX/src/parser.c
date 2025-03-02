#include "../header/parser.h"


// ------------------------------------------------------------------------- //

// Functions for Rendering and Utitlities

void col_encoder(int col_num, char *col_header_buff)
{
    if (col_num < 1 || col_num > MAX_SS_COLS)
    {
        // printf("Column Is Out of Range\n");
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

int col_decoder(char *col_data_buff)
{
    int col_dbuff_size = strlen(col_data_buff);

    // Error Checking

    if (col_dbuff_size < 1 || col_dbuff_size > 4)
    {
        // printf("Column Name is out of bounds\n");
        exit(1);
    }
    else
    {
        int col_num = 0;
        for (int i = 0; i < col_dbuff_size; i++)
        {
            col_num = col_num * 26 + (col_data_buff[i] - 'A' + 1);
        }

        return col_num;
    }
}

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

int safe_render_dim(int rc, int rc_max)
{
    // Returns the non‑inclusive end index for rendering.
    // If rc + MAX_RENDER_DIM exceeds rc_max, simply return rc_max.
    if (rc + MAX_RENDER_DIM > rc_max)
    {
        return rc_max;
    }
    else
    {
        return rc + MAX_RENDER_DIM;
    }
}

int next_render_dim(int rc, int rc_max, int step_size)
{
    // Calculate the candidate new top row.
    int new_rc = rc + step_size;

    // Clamp upward scrolling (cannot go below 0).
    if (new_rc < 0)
    {
        // *exit_code = TCU_OK;
        return 0;
    }

    // Clamp downward scrolling: ensure that (new_rc + MAX_RENDER_DIM) does not exceed rc_max.
    if (new_rc + MAX_RENDER_DIM > rc_max)
    {
        new_rc = rc_max - MAX_RENDER_DIM;
        if (new_rc < 0)
        {
            new_rc = 0;// In case there are fewer than MAX_RENDER_DIM rows.

        }         
        // *exit_code = TCU_OK;
        return new_rc;
    }

    // *exit_code = TCU_OK;
    return new_rc;
}

void render_ss(Spread_Sheet *ss, int row, int col)
{
    // Output Buffer, Column Header and Data Buffer

    // ? Might add a check for row and column later

    char obuff[MIN_COL_WIDTH];
    char col_header_buff[COL_HEADER_BUFF_SIZE];
    char col_data_buff[COL_DATA_BUFF_SIZE];
    // Printing the Column Names

    printf("%s", SPACER);
    printf("%*s%s", MIN_COL_WIDTH, SPACER_00, SPACER);

    for (int j = col; j < safe_render_dim(col, ss->SS_COLS); j++)
    {
        col_encoder(j + 1, col_header_buff);
        set_out_buff(obuff, col_header_buff);
        printf("%*s%s", MIN_COL_WIDTH, obuff, SPACER);
    }

    // Printing the Rows

    for (int i = row; i < safe_render_dim(row, ss->SS_ROWS); i++)
    {
        printf("\n");

        sprintf(col_data_buff, "%d", i + 1);
        printf("%s", SPACER);
        set_out_buff(obuff, col_data_buff);
        printf("%*s%s", MIN_COL_WIDTH, obuff, SPACER);

        // printf("Hi printing row [%d]",i);
        for (int j = col; j < safe_render_dim(col, ss->SS_COLS); j++)
        {
            if ((ss->arr[i * (ss->SS_COLS) + j]).visited_err_flag == '1' || (ss->arr[i * (ss->SS_COLS) + j]).visited_err_flag == '3')
            {
                sprintf(col_data_buff, "ERR");
            }
            else
            {
                sprintf(col_data_buff, "%d", ((ss->arr[i * (ss->SS_COLS) + j]).value));
            }
            // sprintf(col_data_buff, "%d", ((ss->arr[i * (ss->SS_COLS) + j]).cell)->value);
            // printf("Value at [%d][%d] = %.2f\n",i,j,(((ss->arr[i][j]).cell).value));
            set_out_buff(obuff, col_data_buff);
            printf("%*s%s", MIN_COL_WIDTH, obuff, SPACER);
        }
    }

    printf("\n");
}

void set_error_message(char exit_code, char error_buff[])
{
    // printf("Exit Code: %d\n", exit_code);
    switch (exit_code)
    {
    case '0':
        strcpy(error_buff, "ok");
        break;
    case '1':
        strcpy(error_buff, "Invalid Input");
        break;
    case '2':
        strcpy(error_buff, "Out of Range");
        break;
    case '3':
        strcpy(error_buff, "Malloc Failed");
        break;
    case '4':
        strcpy(error_buff, "Unknown Error");
        break;
    case '5':
        strcpy(error_buff, "Cycle Found");
        break;
    case '6':
        strcpy(error_buff, "Division by Zero");
        break;
    default:
        strcpy(error_buff, "Unknown Error");
        break;
    }
}

void terminal_control_unit(Spread_Sheet *ss)
{
    int row_render = 0, col_render = 0;
    int tcell_row, tcell_col;

    char exit_code = '0';
    bool en_ss_render = true;
    
    // We'll display how long the last command took, excluding user typing time
    double command_time = 0.0;

    // For wall-clock timing using CLOCK_MONOTONIC
    struct timespec start_ts, end_ts;

    char command_buff[100], target_cell_buff[10], exp_buff[100], exit_message_buff[100];
    set_error_message(exit_code, exit_message_buff);

    while (1)
    {
        // Render spreadsheet if output is enabled
        if (en_ss_render)
        {
            render_ss(ss, row_render, col_render);
        }

        // Print the prompt using the last command’s processing time
        printf("[%.1f] (%s) > ", command_time, exit_message_buff);

        if (fgets(command_buff, sizeof(command_buff), stdin) != NULL)
        {
            // Strip trailing newline if present
            command_buff[strcspn(command_buff, "\n")] = '\0';

            // Start timing immediately after input is read
            clock_gettime(CLOCK_MONOTONIC, &start_ts);

            if (strcmp(command_buff, "q") == 0)
            {
                // printf("Quitting the Spreadsheet Program\n");
                exit_code = '0';
                clock_gettime(CLOCK_MONOTONIC, &end_ts);
                command_time = (end_ts.tv_sec - start_ts.tv_sec) +
                               (end_ts.tv_nsec - start_ts.tv_nsec) / 1e9;
                break;
            }
            else if (strcmp(command_buff, "w") == 0)
            {
                row_render = next_render_dim(row_render, ss->SS_ROWS, -MAX_RENDER_DIM);
                exit_code = '0';
            }
            else if (strcmp(command_buff, "s") == 0)
            {
                row_render = next_render_dim(row_render, ss->SS_ROWS, MAX_RENDER_DIM);
                exit_code = '0';
            }
            else if (strcmp(command_buff, "a") == 0)
            {
                col_render = next_render_dim(col_render, ss->SS_COLS, -MAX_RENDER_DIM);
                exit_code = '0';
            }
            else if (strcmp(command_buff, "d") == 0)
            {
                col_render = next_render_dim(col_render, ss->SS_COLS, MAX_RENDER_DIM);
                exit_code = '0';
            }
            else if (strcmp(command_buff, "disable_output") == 0)
            {
                if (en_ss_render)
                {
                    en_ss_render = false;
                }

                exit_code = '0';

            }
            else if (strcmp(command_buff, "enable_output") == 0)
            {
                if (!en_ss_render)
                {
                    en_ss_render = true;
                }
                exit_code = '0';
            }
            else if (strncmp(command_buff, "scroll_to ", 10) == 0)
            {
                sscanf(command_buff, "scroll_to %s", target_cell_buff);
                bool flag_valid = is_valid_cell(target_cell_buff, ss->SS_ROWS, ss->SS_COLS, &tcell_row, &tcell_col);
                parse_cell_name(target_cell_buff, &tcell_row, &tcell_col);

                if (flag_valid)
                {
                    row_render = tcell_row;
                    col_render = tcell_col;
                    exit_code = '0';
                }
                else
                {
                    exit_code = '2';
                }
            }

            // Stop timing immediately after processing finishes
            clock_gettime(CLOCK_MONOTONIC, &end_ts);
            command_time = (end_ts.tv_sec - start_ts.tv_sec) +
                           (end_ts.tv_nsec - start_ts.tv_nsec) / 1e9;
        }
        else
        {
            exit_code = '7';
        }

        set_error_message(exit_code, exit_message_buff);
    }

    return;
}

// ------------------------------------------------------------------------- //


bool is_valid_cell(char data_buff[], int SS_ROWS, int SS_COLS, int *tcell_row, int *tcell_col)
{
    int col = 0, row = 0, i = 0;
    // printf("%d %d\n", SS_ROWS, SS_COLS);
    // Check for empty input
    if (data_buff == NULL || strlen(data_buff) == 0)
    {
        // *exit_code = INVALID_INPUT;
        return false;
    }

    // Extract column letters (only uppercase A-Z allowed)
    while (isalpha(data_buff[i]) && i < 3)
    {
        if (islower(data_buff[i]))
        {
            // *exit_code = INVALID_INPUT; // Reject lowercase letters
            return false;
        }
        col = col * 26 + (data_buff[i] - 'A' + 1);
        i++;
    }

    // Ensure at least one column letter was processed
    if (col == 0)
    {
        // *exit_code = INVALID_INPUT;
        return false;
    }

    // Extract row number (must be at least 1 and at most 999)
    if (!isdigit(data_buff[i]))
    {
        // *exit_code = INVALID_INPUT;
        return false; // No row number found after column letters
    }

    row = atoi(&data_buff[i]); // Convert remaining part to integer

    // Validate row and column limits
    // printf("%d %d\n", row, col);
    if (row > SS_ROWS || col > SS_COLS || row < 1 || col < 1)
    {
        // printf("yO\n");
        // *exit_code = INVALID_INPUT;
        return false;
    }

    // Convert to 0-based indexing
    *tcell_row = row - 1;
    *tcell_col = col - 1;
    // *exit_code = TCU_OK;
    return true;
}

void parse_cell_name(const char *cell_str, int *row, int *col)
{
    regex_t regex;
    regmatch_t matches[3]; // Group 1: letters, Group 2: digits
    if (regcomp(&regex, "^([A-Z]+)([0-9]+)$", REG_EXTENDED) != 0)
    {
        return;
    }
    if (regexec(&regex, cell_str, 3, matches, 0) == 0)
    {
        // Extract the column part (letters)
        int len = matches[1].rm_eo - matches[1].rm_so;
        char col_part[16] = {0};
        strncpy(col_part, cell_str + matches[1].rm_so, len);
        col_part[len] = '\0';
        int computed_col = 0;
        for (int i = 0; col_part[i] != '\0'; i++)
        {
            computed_col = computed_col * 26 + (col_part[i] - 'A' + 1);
        }
        // For 0-based indexing, subtract 1 from the computed column.
        *col = computed_col - 1;

        // Extract the row part (digits)
        len = matches[2].rm_eo - matches[2].rm_so;
        char row_part[16] = {0};
        strncpy(row_part, cell_str + matches[2].rm_so, len);
        row_part[len] = '\0';
        // Convert the row number and subtract 1 for 0-based indexing.
        *row = atoi(row_part) - 1;
    }
    regfree(&regex);
}