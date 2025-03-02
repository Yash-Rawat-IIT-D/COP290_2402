#include "../header/parser.h"
// #include "../header/spreadsheet.h"
#include <stdbool.h>
#define _POSIX_C_SOURCE 200809L
#include <time.h>
#include <math.h>
#include <regex.h>

// ------------------------------------------------------------------------- //
// Rendering & Utility Functions


// SCell *get_scell_by_coordinates(Spread_Sheet *ss, int row, int col)
// {
//     if (row < 0 || row >= ss->SS_ROWS || col < 0 || col >= ss->SS_COLS)
//     {
//         printf("Error: Out of Bounds\n");
//         return NULL;
//     }

//     return &(ss->arr[row * ss->SS_COLS + col]);
// }

// void debug_print_scell(Spread_Sheet *ss, int row, int col)
// {
//     SCell *scell = &(ss->arr[row * ss->SS_COLS + col]);
//     printf("%s\n", LONG_SPACER);

//     printf("Cell Location = (%d, %d)\n", row, col);
//     printf("Cell Value = %d\n", scell->value);
//     // printf("Visited Error Flag: %c\n", scell->visited_err_flag);
//     // printf("Dependent Cells List: \n");
//     for (int i = 0; i < scell->dependent_scells_size; i++)
//     {
//         int tempv = ss->arr[scell->dependent_scells[i].x * ss->SS_COLS + scell->dependent_scells[i].y].value;
//         printf("Dependent Cell %d : (%d, %d) , Value : %d\n", i, scell->dependent_scells[i].x, scell->dependent_scells[i].y, tempv);
//     }

//     // debug_print_cell_formula(ss, p);
//     printf("%s\n", LONG_SPACER);

//     return;
// }






void col_encoder(int col_num, char *col_header_buff) {
    if (col_num < 1 || col_num > MAX_SS_COLS) {
        exit(1);
    }
    int ind = 0;
    char col_hbuff_temp[COL_HEADER_BUFF_SIZE];
    while (col_num > 0) {
        col_num -= 1;
        col_hbuff_temp[ind++] = (col_num % 26) + 'A';
        col_num /= 26;
    }
    for (int i = 0; i < ind; i++) {
        col_header_buff[i] = col_hbuff_temp[ind - i - 1];
    }
    col_header_buff[ind] = '\0';
}

int col_decoder(char *col_data_buff) {
    int col_dbuff_size = strlen(col_data_buff);
    if (col_dbuff_size < 1 || col_dbuff_size > 4) {
        exit(1);
    }
    int res_col_num = 0;
    for (int i = 0; i < col_dbuff_size; i++) {
        res_col_num = res_col_num * 26 + (col_data_buff[i] - 'A' + 1);
    }
    return res_col_num;
}

void set_out_buff(char *obuff, char *col_data_buff) {
    int col_dbuff_size = strlen(col_data_buff);
    int left_padding = (MIN_COL_WIDTH - col_dbuff_size) / 2;
    int right_padding = MIN_COL_WIDTH - col_dbuff_size - left_padding;
    for (int i = 0; i < left_padding; i++) {
        obuff[i] = ' ';
    }
    for (int i = 0; i < col_dbuff_size; i++) {
        obuff[left_padding + i] = col_data_buff[i];
    }
    for (int i = 0; i < right_padding; i++) {
        obuff[left_padding + col_dbuff_size + i] = COL_SPACER;
    }
}

int safe_render_dim(int rc, int rc_max) {
    return (rc + MAX_RENDER_DIM > rc_max) ? rc_max : rc + MAX_RENDER_DIM;
}

int next_render_dim(int rc, int rc_max, int step_size) {
    int new_rc = rc + step_size;
    if (new_rc < 0) {
        return 0;
    }
    if (new_rc + MAX_RENDER_DIM > rc_max) {
        new_rc = rc_max - MAX_RENDER_DIM;
        if (new_rc < 0) new_rc = 0;
        return new_rc;
    }
    return new_rc;
}

