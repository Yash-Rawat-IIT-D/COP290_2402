#include "../header/parser_ss.h"
#include <stdbool.h>

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

void is_cell(char data_buff[], int SS_ROWS, int SS_COLS, int *tcell_row, int *tcell_col, TCU_EXIT_CODE *exit_code)
{
    int len_dbuff = strlen(data_buff), i = 0;
    if ((len_dbuff < 2) || (len_dbuff > 6))
    {
        *(exit_code) = INVALID_INPUT;
        // printf("Error : Not a valid Cell Input\n");
        return;
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
                *(exit_code) = INVALID_INPUT;
                // printf("Error: Invalid Cell Input\n");
                return;
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
            *(exit_code) = INVALID_INPUT;
            // printf("Error: Invalid Cell Input\n");
            return;
        }
        i++;
    }

    col_buff[cbuff_p] = '\0';
    row_buff[rbuff_p] = '\0';

    // printf("Row Buff=%s, Col Buff=%s\n",row_buff,col_buff);

    if (cbuff_p == 0 || rbuff_p == 0)
    {
        *(exit_code) = INVALID_INPUT;
        // printf("Error: Invalid Cell Input\n");
        return;
    }

    int col_num = col_decoder(col_buff);
    int row_num = atoi(row_buff);

    if (col_num < 1 || col_num > SS_COLS || row_num < 1 || row_num > SS_ROWS)
    {
        // printf("Error: Cell Out of Range\n");
        *(exit_code) = OUT_OF_RANGE;
        return;
    }

    *tcell_row = row_num - 1;
    *tcell_col = col_num - 1;
    *(exit_code) = TCU_OK;

    return;
}


bool is_valid_cell(char data_buff[], int SS_ROWS, int SS_COLS, int *tcell_row, int *tcell_col, TCU_EXIT_CODE *exit_code) {
    int col = 0, row = 0, i = 0;

    // Check for empty input
    if (data_buff == NULL || strlen(data_buff) == 0) {
        *exit_code = INVALID_INPUT;
        return false;
    }

    // Extract column letters (only uppercase A-Z allowed)
    while (isalpha(data_buff[i]) && i < 3) {
        if (islower(data_buff[i])) {
            *exit_code = INVALID_INPUT; // Reject lowercase letters
            return false;
        }
        col = col * 26 + (data_buff[i] - 'A' + 1);
        i++;
    }

    // Ensure at least one column letter was processed
    if (col == 0) {
        *exit_code = INVALID_INPUT;
        return false;
    }

    // Extract row number (must be at least 1 and at most 999)
    if (!isdigit(data_buff[i])) {
        *exit_code = INVALID_INPUT;
        return false; // No row number found after column letters
    }

    row = atoi(&data_buff[i]); // Convert remaining part to integer

    // Validate row and column limits
    if (row > SS_ROWS || col > SS_COLS || row < 1 || col < 1) {
        *exit_code = INVALID_INPUT;
        return false;
    }

    // Convert to 0-based indexing
    *tcell_row = row - 1;
    *tcell_col = col - 1;
    *exit_code = TCU_OK;
    return true;
}


