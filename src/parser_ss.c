#include "../header/parser_ss.h"
#include <stdbool.h>
#include <time.h>

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

bool is_valid_cell(char data_buff[], int SS_ROWS, int SS_COLS, int *tcell_row, int *tcell_col, TCU_EXIT_CODE *exit_code)
{
    int col = 0, row = 0, i = 0;

    // Check for empty input
    if (data_buff == NULL || strlen(data_buff) == 0)
    {
        *exit_code = INVALID_INPUT;
        return false;
    }

    // Extract column letters (only uppercase A-Z allowed)
    while (isalpha(data_buff[i]) && i < 3)
    {
        if (islower(data_buff[i]))
        {
            *exit_code = INVALID_INPUT; // Reject lowercase letters
            return false;
        }
        col = col * 26 + (data_buff[i] - 'A' + 1);
        i++;
    }

    // Ensure at least one column letter was processed
    if (col == 0)
    {
        *exit_code = INVALID_INPUT;
        return false;
    }

    // Extract row number (must be at least 1 and at most 999)
    if (!isdigit(data_buff[i]))
    {
        *exit_code = INVALID_INPUT;
        return false; // No row number found after column letters
    }

    row = atoi(&data_buff[i]); // Convert remaining part to integer

    // Validate row and column limits
    if (row > SS_ROWS || col > SS_COLS || row < 1 || col < 1)
    {
        *exit_code = INVALID_INPUT;
        return false;
    }

    // Convert to 0-based indexing
    *tcell_row = row - 1;
    *tcell_col = col - 1;
    *exit_code = TCU_OK;
    return true;
}

bool is_range(char data_buff[], int SS_ROWS, int SS_COLS, TCU_EXIT_CODE *exit_code)
{
    char cell1[10], cell2[10];
    int row1, col1, row2, col2;
    int i = 0, j = 0;

    // Check for empty input
    if (data_buff == NULL || strlen(data_buff) == 0)
    {
        *exit_code = INVALID_INPUT;
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
        *exit_code = INVALID_INPUT;
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
        *exit_code = INVALID_INPUT;
        return false; // One of the cells is invalid
    }

    // Extract row and column values for both cells
    col1 = 0, col2 = 0;
    i = 0;
    while (isalpha(cell1[i]))
    {
        if (islower(cell1[i]))
        {
            *exit_code = INVALID_INPUT; // Reject lowercase letters
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
            *exit_code = INVALID_INPUT; // Reject lowercase letters
            return false;
        }
        col2 = col2 * 26 + (cell2[i] - 'A' + 1);
        i++;
    }
    row2 = atoi(&cell2[i]); // Convert remaining part to row number

    // Ensure valid range (top-left to bottom-right)
    if (row1 > row2 || col1 > col2)
    {
        *exit_code = INVALID_INPUT;
        return false;
    }

    return true; // Range is valid
}

// ------------------------------------------------------------------------- //

// Function to check if a function call is valid (e.g., SUM(A1:A10))
bool is_function(char data_buff[], int SS_ROWS, int SS_COLS, TCU_EXIT_CODE *exit_code)
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
        *exit_code = INVALID_INPUT;
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
        *exit_code = INVALID_INPUT;
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

    *exit_code = INVALID_INPUT;
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

void parse_command(char command_buff[], char target_cell_buff[], char exp_buff[], Spread_Sheet *ss, TCU_EXIT_CODE *exit_code)
{
    regex_t regex;
    regmatch_t matches[3];
    int reti;

    // Compile regex
    reti = regcomp(&regex, REGEX_PATTERN, REG_EXTENDED);
    if (!reti)
    {
        printf("Regex compiled successfully\n");
    }
    else if (reti)
    {
        char errbuf[256];
        // size_t errlen = regerror(reti, &regex, errbuf, sizeof(errbuf));
        fprintf(stderr, "Regex compile failed: %s\n", errbuf);

        printf("Could not compile regex\n");
        *exit_code = UNKNOWN_ERROR;
        return;
    }

    // Execute regex match
    reti = regexec(&regex, command_buff, 3, matches, 0);
    if (reti == REG_NOMATCH)
    {
        printf("No match\n");
        *exit_code = INVALID_INPUT;
        return;
    }
    else if (!reti)
    {
        // Extract target cell
        int start = matches[1].rm_so;
        int end = matches[1].rm_eo;
        strncpy(target_cell_buff, command_buff + start, end - start);
        target_cell_buff[end - start] = '\0';
        printf("Target Cell: %s\n", target_cell_buff);

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
            printf("Invalid: Expression is empty\n");
            *exit_code = INVALID_INPUT;
            return;
        }

        // Validate the target cell
        int tcell_row, tcell_col;
        if (!is_valid_cell(target_cell_buff, ss->SS_ROWS, ss->SS_COLS, &tcell_row, &tcell_col, exit_code))
        {
            printf("Invalid cell reference: %s\n", target_cell_buff);
            return;
        }

        // Validate expression (cells, functions, or ranges)
        if (strchr(exp_buff, '(') != NULL)
        {
            if (!is_function(exp_buff, ss->SS_ROWS, ss->SS_COLS, exit_code))
            {
                printf("Invalid function expression: %s\n", exp_buff);
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
                        printf("Invalid range reference: %s\n", token);
                        return;
                    }
                }
                else if (isalpha(token[0]))
                {
                    // Validate cell
                    int tcell_row, tcell_col;
                    if (!is_valid_cell(token, ss->SS_ROWS, ss->SS_COLS, &tcell_row, &tcell_col, exit_code))
                    {
                        printf("Invalid cell reference: %s\n", token);
                        return;
                    }
                }
                token = strtok(NULL, " +-*/(),");
            }
        }

        printf("Valid formula: Target Cell = %s, Expression = %s\n", target_cell_buff, exp_buff);
        *exit_code = TCU_OK;
    }
    else
    {
        *exit_code = INVALID_INPUT;
    }

    regfree(&regex);
}