void render_ss(Spread_Sheet *ss, int row, int col) {
    char obuff[MIN_COL_WIDTH];
    char col_header_buff[COL_HEADER_BUFF_SIZE];
    char col_data_buff[COL_DATA_BUFF_SIZE];
    // Print Column Headers
    printf("%s", SPACER);
    printf("%*s%s", MIN_COL_WIDTH, SPACER_00, SPACER);
    for (int j = col; j < safe_render_dim(col, ss->SS_COLS); j++) {
        col_encoder(j + 1, col_header_buff);
        set_out_buff(obuff, col_header_buff);
        printf("%*s%s", MIN_COL_WIDTH, obuff, SPACER);
    }
    // Print Rows
    for (int i = row; i < safe_render_dim(row, ss->SS_ROWS); i++) {
        printf("\n");
        sprintf(col_data_buff, "%d", i + 1);
        printf("%s", SPACER);
        set_out_buff(obuff, col_data_buff);
        printf("%*s%s", MIN_COL_WIDTH, obuff, SPACER);
        for (int j = col; j < safe_render_dim(col, ss->SS_COLS); j++) {
            if ((ss->arr[i * ss->SS_COLS + j]).visited_err_flag == '1' ||
                (ss->arr[i * ss->SS_COLS + j]).visited_err_flag == '3')
            {
                sprintf(col_data_buff, "ERR");
            } else {
                sprintf(col_data_buff, "%d", ((ss->arr[i * ss->SS_COLS + j]).value));
            }
            set_out_buff(obuff, col_data_buff);
            printf("%*s%s", MIN_COL_WIDTH, obuff, SPACER);
        }
    }
    printf("\n");
}

void set_error_message(char exit_code, char error_buff[]) {
    switch (exit_code) {
        case '0': strcpy(error_buff, "ok"); break;
        case '1': strcpy(error_buff, "Invalid Input"); break;
        case '2': strcpy(error_buff, "Out of Range"); break;
        case '3': strcpy(error_buff, "Malloc Failed"); break;
        case '4': strcpy(error_buff, "Unknown Error"); break;
        case '5': strcpy(error_buff, "Cycle Found"); break;
        case '6': strcpy(error_buff, "Division by Zero"); break;
        default:  strcpy(error_buff, "Unknown Error"); break;
    }
}

// ------------------------------------------------------------------------- //
// Simple is_valid_cell function based on parse_cell_name

bool is_valid_cell(char data_buff[], int SS_ROWS, int SS_COLS, int *tcell_row, int *tcell_col, char *exit_code) {
    if (data_buff == NULL || strlen(data_buff) == 0)
        return false;
    parse_cell_name(data_buff, tcell_row, tcell_col);
    if (*tcell_row < 0 || *tcell_row >= SS_ROWS || *tcell_col < 0 || *tcell_col >= SS_COLS)
        return false;
    return true;
}

// ------------------------------------------------------------------------- //
// Parsing Functions

// parse_cell_name: Uses regex to split a cell string (e.g. "C1") into its row and column.
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



bool is_range(char data_buff[], int SS_ROWS, int SS_COLS, char *exit_code)
{
    char cell1[10], cell2[10];
    int row1, col1, row2, col2;
    int i = 0, j = 0;

    // Check for empty input
    if (data_buff == NULL || strlen(data_buff) == 0)
    {
        *exit_code = '1';
        return false;
    }

    // Split range into two cells (e.g., "A1:D10" -> "A1", "D10")
    while (data_buff[i] != ':' && data_buff[i] != '\0')
    {
        cell1[i] = data_buff[i];
        i++;
    }
    cell1[i] = '\0'; // Null-terminate first cell

    if (data_buff[i] != ':')
    {
        *exit_code = '1';
        return false; // No ':' found, not a valid range
    }

    i++; // Move past ':'
    while (data_buff[i] != '\0')
    {
        cell2[j++] = data_buff[i++];
    }
    cell2[j] = '\0'; // Null-terminate second cell

    // Validate both cells
    int dummy_row, dummy_col;
    if (!is_valid_cell(cell1, SS_ROWS, SS_COLS, &dummy_row, &dummy_col, exit_code) || !is_valid_cell(cell2, SS_ROWS, SS_COLS, &dummy_row, &dummy_col, exit_code))
    {
        *exit_code = '1';
        return false; // One of the cells is invalid
    }

    // Extract row and column values for both cells
    col1 = 0, col2 = 0;
    i = 0;
    while (isalpha(cell1[i]))
    {
        if (islower(cell1[i]))
        {
            *exit_code = '1'; // Reject lowercase letters
            return false;
        }
        col1 = col1 * 26 + (cell1[i] - 'A' + 1);
        i++;
    }
    row1 = atoi(&cell1[i]); // Convert remaining part to row number

    i = 0;
    while (isalpha(cell2[i]))
    {
        if (islower(cell2[i]))
        {
            *exit_code = '1'; // Reject lowercase letters
            return false;
        }
        col2 = col2 * 26 + (cell2[i] - 'A' + 1);
        i++;
    }
    row2 = atoi(&cell2[i]); // Convert remaining part to row number

    // Ensure valid range (top-left to bottom-right)
    if (row1 > row2 || col1 > col2)
    {
        *exit_code = '1';
        return false;
    }

    return true; // Range is valid
}