bool is_range(char data_buff[], int SS_ROWS, int SS_COLS, TCU_EXIT_CODE *exit_code) {
    char cell1[10], cell2[10];
    int row1, col1, row2, col2;
    int i = 0, j = 0;

    // Check for empty input
    if (data_buff == NULL || strlen(data_buff) == 0) {
        *exit_code = INVALID_INPUT;
        return false;
    }

    // Split range into two cells (e.g., "A1:D10" -> "A1", "D10")
    while (data_buff[i] != ':' && data_buff[i] != '\0') {
        cell1[i] = data_buff[i];
        i++;
    }
    cell1[i] = '\0';  // Null-terminate first cell

    if (data_buff[i] != ':') {
        *exit_code = INVALID_INPUT;
        return false; // No ':' found, not a valid range
    }

    i++; // Move past ':'
    while (data_buff[i] != '\0') {
        cell2[j++] = data_buff[i++];
    }
    cell2[j] = '\0'; // Null-terminate second cell

    // Validate both cells
    int dummy_row, dummy_col; 
    if (!is_valid_cell(cell1, SS_ROWS, SS_COLS, &dummy_row, &dummy_col, exit_code) || !is_valid_cell(cell2, SS_ROWS, SS_COLS, &dummy_row, &dummy_col, exit_code)) {
        *exit_code = INVALID_INPUT;
        return false; // One of the cells is invalid
    }

    // Extract row and column values for both cells
    col1 = 0, col2 = 0;
    i = 0;
    while (isalpha(cell1[i])) {
        if (islower(cell1[i])) {
            *exit_code = INVALID_INPUT; // Reject lowercase letters
            return false;
        }
        col1 = col1 * 26 + (cell1[i] - 'A' + 1);
        i++;
    }
    row1 = atoi(&cell1[i]); // Convert remaining part to row number

    i = 0;
    while (isalpha(cell2[i])) {
        if (islower(cell2[i])) {
            *exit_code = INVALID_INPUT; // Reject lowercase letters
            return false;
        }
        col2 = col2 * 26 + (cell2[i] - 'A' + 1);
        i++;
    }
    row2 = atoi(&cell2[i]); // Convert remaining part to row number

    // Ensure valid range (top-left to bottom-right)
    if (row1 > row2 || col1 > col2) {
        *exit_code = INVALID_INPUT;
        return false;
    }

    return true; // Range is valid
}


// ------------------------------------------------------------------------- //


// Function to check if a function call is valid (e.g., SUM(A1:A10))
bool is_function(char data_buff[], int SS_ROWS, int SS_COLS, TCU_EXIT_CODE *exit_code) {
    char func_name[10], func_arg[20];
    int i = 0, j = 0;

    // Extract function name
    while (isalpha(data_buff[i])) {
        func_name[j++] = data_buff[i++];
    }
    func_name[j] = '\0'; // Null-terminate function name

    // Ensure '(' follows function name
    if (data_buff[i] != '(') {
        *exit_code = INVALID_INPUT;
        return false;
    }
    i++; // Move past '('

    // Extract function argument
    j = 0;
    while (data_buff[i] != ')' && data_buff[i] != '\0') {
        func_arg[j++] = data_buff[i++];
    }
    func_arg[j] = '\0'; // Null-terminate argument

    // Ensure ')' is present
    if (data_buff[i] != ')') {
        *exit_code = INVALID_INPUT;
        return false;
    }

    // Validate function name
    if (strcmp(func_name, "SUM") == 0 || strcmp(func_name, "MIN") == 0 ||
        strcmp(func_name, "MAX") == 0 || strcmp(func_name, "AVG") == 0 ||
        strcmp(func_name, "STDEV") == 0) {
        return is_range(func_arg, SS_ROWS, SS_COLS, exit_code);
    } else if (strcmp(func_name, "SLEEP") == 0) {
        int dummy_row, dummy_col;
        return is_valid_cell(func_arg, SS_ROWS, SS_COLS, &dummy_row, &dummy_col, exit_code) || atoi(func_arg) > 0;
    }

    *exit_code = INVALID_INPUT;
    return false;
}

void trim_whitespace(char *str) {
    int start = 0, end = strlen(str) - 1;

    // Trim leading spaces
    while (isspace((unsigned char)str[start])) {
        start++;
    }

    // Trim trailing spaces
    while (end >= start && isspace((unsigned char)str[end])) {
        end--;
    }

    // Shift the trimmed string
    if (start > 0 || end < (int)strlen(str) - 1) {
        memmove(str, str + start, end - start + 1);
    }

    // Null-terminate the trimmed string
    str[end - start + 1] = '\0';
}