/* --- Helper: Convert a cell name (e.g. "C1") into row and column numbers --- */
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

/* --- Helper: Given an operand string, determine whether it is a constant (number)
         or a cell reference.
         Uses your in-place trim_whitespace() after duplicating the operand.
         If constant, stores its value in *value; if a cell reference, sets *cell accordingly.
*/
void parse_operand(const char *operand,
                   SIM_BOOL *is_constant,
                   int *value,
                   Cell **cell,
                   Spread_Sheet *ss,
                   TCU_EXIT_CODE *exit_code)
{
    char *temp = strdup(operand);
    int len = strlen(temp);
    if (!temp)
    {
        *exit_code = MALLOC_FAILED;
        return;
    }
    trim_whitespace(temp); // Your provided in-place trim_whitespace function
    int i = 0;
    if ((len >= 2) && (temp[0] == '+' || temp[0] == '-') && (isdigit((unsigned char)temp[1])))
        i = 1;

    int allDigits = 1;
    for (; temp[i] != '\0'; i++)
    {
        if (!isdigit((unsigned char)temp[i]))
        {
            allDigits = 0;
            break;
        }
    }
    if (allDigits)
    {
        *is_constant = TRUE;
        *value = atoi(temp);
        *cell = NULL;
    }
    else
    {
        *is_constant = FALSE;
        int row, col;
        parse_cell_name(temp, &row, &col);
        SCell *s = get_scell_by_coordinates(ss, row, col);
        if (!s)
        {
            printf("Operand %s is not a valid cell reference\n", temp);
            printf("Invalid cell reference in formula\n");
            *exit_code = OUT_OF_RANGE;
        }
        *cell = s ? s->cell : NULL;
    }
    free(temp);
}

/* --- Helper: Parse a simple value expression (constant or cell reference) --- */
void parse_value(const char *exp, Cell_Formula *formula, Spread_Sheet *ss, TCU_EXIT_CODE *exit_code)
{
    formula->valid_exp_type = VALUE;
    parse_operand(exp, &formula->is_constant, &formula->value, &formula->cell, ss, exit_code);
}

/* --- Helper: Parse an arithmetic expression (e.g., "A1+23") --- */
void parse_arithmetic(const char *exp, Cell_Formula *formula, Spread_Sheet *ss, TCU_EXIT_CODE *exit_code)
{
    // printf("Parsing arithmetic expression: %s\n", exp);
    formula->valid_exp_type = VALUE_OP_VALUE;

    // Make a writable copy of the incoming expression
    char *copy = strdup(exp);
    if (!copy)
    {
        *exit_code = MALLOC_FAILED;
        return;
    }
    trim_whitespace(copy);

    // Remove all internal spaces so we don't accidentally treat them as part of integers
    // (Optional, but helpful if user writes " -10 +  20 " etc.)
    {
        char *dst = copy;
        char *src = copy;
        while (*src)
        {
            if (!isspace((unsigned char)*src))
            {
                *dst++ = *src;
            }
            src++;
        }
        *dst = '\0';
    }

    // If the expression starts with '+' or '-' followed by a digit,
    // that's a sign for the LEFT operand, not the arithmetic operator.
    // So we skip ahead when searching for the real operator.
    int start_index = 0;
    if ((copy[0] == '+' || copy[0] == '-') && isdigit((unsigned char)copy[1]))
    {
        start_index = 1;
        // We'll keep it in the left substring; just skip it when looking for the operator below.
    }

    // Find the first occurrence of +, -, * or / after we skip the sign of the left operand
    int op_index = -1;
    char op = '\0';
    for (int i = start_index; copy[i] != '\0'; i++)
    {
        if (strchr("+-*/", copy[i]) != NULL)
        {
            op = copy[i];
            op_index = i;
            break;
        }
    }

    // If no operator found, this isn't a valid arithmetic expression
    if (op_index == -1)
    {
        *exit_code = INVALID_INPUT;
        free(copy);
        return;
    }

    // The left operand is everything up to the operator index
    // (including a possible leading sign if it was found)
    char *left = strndup(copy, op_index);
    // The right operand is everything after the operator
    char *right = strdup(copy + op_index + 1);

    if (!left || !right)
    {
        *exit_code = MALLOC_FAILED;
        free(left);
        free(right);
        free(copy);
        return;
    }

    // Parse each side using your updated parse_operand that handles signed integers
    parse_operand(left,
                  &formula->is_left_value_constant,
                  &formula->left_value,
                  &formula->left_cell,
                  ss,
                  exit_code);
    if (*exit_code != TCU_OK)
    {
        free(left);
        free(right);
        free(copy);
        return;
    }

    parse_operand(right,
                  &formula->is_right_value_constant,
                  &formula->right_value,
                  &formula->right_cell,
                  ss,
                  exit_code);
    if (*exit_code != TCU_OK)
    {
        free(left);
        free(right);
        free(copy);
        return;
    }

    // Determine which arithmetic operator we have
    switch (op)
    {
    case '+':
        formula->arithmetic_op = ADDITION;
        break;
    case '-':
        formula->arithmetic_op = SUBTRACTION;
        break;
    case '*':
        formula->arithmetic_op = MULTIPLICATION;
        break;
    case '/':
        formula->arithmetic_op = DIVISION;
        break;
    default:
        *exit_code = INVALID_INPUT;
        free(left);
        free(right);
        free(copy);
        return;
    }

    free(left);
    free(right);
    free(copy);
}