// ------------------------------------------------------------------------- //

// Function to check if a function call is valid (e.g., SUM(A1:A10))
bool is_function(char data_buff[], int SS_ROWS, int SS_COLS, char *exit_code)
{
    char func_name[10], func_arg[20];
    int i = 0, j = 0;

    // Extract function name
    while (isalpha(data_buff[i]))
    {
        func_name[j++] = data_buff[i++];
    }
    func_name[j] = '\0'; // Null-terminate function name

    // Ensure '(' follows function name
    if (data_buff[i] != '(')
    {
        *exit_code = '1';
        return false;
    }
    i++; // Move past '('

    // Extract function argument
    j = 0;
    while (data_buff[i] != ')' && data_buff[i] != '\0')
    {
        func_arg[j++] = data_buff[i++];
    }
    func_arg[j] = '\0'; // Null-terminate argument

    // Ensure ')' is present
    if (data_buff[i] != ')')
    {
        *exit_code = '1';
        return false;
    }

    // Validate function name
    if (strcmp(func_name, "SUM") == 0 || strcmp(func_name, "MIN") == 0 ||
        strcmp(func_name, "MAX") == 0 || strcmp(func_name, "AVG") == 0 ||
        strcmp(func_name, "STDEV") == 0)
    {
        return is_range(func_arg, SS_ROWS, SS_COLS, exit_code);
    }
    else if (strcmp(func_name, "SLEEP") == 0)
    {
        int dummy_row, dummy_col;
        return is_valid_cell(func_arg, SS_ROWS, SS_COLS, &dummy_row, &dummy_col, exit_code) || atoi(func_arg) > 0;
    }

    *exit_code = '1';
    return false;
}

void trim_whitespace(char *str)
{
    int start = 0, end = strlen(str) - 1;

    // Trim leading spaces
    while (isspace((unsigned char)str[start]))
    {
        start++;
    }

    // Trim trailing spaces
    while (end >= start && isspace((unsigned char)str[end]))
    {
        end--;
    }

    // Shift the trimmed string
    if (start > 0 || end < (int)strlen(str) - 1)
    {
        memmove(str, str + start, end - start + 1);
    }

    // Null-terminate the trimmed string
    str[end - start + 1] = '\0';
}