void parse_command(char command_buff[], char target_cell_buff[], char exp_buff[], Spread_Sheet *ss, TCU_EXIT_CODE *exit_code) {
    regex_t regex;
    regmatch_t matches[3];
    int reti;

    // Compile regex
    reti = regcomp(&regex, REGEX_PATTERN, REG_EXTENDED);
    if (reti) {
        printf("Could not compile regex\n");
        *exit_code = UNKNOWN_ERROR;
        return;
    }

    // Execute regex match
    reti = regexec(&regex, command_buff, 3, matches, 0);
    if (!reti) {
        // Extract target cell
        int start = matches[1].rm_so;
        int end = matches[1].rm_eo;
        strncpy(target_cell_buff, command_buff + start, end - start);
        target_cell_buff[end - start] = '\0';

        // Extract expression
        start = matches[2].rm_so;
        end = matches[2].rm_eo;
        strncpy(exp_buff, command_buff + start, end - start);
        exp_buff[end - start] = '\0';

        // Trim whitespace from expression
        trim_whitespace(exp_buff);

        // Reject empty expressions after trimming
        if (strlen(exp_buff) == 0) {
            printf("Invalid: Expression is empty\n");
            *exit_code = INVALID_INPUT;
            return;
        }

        // Validate the target cell
        int tcell_row, tcell_col;
        if (!is_valid_cell(target_cell_buff, ss->SS_ROWS, ss->SS_COLS, &tcell_row, &tcell_col, exit_code)) {
            printf("Invalid cell reference: %s\n", target_cell_buff);
            return;
        }

        // Validate expression (cells, functions, or ranges)
        if (strchr(exp_buff, '(') != NULL) {
            if (!is_function(exp_buff, ss->SS_ROWS, ss->SS_COLS, exit_code)) {
                printf("Invalid function expression: %s\n", exp_buff);
                return;
            }
        } else {
            // Tokenize and check individual terms
            char exp_copy[100];
            strcpy(exp_copy, exp_buff);
            char *token = strtok(exp_copy, " +-*/(),");

            while (token != NULL) {
                if (strchr(token, ':')) {
                    // Validate range
                    if (!is_range(token, ss->SS_ROWS, ss->SS_COLS, exit_code)) {
                        printf("Invalid range reference: %s\n", token);
                        return;
                    }
                } else if (isalpha(token[0])) {
                    // Validate cell
                    int tcell_row, tcell_col;
                    if (!is_valid_cell(token, ss->SS_ROWS, ss->SS_COLS, &tcell_row, &tcell_col, exit_code)) {
                        printf("Invalid cell reference: %s\n", token);
                        return;
                    }
                }
                token = strtok(NULL, " +-*/(),");
            }
        }

        printf("Valid formula: Target Cell = %s, Expression = %s\n", target_cell_buff, exp_buff);
        *exit_code = TCU_OK;
    } else {
        *exit_code = INVALID_INPUT;
    }

    regfree(&regex);
}

// Assumes a valid instruction has been provided by the user
void parse_expression(char target_cell_buff[],char exp_buff[], Spread_Sheet *ss, TCU_EXIT_CODE *exit_code)
{
    
}

// ------------------------------------------------------------------------- //

// Given the current row and column as well as their maximum values,
// returns the upper bound of the respective dimension for rendering

int safe_render_dim(int rc, int rc_max)
{
    if (rc + MAX_RENDER_DIM > rc_max)
    {
        return rc_max;
    }
    else
    {
        return rc + MAX_RENDER_DIM;
    }
}

// ------------------------------------------------------------------------- //

// Need to take a look at this function's error handling
// Will implement generic error handling later I guess
// Right now, just loops back to the 0th index

int next_render_dim(int rc, int rc_max, int step_size, TCU_EXIT_CODE *exit_code)
{
    if (rc + step_size < 0 || rc + step_size >= rc_max)
    {
        *(exit_code) = OUT_OF_RANGE;
        return rc;
    }
    else
    {
        *(exit_code) = TCU_OK;
        return rc + step_size;
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

    printf("%s", SPACER);
    printf("%*s%s", MIN_COL_WIDTH, "==============", SPACER);

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
            sprintf(col_data_buff, "%d", ((ss->arr[i*(ss->SS_COLS)+j]).cell)->value);
            // printf("Value at [%d][%d] = %.2f\n",i,j,(((ss->arr[i][j]).cell).value));
            set_out_buff(obuff, col_data_buff);
            printf("%*s%s", MIN_COL_WIDTH, obuff, SPACER);
        }
    }

    printf("\n");
}