/* --- Helper: Parse a range string (e.g., "A1:A20" or "A1:D10")
         Returns an allocated Cell_Range structure.
*/
Cell_Range *parse_range(const char *range_str, Spread_Sheet *ss, TCU_EXIT_CODE *exit_code)
{
    Cell_Range *range = malloc(sizeof(Cell_Range));
    if (!range)
    {
        *exit_code = MALLOC_FAILED;
        return NULL;
    }
    char *colon = strchr(range_str, ':');
    if (!colon)
    {
        free(range);
        *exit_code = INVALID_INPUT;
        return NULL;
    }
    int len1 = colon - range_str;
    char *start_str = strndup(range_str, len1);
    char *end_str = strdup(colon + 1);
    if (!start_str || !end_str)
    {
        free(start_str);
        free(end_str);
        free(range);
        *exit_code = MALLOC_FAILED;
        return NULL;
    }
    int start_row, start_col, end_row, end_col;
    parse_cell_name(start_str, &start_row, &start_col);
    parse_cell_name(end_str, &end_row, &end_col);
    if (start_row > end_row || start_col > end_col)
    {
        free(start_str);
        free(end_str);
        free(range);
        *exit_code = INVALID_INPUT;
        return NULL;
    }
    SCell *start_scell = get_scell_by_coordinates(ss, start_row, start_col);
    SCell *end_scell = get_scell_by_coordinates(ss, end_row, end_col);
    if (!start_scell || !end_scell)
    {
        free(start_str);
        free(end_str);
        free(range);
        printf("Invalid cell reference in range\n");
        *exit_code = OUT_OF_RANGE;
        return NULL;
    }
    range->start_cell = start_scell->cell;
    range->end_cell = end_scell->cell;
    range->cell_range_type = (start_row == end_row || start_col == end_col) ? ONE_D : TWO_D;
    free(start_str);
    free(end_str);
    return range;
}

/* --- Helper: Parse a function call expression.
         Supported functions: MIN, MAX, AVG, SUM, STDEV (which operate on a range)
         and SLEEP (which takes a value).
*/
void parse_function(const char *exp, Cell_Formula *formula, Spread_Sheet *ss, TCU_EXIT_CODE *exit_code)
{
    formula->valid_exp_type = FUNCT_ON_RANGE;
    const char *open_paren = strchr(exp, '(');
    const char *close_paren = strrchr(exp, ')');
    if (!open_paren || !close_paren)
    {
        *exit_code = INVALID_INPUT;
        return;
    }
    int func_name_len = open_paren - exp;
    char *func_name = strndup(exp, func_name_len);
    int param_len = close_paren - open_paren - 1;
    char *param = strndup(open_paren + 1, param_len);
    if (!func_name || !param)
    {
        free(func_name);
        free(param);
        *exit_code = MALLOC_FAILED;
        return;
    }
    if (strcmp(func_name, "MIN") == 0)
    {
        formula->function = MIN;
        formula->cell_range = parse_range(param, ss, exit_code);
    }
    else if (strcmp(func_name, "MAX") == 0)
    {
        formula->function = MAX;
        formula->cell_range = parse_range(param, ss, exit_code);
    }
    else if (strcmp(func_name, "AVG") == 0)
    {
        formula->function = AVG;
        formula->cell_range = parse_range(param, ss, exit_code);
    }
    else if (strcmp(func_name, "SUM") == 0)
    {
        formula->function = SUM;
        formula->cell_range = parse_range(param, ss, exit_code);
    }
    else if (strcmp(func_name, "STDEV") == 0)
    {
        formula->function = STDEV;
        formula->cell_range = parse_range(param, ss, exit_code);
    }
    else if (strcmp(func_name, "SLEEP") == 0)
    {
        formula->function = SLEEP;
        /* For SLEEP, the parameter is a value rather than a range.
           Create a dummy Cell_Range that holds a single cell. */
        Cell *dummy_cell = malloc(sizeof(Cell));
        if (!dummy_cell)
        {
            *exit_code = MALLOC_FAILED;
            free(func_name);
            free(param);
            return;
        }
        SIM_BOOL is_const;
        int val;
        Cell *ref_cell;
        parse_operand(param, &is_const, &val, &ref_cell, ss, exit_code);
        if (is_const == TRUE)
        {
            init_cell(dummy_cell, 0, 0, val);
        }
        else
        {
            dummy_cell = ref_cell;
        }
        formula->cell_range = malloc(sizeof(Cell_Range));
        if (!formula->cell_range)
        {
            *exit_code = MALLOC_FAILED;
            free(func_name);
            free(param);
            return;
        }
        formula->cell_range->start_cell = dummy_cell;
        formula->cell_range->end_cell = dummy_cell;
        formula->cell_range->cell_range_type = ONE_D;
    }
    else
    {
        *exit_code = INVALID_INPUT;
    }
    free(func_name);
    free(param);
}