void parse_command(char command_buff[], char target_cell_buff[], char exp_buff[], Spread_Sheet *ss, char *exit_code)
{
    regex_t regex;
    regmatch_t matches[3];
    int reti;

    // Compile regex
    reti = regcomp(&regex, REGEX_PATTERN, REG_EXTENDED);
    if (!reti)
    {
        // printf("Regex compiled successfully\n");
    }
    else if (reti)
    {
        char errbuf[256];
        // size_t errlen = regerror(reti, &regex, errbuf, sizeof(errbuf));
        // fprintf(stderr, "Regex compile failed: %s\n", errbuf);

        // printf("Could not compile regex\n");
        *exit_code = '1';
        return;
    }

    // Execute regex match
    reti = regexec(&regex, command_buff, 3, matches, 0);
    if (reti == REG_NOMATCH)
    {
        // printf("No match\n");
        *exit_code = '1';
        return;
    }
    else if (!reti)
    {
        // Extract target cell
        int start = matches[1].rm_so;
        int end = matches[1].rm_eo;
        strncpy(target_cell_buff, command_buff + start, end - start);
        target_cell_buff[end - start] = '\0';
        // printf("Target Cell: %s\n", target_cell_buff);

        // Extract expression
        start = matches[2].rm_so;
        end = matches[2].rm_eo;
        strncpy(exp_buff, command_buff + start, end - start);
        exp_buff[end - start] = '\0';

        // Trim whitespace from expression
        trim_whitespace(exp_buff);

        // Reject empty expressions after trimming
        if (strlen(exp_buff) == 0)
        {
            // printf("Invalid: Expression is empty\n");
            *exit_code = '1';
            return;
        }

        // Validate the target cell
        int tcell_row, tcell_col;
        if (!is_valid_cell(target_cell_buff, ss->SS_ROWS, ss->SS_COLS, &tcell_row, &tcell_col, exit_code))
        {
            // printf("Invalid cell reference: %s\n", target_cell_buff);
            return;
        }

        // Validate expression (cells, functions, or ranges)
        if (strchr(exp_buff, '(') != NULL)
        {
            if (!is_function(exp_buff, ss->SS_ROWS, ss->SS_COLS, exit_code))
            {
                // printf("Invalid function expression: %s\n", exp_buff);
                return;
            }
        }
        else
        {
            // Tokenize and check individual terms
            char exp_copy[100];
            strcpy(exp_copy, exp_buff);
            char *token = strtok(exp_copy, " +-*/(),");

            while (token != NULL)
            {
                if (strchr(token, ':'))
                {
                    // Validate range
                    if (!is_range(token, ss->SS_ROWS, ss->SS_COLS, exit_code))
                    {
                        // printf("Invalid range reference: %s\n", token);
                        return;
                    }
                }
                else if (isalpha(token[0]))
                {
                    // Validate cell
                    int tcell_row, tcell_col;
                    if (!is_valid_cell(token, ss->SS_ROWS, ss->SS_COLS, &tcell_row, &tcell_col, exit_code))
                    {
                        // printf("Invalid cell reference: %s\n", token);
                        return;
                    }
                }
                token = strtok(NULL, " +-*/(),");
            }
        }

        // printf("Valid formula: Target Cell = %s, Expression = %s\n", target_cell_buff, exp_buff);
        *exit_code = '0';
    }
    else
    {
        *exit_code = '1';
    }

    regfree(&regex);
}














/*===========================================================================
  Parsing Functions with Exit Code Parameter
===========================================================================*/

// parse_operand_expr: Parses an operand (number or cell reference) and sets the CELL_FORMULA union.
void parse_operand_expr(const char *operand, CELL_FORMULA *cf, Spread_Sheet *ss, char *exit_code) {
    char *endptr;
    long num = strtol(operand, &endptr, 10);
    if (*endptr == '\0') {
        cf->valid_exp_type = '0';
        cf->fvcons.value = (int) num;
        *exit_code = '0';
    } else {
        int row, col;
        parse_cell_name(operand, &row, &col);
        cf->valid_exp_type = '1';
        cf->fvcell.cell_row = (short int) row;
        cf->fvcell.cell_col = (short int) col;
        *exit_code = '0';
    }
}