// ------------------------------------------------------------------------- //

// Sets the error message based on the error code, to be displayed in the terminal
// On next render based on the error code set by reference

void set_error_message(TCU_EXIT_CODE exit_code, char error_buff[])
{
    switch (exit_code)
    {
    case TCU_OK:
        strcpy(error_buff, "ok");
        break;
    case INVALID_INPUT:
        strcpy(error_buff, "Invalid Input");
        break;
    case OUT_OF_RANGE:
        strcpy(error_buff, "Out of Range");
        break;
    case MALLOC_FAILED:
        strcpy(error_buff, "Malloc Failed");
        break;
    case UNKNOWN_ERROR:
        strcpy(error_buff, "Unknown Error");
        break;
    default:
        printf("Unrecognized error code");
        break;
    }
}

// ------------------------------------------------------------------------- //

// The terminal control unit for the Spread_Sheet
// Takes in the Spread_Sheet and allows the user to interact with it




void terminal_control_unit(Spread_Sheet *ss)
{
    // ss->arr[i*ss->SS_COLS +j].cell_formula

    int row_render = 0, col_render = 0;
    TCU_EXIT_CODE exit_code = TCU_OK;
    SIM_BOOL en_ss_render = TRUE;
    int tcell_row, tcell_col;

    char command_buff[100], target_cell_buff[10], exp_buff[100], exit_message_buff[100];
    
    set_error_message(exit_code, exit_message_buff);

    while (1)
    {
        if (en_ss_render)
        {
            render_ss(ss, row_render, col_render);
        }

        printf("[0.0] (%s) > ", exit_message_buff);


        if (fgets(command_buff, sizeof(command_buff), stdin) != NULL)
        {
            if (strcmp(command_buff, "q\n") == 0)
            {
                printf("Quitting the Spreadsheet Program\n");
                exit_code = TCU_OK;
                break;
            }
            else if (strcmp(command_buff, "w\n") == 0)
            {
                row_render = next_render_dim(row_render, ss->SS_ROWS, -MAX_RENDER_DIM, &exit_code);
                
            }
            else if (strcmp(command_buff, "s\n") == 0)
            {
                row_render = next_render_dim(row_render, ss->SS_ROWS, MAX_RENDER_DIM, &exit_code);
                
            }
            else if (strcmp(command_buff, "a\n") == 0)
            {
                col_render = next_render_dim(col_render, ss->SS_COLS, -MAX_RENDER_DIM, &exit_code);
                
            }
            else if (strcmp(command_buff, "d\n") == 0)
            {
                col_render = next_render_dim(col_render, ss->SS_COLS, MAX_RENDER_DIM, &exit_code);
            }
            else if(strcmp(command_buff, "disable_output\n") == 0)
            {
                if(en_ss_render)
                {
                    en_ss_render = FALSE;
                }
                exit_code = TCU_OK;
            }
            else if(strcmp(command_buff, "enable_output\n") == 0)
            {
                if(!en_ss_render)
                {
                    en_ss_render = TRUE;
                }
                exit_code = TCU_OK;
            }
            else if(strncmp(command_buff, "scroll_to ",10) == 0)
            {

                sscanf(command_buff, "scroll_to %6s", &target_cell_buff);

                is_valid_cell(target_cell_buff, ss->SS_ROWS, ss->SS_COLS, &tcell_row, &tcell_col, &exit_code);

                if(exit_code == TCU_OK)
                {
                    row_render = tcell_row;
                    col_render = tcell_col;
                }
            }
            else
            {
                // printf("You entered %s", command_buff);
                parse_command(command_buff, target_cell_buff, exp_buff, ss, &exit_code);
                if(exit_code == TCU_OK)
                {
                    printf("Target Cell: %s, Eval Expression: %s\n", target_cell_buff, exp_buff);
                    // parse_expression();
                }
            }
        }
        else
        {
            exit_code = UNKNOWN_ERROR;
        }

        set_error_message(exit_code, exit_message_buff);
    }

    return;
}
// ------------------------------------------------------------------------- //