/* Custom implementation of absolute value for doubles */
double my_fabs(double x)
{
    return (x < 0.0) ? -x : x;
}

/* Custom implementation of sqrt using Newton–Raphson method */
double sqrt(double x)
{
    /* Return an error indicator for negative input.
       You may choose to handle this differently. */
    if (x < 0.0)
    {
        return -1.0;
    }

    /* Initial guess: use x if x > 1, otherwise 1 */
    double guess = (x > 1.0) ? x : 1.0;
    const double epsilon = 1e-10;

    /* Iterate until the change is small enough */
    while (my_fabs(guess * guess - x) > epsilon)
    {
        guess = (guess + x / guess) / 2.0;
    }

    return guess;
}

/* --- Helper: Evaluate a parsed formula and return an integer value.
         This function uses your cell functions (such as get_cell_value) and iterates
         over a range when needed.
*/
int evaluate_formula(Cell_Formula *formula, Spread_Sheet *ss, SIM_BOOL sleep_over_ride, TCU_EXIT_CODE *exit_code)
{
    if (formula->valid_exp_type == VALUE)
    {
        if (formula->is_constant == TRUE)
            return formula->value;
        else
            return get_cell_value(formula->cell);
    }
    else if (formula->valid_exp_type == VALUE_OP_VALUE)
    {
        int left = (formula->is_left_value_constant == TRUE) ? formula->left_value : get_cell_value(formula->left_cell);
        int right = (formula->is_right_value_constant == TRUE) ? formula->right_value : get_cell_value(formula->right_cell);
        switch (formula->arithmetic_op)
        {
        case ADDITION:
            return left + right;
        case SUBTRACTION:
            return left - right;
        case MULTIPLICATION:
            return left * right;
        case DIVISION:
            if (right == 0)
            {
                *exit_code = INVALID_INPUT;
                return 0;
            }
            return left / right;
        default:
            return 0;
        }
    }
    else if (formula->valid_exp_type == FUNCT_ON_RANGE)
    {
        if (formula->function == SLEEP)
        {
            int sleep_val = get_cell_value(formula->cell_range->start_cell);
            if (sleep_over_ride == FALSE)
            {
                sleep(sleep_val);
            }
            return sleep_val;
        }
        else
        {
            int start_row = get_cell_row(formula->cell_range->start_cell);
            int start_col = get_cell_col(formula->cell_range->start_cell);
            int end_row = get_cell_row(formula->cell_range->end_cell);
            int end_col = get_cell_col(formula->cell_range->end_cell);
            int count = 0, sum = 0, min = 0, max = 0;
            double mean, variance = 0, stdev;
            SCell *first = get_scell_by_coordinates(ss, start_row, start_col);
            if (!first)
            {
                *exit_code = OUT_OF_RANGE;
                return 0;
            }
            min = max = get_cell_value(first->cell);
            for (int r = start_row; r <= end_row; r++)
            {
                for (int c = start_col; c <= end_col; c++)
                {
                    SCell *curr = get_scell_by_coordinates(ss, r, c);
                    if (curr)
                    {
                        int val = get_cell_value(curr->cell);
                        sum += val;
                        if (val < min)
                            min = val;
                        if (val > max)
                            max = val;
                        count++;
                    }
                }
            }
            switch (formula->function)
            {
            case MIN:
                return min;
            case MAX:
                return max;
            case SUM:
                return sum;
            case AVG:
                return (count > 0) ? sum / count : 0;
            case STDEV:
                mean = (count > 0) ? (double)sum / count : 0;
                for (int r = start_row; r <= end_row; r++)
                {
                    for (int c = start_col; c <= end_col; c++)
                    {
                        SCell *curr = get_scell_by_coordinates(ss, r, c);
                        if (curr)
                        {
                            int val = get_cell_value(curr->cell);
                            variance += (val - mean) * (val - mean);
                        }
                    }
                }
                if (count > 0)
                    variance /= count;
                stdev = sqrt(variance);
                return (int)stdev;
            default:
                return 0;
            }
        }
    }
    return 0;
}