// parse_arithmetic_expr: Parses an arithmetic expression ("operand op operand") and fills the appropriate union.
void parse_arithmetic_expr(const char *exp, CELL_FORMULA *cf, Spread_Sheet *ss, char *exit_code) {
    const char *ops = "+-*/";
    const char *op_ptr = NULL;
    for (int i = 0; exp[i] != '\0'; i++) {
        if (i == 0 && (exp[i]=='+' || exp[i]=='-')) continue; // skip leading sign
        if (strchr(ops, exp[i]) != NULL) {
            op_ptr = exp + i;
            break;
        }
    }
    if (!op_ptr) {
        *exit_code = '1';
        return;
    }
    char op = *op_ptr;
    int op_index = op_ptr - exp;
    char left_str[50] = {0};
    char right_str[50] = {0};
    strncpy(left_str, exp, op_index);
    left_str[op_index] = '\0';
    strcpy(right_str, exp + op_index + 1);
    // Trim whitespace from operands
    // (Assume a trim_whitespace function is available)
    trim_whitespace(left_str);
    trim_whitespace(right_str);
    CELL_FORMULA left_cf, right_cf;
    parse_operand_expr(left_str, &left_cf, ss, exit_code);
    if (*exit_code != '0') return;
    parse_operand_expr(right_str, &right_cf, ss, exit_code);
    if (*exit_code != '0') return;
    if (left_cf.valid_exp_type == '0' && right_cf.valid_exp_type == '0') {
        cf->valid_exp_type = '2';
        cf->farith_cons_cons.arithmetic_op = op;
        cf->farith_cons_cons.left_value = left_cf.fvcons.value;
        cf->farith_cons_cons.right_value = right_cf.fvcons.value;
    } else if (left_cf.valid_exp_type == '0' && right_cf.valid_exp_type == '1') {
        cf->valid_exp_type = '3';
        cf->farith_cons_cell.arithmetic_op = op;
        cf->farith_cons_cell.left_value = left_cf.fvcons.value;
        cf->farith_cons_cell.right_cell_row = right_cf.fvcell.cell_row;
        cf->farith_cons_cell.right_cell_col = right_cf.fvcell.cell_col;
    } else if (left_cf.valid_exp_type == '1' && right_cf.valid_exp_type == '0') {
        cf->valid_exp_type = '4';
        cf->farith_cell_cons.arithmetic_op = op;
        cf->farith_cell_cons.left_cell_row = left_cf.fvcell.cell_row;
        cf->farith_cell_cons.left_cell_col = left_cf.fvcell.cell_col;
        cf->farith_cell_cons.right_value = right_cf.fvcons.value;
    } else if (left_cf.valid_exp_type == '1' && right_cf.valid_exp_type == '1') {
        cf->valid_exp_type = '5';
        cf->farith_cell_cell.arithmetic_op = op;
        cf->farith_cell_cell.left_cell_row = left_cf.fvcell.cell_row;
        cf->farith_cell_cell.left_cell_col = left_cf.fvcell.cell_col;
        cf->farith_cell_cell.right_cell_row = right_cf.fvcell.cell_row;
        cf->farith_cell_cell.right_cell_col = right_cf.fvcell.cell_col;
    }
    *exit_code = '0';
}

// parse_range_expr: Parses a range string like "A1:B2" and fills the FFUNCTION union.
void parse_range_expr(const char *range_str, CELL_FORMULA *cf, Spread_Sheet *ss, char *exit_code) {
    char start_cell[10] = {0};
    char end_cell[10] = {0};
    const char *colon = strchr(range_str, ':');
    if (!colon) {
        *exit_code = '1';
        return;
    }
    int len = colon - range_str;
    strncpy(start_cell, range_str, len);
    start_cell[len] = '\0';
    strcpy(end_cell, colon + 1);
    int start_row, start_col, end_row, end_col;
    parse_cell_name(start_cell, &start_row, &start_col);
    parse_cell_name(end_cell, &end_row, &end_col);
    if (start_row > end_row || start_col > end_col) {
        *exit_code = '1';
        return;
    }
    cf->valid_exp_type = '8';
    cf->ffunc.start_row = (short int) start_row;
    cf->ffunc.start_col = (short int) start_col;
    cf->ffunc.end_row = (short int) end_row;
    cf->ffunc.end_col = (short int) end_col;
    *exit_code = '0';
}

// parse_function_expr: Parses a function call expression.
// For SLEEP, if the argument is a constant, sets type '6'; if a cell reference, sets type '7'.
// For range functions (MIN, MAX, AVG, SUM, STDEV), sets type '8'.
void parse_function_expr(const char *exp, CELL_FORMULA *cf, Spread_Sheet *ss, char *exit_code) {
    const char *open_paren = strchr(exp, '(');
    const char *close_paren = strrchr(exp, ')');
    if (!open_paren || !close_paren) {
        *exit_code = '1';
        return;
    }
    int func_name_len = open_paren - exp;
    char func_name[16] = {0};
    strncpy(func_name, exp, func_name_len);
    func_name[func_name_len] = '\0';
    if (strcmp(func_name, "SLEEP") == 0) {
        // For SLEEP, set type '6' for constant, '7' for cell.
        char arg[50] = {0};
        int arg_len = close_paren - open_paren - 1;
        strncpy(arg, open_paren + 1, arg_len);
        arg[arg_len] = '\0';
        char *endptr;
        long num = strtol(arg, &endptr, 10);
        if (*endptr == '\0') {
            cf->valid_exp_type = '6';
            cf->fsleep_cons.sleep_time = (int) num;
        } else {
            cf->valid_exp_type = '7';
            int row, col;
            parse_cell_name(arg, &row, &col);
            cf->fsleep_cell.sleep_cell_row = (short int) row;
            cf->fsleep_cell.sleep_cell_col = (short int) col;
        }
    } else {
        // Otherwise, assume one of the range functions.
        cf->valid_exp_type = '8';
        cf->ffunc.function = func_name[0]; // e.g., 'M' for MIN, 'X' for MAX, 'A' for AVG, 'S' for SUM, 'T' for STDEV.
        char range_arg[50] = {0};
        int range_len = close_paren - open_paren - 1;
        strncpy(range_arg, open_paren + 1, range_len);
        range_arg[range_len] = '\0';
        parse_range_expr(range_arg, cf, ss, exit_code);
        if (*exit_code != '0') return;
    }
    *exit_code = '0';
}

// Evaluation function: evaluates the formula and returns an integer value.
int evaluate_formula(CELL_FORMULA *cf, Spread_Sheet *ss, bool sleep_override, char *exit_code) {
    switch (cf->valid_exp_type) {
        case '0': // constant
            *exit_code = '0';
            return cf->fvcons.value;
        case '1': { // cell reference
            SCell *ref = get_scell_by_coordinates(ss, cf->fvcell.cell_row, cf->fvcell.cell_col);
            *exit_code = '0';
            return (ref ? ref->value : 0);
        }
        case '2': { // constant op constant
            int l = cf->farith_cons_cons.left_value;
            int r = cf->farith_cons_cons.right_value;
            char op = cf->farith_cons_cons.arithmetic_op;
            *exit_code = '0';
            switch (op) {
                case '+': return l + r;
                case '-': return l - r;
                case '*': return l * r;
                case '/': return (r != 0) ? l / r : 0;
                default: return 0;
            }
        }
        case '3': { // constant op cell
            int l = cf->farith_cons_cell.left_value;
            SCell *r_cell = get_scell_by_coordinates(ss, cf->farith_cons_cell.right_cell_row, cf->farith_cons_cell.right_cell_col);
            int r = (r_cell ? r_cell->value : 0);
            char op = cf->farith_cons_cell.arithmetic_op;
            *exit_code = '0';
            switch (op) {
                case '+': return l + r;
                case '-': return l - r;
                case '*': return l * r;
                case '/': return (r != 0) ? l / r : 0;
                default: return 0;
            }
        }
        case '4': { // cell op constant
            SCell *l_cell = get_scell_by_coordinates(ss, cf->farith_cell_cons.left_cell_row, cf->farith_cell_cons.left_cell_col);
            int l = (l_cell ? l_cell->value : 0);
            int r = cf->farith_cell_cons.right_value;
            char op = cf->farith_cell_cons.arithmetic_op;
            *exit_code = '0';
            switch (op) {
                case '+': return l + r;
                case '-': return l - r;
                case '*': return l * r;
                case '/': return (r != 0) ? l / r : 0;
                default: return 0;
            }
        }
        case '5': { // cell op cell
            SCell *l_cell = get_scell_by_coordinates(ss, cf->farith_cell_cell.left_cell_row, cf->farith_cell_cell.left_cell_col);
            SCell *r_cell = get_scell_by_coordinates(ss, cf->farith_cell_cell.right_cell_row, cf->farith_cell_cell.right_cell_col);
            int l = (l_cell ? l_cell->value : 0);
            int r = (r_cell ? r_cell->value : 0);
            char op = cf->farith_cell_cell.arithmetic_op;
            *exit_code = '0';
            switch (op) {
                case '+': return l + r;
                case '-': return l - r;
                case '*': return l * r;
                case '/': return (r != 0) ? l / r : 0;
                default: return 0;
            }
        }
        case '6': { // SLEEP constant
            int t = cf->fsleep_cons.sleep_time;
            if (!sleep_override)
                sleep(t);
            *exit_code = '0';
            return t;
        }
        case '7': { // SLEEP cell
            SCell *ref = get_scell_by_coordinates(ss, cf->fsleep_cell.sleep_cell_row, cf->fsleep_cell.sleep_cell_col);
            int t = (ref ? ref->value : 0);
            if (!sleep_override)
                sleep(t);
            *exit_code = '0';
            return t;
        }
        case '8': { // Range function
            int start_row = cf->ffunc.start_row;
            int end_row = cf->ffunc.end_row;
            int start_col = cf->ffunc.start_col;
            int end_col = cf->ffunc.end_col;
            int count = 0, sum = 0, min = 0, max = 0, sum_sq = 0;
            bool first = true;
            for (int r = start_row; r <= end_row; r++) {
                for (int c = start_col; c <= end_col; c++) {
                    SCell *cell = get_scell_by_coordinates(ss, r, c);
                    if (cell) {
                        int v = cell->value;
                        if (first) {
                            min = max = v;
                            first = false;
                        } else {
                            if (v < min) min = v;
                            if (v > max) max = v;
                        }
                        sum += v;
                        sum_sq += v * v;
                        count++;
                    }
                }
            }
            char func = cf->ffunc.function;
            *exit_code = '0';
            if (func == 'M')      return min;              // MIN
            else if (func == 'X') return max;              // MAX (using 'X' for max)
            else if (func == 'S') return sum;              // SUM
            else if (func == 'A') return (count > 0) ? sum / count : 0; // AVG
            else if (func == 'T') { // STDEV: using the algorithm provided
                double mean = (count > 0) ? (double)sum / count : 0;
                double variance = (count > 0) ? ((double)sum_sq) / count - mean * mean : 0;
                // Clamp tiny negative variance due to floating point error.
                if (variance < 0 && fabs(variance) < 1e-12)
                    variance = 0;
                return (int)round(sqrt(variance));
            }
            return 0;
        }
        default:
            *exit_code = '4';
            return 0;
    }
}