/* === Main Function: parse_expression ===
   This function extracts the target cell (using the cell name from target_cell_buff),
   parses the expression in exp_buff (which can be a constant, cell reference, arithmetic expression,
   or a function call), and finally evaluates the formula and assigns the result to the target cell.
*/
void parse_expression(char target_cell_buff[], char exp_buff[], Spread_Sheet *ss, TCU_EXIT_CODE *exit_code)
{

    int target_row, target_col;
    parse_cell_name(target_cell_buff, &target_row, &target_col);
    SCell *target_scell = get_scell_by_coordinates(ss, target_row, target_col);
    if (!target_scell)
    {
        *exit_code = OUT_OF_RANGE;
        return;
    }

    Cell_Formula *formula = malloc(sizeof(Cell_Formula));
    // printf("Exp Buff: %s\n", exp_buff);
    if (!formula)
    {
        *exit_code = MALLOC_FAILED;
        return;
    }

    /*
        Decide the expression type based on exp_buff.
        Function calls are tested first, then arithmetic expressions, then simple values.
    */
    if ((strncmp(exp_buff, "MIN(", 4) == 0) ||
        (strncmp(exp_buff, "MAX(", 4) == 0) ||
        (strncmp(exp_buff, "AVG(", 4) == 0) ||
        (strncmp(exp_buff, "SUM(", 4) == 0) ||
        (strncmp(exp_buff, "STDEV(", 6) == 0) ||
        (strncmp(exp_buff, "SLEEP(", 6) == 0))
    {
        parse_function(exp_buff, formula, ss, exit_code);
    }

    /*
        Checking for Value_OP_Value
    */

    else if ((strchr(exp_buff, '+') - exp_buff) > 0 || (strchr(exp_buff, '-') - exp_buff > 0) || (strchr(exp_buff, '*') - exp_buff > 0) || (strchr(exp_buff, '/') - exp_buff > 0))
    {

        parse_arithmetic(exp_buff, formula, ss, exit_code);
    }
    // 3) Otherwise parse as a single value or cell reference
    else
    {
        parse_value(exp_buff, formula, ss, exit_code);
    }

    // If parsing failed for any reason, free formula and return
    if (*exit_code != TCU_OK)
    {
        free(formula);
        return;
    }

    TCU_EXIT_CODE update_exit_code;

    update_logic_unit(ss, target_scell, formula, &update_exit_code);

    if (update_exit_code != TCU_OK)
    {
        *exit_code = update_exit_code;
    }
    // target_scell->cell_formula = formula;
    // int result = evaluate_formula(formula, ss, exit_code);
    // set_cell_value(target_scell->cell, result);

    return;
}

// ------------------------------------------------------------------------- //

void debug_print_scell(Spread_Sheet *ss, SCell *scell)
{
    printf("// -------------------------------------------------------------------------------------------------------------------------------------------------- //\n");
    printf("Cell - Row: %d, Col: %d, Value: %d\n", get_cell_row(scell->cell), get_cell_col(scell->cell), get_cell_value(scell->cell));
    for (int i = 0; i < scell->dependent_scells->size; i++)
    {
        printf("Dependent Cell %d: Row: %d, Col: %d, Value: %d\n", i, get_cell_row(scell->dependent_scells->scell_ptrs[i]->cell), get_cell_col(scell->dependent_scells->scell_ptrs[i]->cell), get_cell_value(scell->dependent_scells->scell_ptrs[i]->cell));
    }


    debug_print_formula(scell->cell_formula);








    int prec_cell_count = 0;

    if (scell->cell_formula->valid_exp_type == VALUE && scell->cell_formula->is_constant == FALSE)
    {
        printf("Precedent Cell %d: Row: %d, Col: %d, Value: %d\n", prec_cell_count, scell->cell->row, scell->cell->col, scell->cell->value);
        prec_cell_count++;
    }
    else if (scell->cell_formula->valid_exp_type == VALUE_OP_VALUE)
    {
        if (scell->cell_formula->is_left_value_constant == FALSE)
        {
            printf("Precedent Cell %d: Row: %d, Col: %d, Value: %d\n", prec_cell_count, scell->cell_formula->left_cell->row, scell->cell_formula->left_cell->col, scell->cell_formula->left_cell->value);
            prec_cell_count++;
        }
        if (scell->cell_formula->is_right_value_constant == FALSE)
        {
            printf("Precedent Cell %d: Row: %d, Col: %d, Value: %d\n", prec_cell_count, scell->cell_formula->right_cell->row, scell->cell_formula->right_cell->col, scell->cell_formula->right_cell->value);
            prec_cell_count++;
        }
    }
    else if (scell->cell_formula->valid_exp_type == FUNCT_ON_RANGE && scell->cell_formula->function != SLEEP && scell->cell_formula->cell_range != NULL)
    {

        if (scell->cell_formula->function != SLEEP)
        {
            int i_min = get_cell_row(scell->cell_formula->cell_range->start_cell);
            int j_min = get_cell_col(scell->cell_formula->cell_range->start_cell);
            int i_max = get_cell_row(scell->cell_formula->cell_range->end_cell);
            int j_max = get_cell_col(scell->cell_formula->cell_range->end_cell);

            for (int r = i_min; r <= i_max; r++)
            {
                for (int c = j_min; c <= j_max; c++)
                {
                    printf("Precedent Cell %d: Row: %d, Col: %d, Value: %d\n", prec_cell_count, r, c, get_cell_value(ss->arr[r * ss->SS_COLS + c].cell));
                    prec_cell_count++;
                }
            }
        }
    }

    printf("// -------------------------------------------------------------------------------------------------------------------------------------------------- //\n");
    return;
}