// parse_expression: Parses the target cell and expression, builds a CELL_FORMULA,
// evaluates the formula, and updates the target cell’s value.
void parse_expression(char exp_buff[], char target_cell_buff[], Spread_Sheet *ss, char *exit_code) {
    int row, col;
    parse_cell_name(target_cell_buff, &row, &col);
    SCell *target = get_scell_by_coordinates(ss, row, col);
    if (!target) {
        *exit_code = '2';
        return;
    }
    CELL_FORMULA *new_formula = malloc(sizeof(CELL_FORMULA));
    if (!new_formula) {
        *exit_code = '3';
        return;
    }
    // Determine expression type:
    if (strncmp(exp_buff, "MIN(", 4) == 0 ||
        strncmp(exp_buff, "MAX(", 4) == 0 ||
        strncmp(exp_buff, "AVG(", 4) == 0 ||
        strncmp(exp_buff, "SUM(", 4) == 0 ||
        strncmp(exp_buff, "STDEV(", 6) == 0 ||
        strncmp(exp_buff, "SLEEP(", 6) == 0) {
        parse_function_expr(exp_buff, new_formula, ss, exit_code);
        if (*exit_code != '0') { free(new_formula); return; }
    } else if (strpbrk(exp_buff + 1, "+-*/") != NULL) {
        parse_arithmetic_expr(exp_buff, new_formula, ss, exit_code);
        if (*exit_code != '0') { free(new_formula); return; }
    } else {
        parse_operand_expr(exp_buff, new_formula, ss, exit_code);
        if (*exit_code != '0') { free(new_formula); return; }
    }
    

    char my_update = update_logic_unit(ss, make_pair(row,col), new_formula);

    // target->cell_formula = new_formula;
    // int new_val = evaluate_formula(new_formula, ss, false, exit_code);
    // target->value = new_val;
    // printf("parse_expression(): Set cell %s to value %d\n", target_cell_buff, new_val);
    
    *exit_code = my_update;
}