// (1) Remove Old Dependencies
// For each cell in target->precedent_scells, remove target from that cell's dependent list.
void remove_old_dependencies(Spread_Sheet *ss, SCell *target)
{

    if (target == NULL)
        return;

    SCell *prec_scell = NULL;
    Cell *prec_cell = NULL;

    if (target->cell_formula->valid_exp_type == VALUE)
    {
        if (target->cell_formula->is_constant == FALSE)
        {
            prec_cell = target->cell_formula->cell;
            prec_scell = get_scell_by_coordinates(ss, get_cell_row(prec_cell), get_cell_col(prec_cell));
            remove_scell_ptr(prec_scell->dependent_scells, target);
        }
    }
    else if (target->cell_formula->valid_exp_type == VALUE_OP_VALUE)
    {
        if (target->cell_formula->is_left_value_constant == FALSE)
        {
            prec_cell = target->cell_formula->left_cell;
            prec_scell = get_scell_by_coordinates(ss, get_cell_row(prec_cell), get_cell_col(prec_cell));
            remove_scell_ptr(prec_scell->dependent_scells, target);
        }
        if (target->cell_formula->is_right_value_constant == FALSE)
        {
            prec_cell = target->cell_formula->right_cell;
            prec_scell = get_scell_by_coordinates(ss, get_cell_row(prec_cell), get_cell_col(prec_cell));
            remove_scell_ptr(prec_scell->dependent_scells, target);
        }
    }
    else if (target->cell_formula->valid_exp_type == FUNCT_ON_RANGE)
    {
        if (target->cell_formula->function != SLEEP)
        {
            int i_min = get_cell_row(target->cell_formula->cell_range->start_cell);
            int j_min = get_cell_col(target->cell_formula->cell_range->start_cell);
            int i_max = get_cell_row(target->cell_formula->cell_range->end_cell);
            int j_max = get_cell_col(target->cell_formula->cell_range->end_cell);

            for (int r = i_min; r <= i_max; r++)
            {
                for (int c = j_min; c <= j_max; c++)
                {
                    prec_scell = get_scell_by_coordinates(ss, r, c);
                    if (prec_scell != NULL)
                    {
                        remove_scell_ptr(prec_scell->dependent_scells, target);
                    }
                }
            }
        }
    }

    return;
}

// (2) Add New Dependencies
// For each new precedent cell, add target to its dependent list and add that precedent cell to target->precedent_scells.
void add_new_dependencies(SCell *target, SCell *new_precedent_tl, SCell *new_precedent_br, Spread_Sheet *ss)
{
    if (target == NULL || new_precedent_tl == NULL || new_precedent_br == NULL)
        return;

    int i_min = new_precedent_tl->cell->row;
    int i_max = new_precedent_br->cell->row;
    int j_min = new_precedent_tl->cell->col;
    int j_max = new_precedent_br->cell->col;

    for (int i = i_min; i <= i_max; i++)
    {
        for (int j = j_min; j <= j_max; j++)
        {

            SCell *prec = &(ss->arr[i * ss->SS_COLS + j]);
            // Add target to the precedent's dependent list.
            push_back_scell_ptrs(prec->dependent_scells, target);
            // Also add the precedent to target's precedent list.
        }
    }

    return;
}

void update_logic_unit(Spread_Sheet *ss, SCell *node, Cell_Formula *cformula, TCU_EXIT_CODE *exit_code)
{
    // debug_print_formula(cformula);
    SCell *tnode_l = NULL;
    SCell *tnode_r = NULL;

    // Allocate a stack for cycle detection
    Stack_SCell *vis_stack = (Stack_SCell *)malloc(sizeof(Stack_SCell));
    if (!vis_stack)
    {
        *exit_code = MALLOC_FAILED;
        return;
    }
    // printf("Begin\n");
    // printf("%d\n", cformula->valid_exp_type);

    // remove_old_dependencies(ss, node);

    if (cformula->valid_exp_type == VALUE)
    {
        if (cformula->is_constant == FALSE)
        {
            init_stack(vis_stack, 10);
            SIM_BOOL cycle_check = FALSE;
            tnode_l = &(ss->arr[(cformula->cell->row) * (ss->SS_COLS) + cformula->cell->col]);
            dfs_cycle_check(node, tnode_l, tnode_l, vis_stack, &cycle_check);
            pop_and_unmark(ss, vis_stack);
            if (cycle_check == TRUE)
            {
                printf("Cycle Found\n");
                *exit_code = CYCLE_FOUND;
                free(vis_stack);
                return;
            }
            // printf("HI\n");
            remove_old_dependencies(ss, node);
            add_new_dependencies(node, tnode_l, tnode_l, ss);
        }
        else
        {
            remove_old_dependencies(ss, node);
            add_new_dependencies(node, NULL, NULL, ss);
        }
        // printf("YO\n");
    }
    else if (cformula->valid_exp_type == VALUE_OP_VALUE)
    {

        init_stack(vis_stack, 10);
        SIM_BOOL left_cycle_check = FALSE;
        SIM_BOOL right_cycle_check = FALSE;

        if(cformula->is_left_value_constant == FALSE)
        {
            tnode_l = &(ss->arr[(cformula->left_cell->row) * (ss->SS_COLS) + cformula->left_cell->col]);
            dfs_cycle_check(node, tnode_l, tnode_l, vis_stack, &left_cycle_check);
            pop_and_unmark(ss, vis_stack);
        }

        if(cformula->is_right_value_constant == FALSE)
        {
            tnode_r = &(ss->arr[(cformula->right_cell->row) * (ss->SS_COLS) + cformula->right_cell->col]);
            dfs_cycle_check(node, tnode_r, tnode_r, vis_stack, &right_cycle_check);
            pop_and_unmark(ss, vis_stack);
        }

        if (left_cycle_check == TRUE || right_cycle_check == TRUE)
        {
            printf("Cycle Found\n");
            *exit_code = CYCLE_FOUND;
            free(vis_stack);
            return;
        }

        remove_old_dependencies(ss, node);
        if (cformula->is_left_value_constant == FALSE)
        {
            add_new_dependencies(node, tnode_l, tnode_l, ss);
        }
        if (cformula->is_right_value_constant == FALSE && (cformula->left_cell != cformula->right_cell))
        {
            add_new_dependencies(node, tnode_r, tnode_r, ss);
        }

        // if (cformula->is_left_value_constant == FALSE)
        // {
        //     init_stack(vis_stack, 10);
        //     tnode_l = &(ss->arr[(cformula->left_cell->row) * (ss->SS_COLS) + cformula->left_cell->col]);
        //     SIM_BOOL cycle_check = FALSE;
        //     dfs_cycle_check(node, tnode_l, tnode_l, vis_stack, &cycle_check);
        //     pop_and_unmark(ss, vis_stack);
        //     if (cycle_check == TRUE)
        //     {
        //         *exit_code = CYCLE_FOUND;
        //         free(vis_stack);
        //         return;
        //     }

        //     // printf("Hi\n");
        //     // debug_print_scell(ss, tnode_l);

        //     // remove_old_dependencies(node);
        //     add_new_dependencies(node, tnode_l, tnode_l, ss);
        // }

        // if (cformula->is_right_value_constant == FALSE && (cformula->left_cell != cformula->right_cell))
        // {
        //     init_stack(vis_stack, 10);
        //     tnode_r = &(ss->arr[(cformula->right_cell->row) * (ss->SS_COLS) + cformula->right_cell->col]);
        //     SIM_BOOL cycle_check = FALSE;
        //     dfs_cycle_check(node, tnode_r, tnode_r, vis_stack, &cycle_check);
        //     pop_and_unmark(ss, vis_stack);
        //     if (cycle_check == TRUE)
        //     {
        //         *exit_code = CYCLE_FOUND;
        //         free(vis_stack);
        //         return;
        //     }

        //     // printf("Hi\n");
        //     // debug_print_scell(ss, tnode_r);

        //     //  remove_old_dependencies(node);
        //     add_new_dependencies(node, tnode_r, tnode_r, ss);
        // }
    }
    else if (cformula->valid_exp_type == FUNCT_ON_RANGE)
    {
        if (cformula->function == SLEEP)
        {
            // For SLEEP, immediately evaluate the formula.
            // get_cell_value of the dummy cell in the cell_range will yield the sleep argument.
            int sleep_val = evaluate_formula(cformula, ss, FALSE, exit_code);
            // Only sleep if sleep_over_ride is false (here we pass FALSE)
            if (*exit_code == TCU_OK)
            {
                sleep(sleep_val);
                set_cell_value(node->cell, sleep_val);
                node->cell_formula = cformula;
            }

            remove_old_dependencies(ss, node);
            add_new_dependencies(node, tnode_l, tnode_r, ss);

            free(vis_stack);
        }
        else
        {
            init_stack(vis_stack, 10);
            tnode_l = &(ss->arr[(cformula->cell_range->start_cell->row) * (ss->SS_COLS) + cformula->cell_range->start_cell->col]);
            tnode_r = &(ss->arr[(cformula->cell_range->end_cell->row) * (ss->SS_COLS) + cformula->cell_range->end_cell->col]);
            SIM_BOOL cycle_check = FALSE;
            dfs_cycle_check(node, tnode_l, tnode_r, vis_stack, &cycle_check);
            pop_and_unmark(ss, vis_stack);
            if (cycle_check == TRUE)
            {
                printf("Cycle Found\n");
                *exit_code = CYCLE_FOUND;
                free(vis_stack);
                return;
            }
            remove_old_dependencies(ss, node);
            add_new_dependencies(node, tnode_l, tnode_r, ss);
        }

        return;
    }
    else
    {
        *(exit_code) = INVALID_INPUT;
        free(vis_stack);
        return;
    }

    node->cell_formula = cformula;

    Stack_SCell *topo_sort_st = (Stack_SCell *)malloc(sizeof(Stack_SCell));
    if (!topo_sort_st)
    {
        *exit_code = MALLOC_FAILED;
        free(vis_stack);
        return;
    }

    init_stack(topo_sort_st, 10);
    // printf("Checking formula assignment\n");
    // debug_print_formula(node->cell_formula);
    dfs_topological(node, topo_sort_st);
    // printf("Checking formula assignment after dfs\n");
    // debug_print_formula(node->cell_formula);
    pop_and_update(topo_sort_st, ss, exit_code);

    free(vis_stack);
    free(topo_sort_st);
    return;
}