// ------------------------------------------------------------------------- //
// Terminal Control Unit

void terminal_control_unit(Spread_Sheet *ss) {
    int row_render = 0, col_render = 0;
    int tcell_row, tcell_col;
    char exit_code = '0';
    bool en_ss_render = true;
    double command_time = 0.0;
    struct timespec start_ts, end_ts;
    char command_buff[100], target_cell_buff[10], exp_buff[100], exit_message_buff[100];
    set_error_message(exit_code, exit_message_buff);
    while (1) {
        if (en_ss_render) {
            render_ss(ss, row_render, col_render);
        }
        printf("[%.1f] (%s) > ", command_time, exit_message_buff);
        if (fgets(command_buff, sizeof(command_buff), stdin) != NULL) {
            command_buff[strcspn(command_buff, "\n")] = '\0';
            clock_gettime(CLOCK_MONOTONIC, &start_ts);
            if (strcmp(command_buff, "q") == 0) {
                exit_code = '0';
                clock_gettime(CLOCK_MONOTONIC, &end_ts);
                command_time = (end_ts.tv_sec - start_ts.tv_sec) +
                               (end_ts.tv_nsec - start_ts.tv_nsec) / 1e9;
                break;
            } else if (strcmp(command_buff, "w") == 0) {
                row_render = next_render_dim(row_render, ss->SS_ROWS, -MAX_RENDER_DIM);
                exit_code = '0';
            } else if (strcmp(command_buff, "s") == 0) {
                row_render = next_render_dim(row_render, ss->SS_ROWS, MAX_RENDER_DIM);
                exit_code = '0';
            } else if (strcmp(command_buff, "a") == 0) {
                col_render = next_render_dim(col_render, ss->SS_COLS, -MAX_RENDER_DIM);
                exit_code = '0';
            } else if (strcmp(command_buff, "d") == 0) {
                col_render = next_render_dim(col_render, ss->SS_COLS, MAX_RENDER_DIM);
                exit_code = '0';
            } else if (strcmp(command_buff, "disable_output") == 0) {
                en_ss_render = false;
                exit_code = '0';
            } else if (strcmp(command_buff, "enable_output") == 0) {
                en_ss_render = true;
                exit_code = '0';
            } else if (strncmp(command_buff, "scroll_to ", 10) == 0) {
                sscanf(command_buff, "scroll_to %s", target_cell_buff);
                if (is_valid_cell(target_cell_buff, ss->SS_ROWS, ss->SS_COLS, &tcell_row, &tcell_col, &exit_code)) {
                    parse_cell_name(target_cell_buff, &tcell_row, &tcell_col);
                    row_render = tcell_row;
                    col_render = tcell_col;
                    exit_code = '0';
                } else {
                    exit_code = '2';
                }
            } else if (strncmp(command_buff, "dbg ", 4) == 0) {
                int rows = 0, cols = 0;
                parse_cell_name(command_buff + 4, &rows, &cols);
                SCell *sc = get_scell_by_coordinates(ss, rows, cols);
                debug_print_scell(ss, rows, cols);
            } else {
                // In this branch, call parse_expression to parse and update cell value.
                // (Assume that parse_command() already populated target_cell_buff and exp_buff.)
                parse_command(command_buff, target_cell_buff, exp_buff, ss, &exit_code);
                if(exit_code == '0')
                {
                    parse_expression(exp_buff, target_cell_buff, ss, &exit_code);
                }
            }
            clock_gettime(CLOCK_MONOTONIC, &end_ts);
            command_time = (end_ts.tv_sec - start_ts.tv_sec) +
                           (end_ts.tv_nsec - start_ts.tv_nsec) / 1e9;
        } else {
            exit_code = '4';
        }
        set_error_message(exit_code, exit_message_buff);
    }
    return;
}