// ------------------------------------------------------------------------- //

void pop_and_update(Stack_SCell *topo_sort_st, Spread_Sheet *ss, TCU_EXIT_CODE *exit_code)
{
    while (topo_sort_st->top >= 0)
    {
        SCell *node = pop_stack(topo_sort_st);
        node->visited = FALSE;
        // debug_print_formula(node->cell_formula);
        // debug_print_scell(node);
        node->cell->value = evaluate_formula(node->cell_formula, ss, FALSE, exit_code);
    }
    return;
}

void pop_and_unmark(Spread_Sheet *ss, Stack_SCell *visitedStack)
{
    while (visitedStack->top >= 0)
    {
        SCell *node = pop_stack(visitedStack);
        node->visited = FALSE;
        // debug_print_scell(ss, node);
    }
    return;
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
            sprintf(col_data_buff, "%d", ((ss->arr[i * (ss->SS_COLS) + j]).cell)->value);
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
    case CYCLE_FOUND:
        strcpy(error_buff, "Cycle Found");
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
    int row_render = 0, col_render = 0;
    TCU_EXIT_CODE exit_code = TCU_OK;
    SIM_BOOL en_ss_render = TRUE;
    int tcell_row, tcell_col;

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
                printf("Quitting the Spreadsheet Program\n");
                exit_code = TCU_OK;
                clock_gettime(CLOCK_MONOTONIC, &end_ts);
                command_time = (end_ts.tv_sec - start_ts.tv_sec) +
                               (end_ts.tv_nsec - start_ts.tv_nsec) / 1e9;
                break;
            }
            else if (strcmp(command_buff, "w") == 0)
            {
                row_render = next_render_dim(row_render, ss->SS_ROWS, -MAX_RENDER_DIM, &exit_code);
            }
            else if (strcmp(command_buff, "s") == 0)
            {
                row_render = next_render_dim(row_render, ss->SS_ROWS, MAX_RENDER_DIM, &exit_code);
            }
            else if (strcmp(command_buff, "a") == 0)
            {
                col_render = next_render_dim(col_render, ss->SS_COLS, -MAX_RENDER_DIM, &exit_code);
            }
            else if (strcmp(command_buff, "d") == 0)
            {
                col_render = next_render_dim(col_render, ss->SS_COLS, MAX_RENDER_DIM, &exit_code);
            }
            else if (strcmp(command_buff, "disable_output") == 0)
            {
                if (en_ss_render)
                {
                    en_ss_render = FALSE;
                }
                exit_code = TCU_OK;
            }
            else if (strcmp(command_buff, "enable_output") == 0)
            {
                if (!en_ss_render)
                {
                    en_ss_render = TRUE;
                }
                exit_code = TCU_OK;
            }
            else if (strncmp(command_buff, "scroll_to ", 10) == 0)
            {
                sscanf(command_buff, "scroll_to %6s", target_cell_buff);
                is_valid_cell(target_cell_buff, ss->SS_ROWS, ss->SS_COLS,
                              &tcell_row, &tcell_col, &exit_code);

                if (exit_code == TCU_OK)
                {
                    row_render = tcell_row;
                    col_render = tcell_col;
                }
            }
            else if (strncmp(command_buff, "dbg ", 4) == 0)
            {
                // Debug command: parse cell name from after "dbg "
                int rows = 0, cols = 0;
                parse_cell_name(command_buff + 4, &rows, &cols);
                // printf("ROW: %d, COL: %d\n", rows, cols);
                SCell *sc = get_scell_by_coordinates(ss, rows, cols);
                // printf("Value : %d\n", sc->cell->value);
                debug_print_scell(ss, sc);
            }
            else
            {
                printf("You entered %s\n", command_buff);
                parse_command(command_buff, target_cell_buff, exp_buff, ss, &exit_code);
                if (exit_code == TCU_OK)
                {
                    printf("Target Cell: %s, Eval Expression: %s\n", target_cell_buff, exp_buff);
                    parse_expression(target_cell_buff, exp_buff, ss, &exit_code);
                }
            }

            // Stop timing immediately after processing finishes
            clock_gettime(CLOCK_MONOTONIC, &end_ts);
            command_time = (end_ts.tv_sec - start_ts.tv_sec) +
                           (end_ts.tv_nsec - start_ts.tv_nsec) / 1e9;
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
