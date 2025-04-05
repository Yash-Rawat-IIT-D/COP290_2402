// parse_command_unit_test.c

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../header/parser.h"       // Contains parse_command, parse_expression, etc.
#include "../header/spreadsheet.h"  // Contains the definition of Spread_Sheet and SCell
#include "../header/constants.h"    // Contains the constants and REGEX_PATTERN

// --- Dummy Spread_Sheet Initialization ---
//
// This function creates a Spread_Sheet with the specified number
// of rows and columns. It allocates an array of SCell and initializes
// each cell with default values.
Spread_Sheet *init_dummy_spreadsheet(int rows, int cols) {
    Spread_Sheet *ss = malloc(sizeof(Spread_Sheet));
    if (!ss) {
        fprintf(stderr, "Malloc failed for Spread_Sheet\n");
        exit(1);
    }
    ss->SS_ROWS = rows;
    ss->SS_COLS = cols;
    ss->arr = malloc(sizeof(SCell) * rows * cols);
    if (!ss->arr) {
        fprintf(stderr, "Malloc failed for SCell array\n");
        free(ss);
        exit(1);
    }
    for (int i = 0; i < rows * cols; i++) {
        ss->arr[i].value = 0;
        ss->arr[i].cell_formula = NULL;
        ss->arr[i].dependent_scells = NULL;
        ss->arr[i].dependent_scells_size = 0;
        ss->arr[i].dependent_scells_capacity = 0;
        ss->arr[i].visited_err_flag = '0';
    }
    return ss;
}

// Free the allocated Spread_Sheet.
void free_dummy_spreadsheet(Spread_Sheet *ss) {
    if (ss) {
        free(ss->arr);
        free(ss);
    }
}

// ----------------- Test Cases ----------------- //

// Test valid simple numeric expression "A1 = 42"
void test_parse_command_valid() {
    Spread_Sheet *ss = init_dummy_spreadsheet(10, 10);
    char command_buff[100] = "A1 = 42";
    char target_cell_buff[10] = {0};
    char exp_buff[100] = {0};
    char exit_code = '9'; // initialize to non-'0'
    parse_command(command_buff, target_cell_buff, exp_buff, ss, &exit_code);
    printf("Test Valid Input:\n");
    printf("  Input Command: \"%s\"\n", command_buff);
    printf("  Expected: exit_code = '0', target_cell = \"A1\", expression = \"42\"\n");
    printf("  Actual:   exit_code = '%c', target_cell = \"%s\", expression = \"%s\"\n\n",
           exit_code, target_cell_buff, exp_buff);
    assert(exit_code == '0');
    assert(strcmp(target_cell_buff, "A1") == 0);
    assert(strcmp(exp_buff, "42") == 0);
    free_dummy_spreadsheet(ss);
}

// Test invalid cell reference "a1=42" (lowercase)
void test_parse_command_invalid() {
    Spread_Sheet *ss = init_dummy_spreadsheet(10, 10);
    char command_buff[100] = "a1=42";  // invalid because cell must be uppercase
    char target_cell_buff[10] = {0};
    char exp_buff[100] = {0};
    char exit_code = '0';
    parse_command(command_buff, target_cell_buff, exp_buff, ss, &exit_code);
    printf("Test Invalid Cell Reference:\n");
    printf("  Input Command: \"%s\"\n", command_buff);
    printf("  Expected: exit_code != '0' (error)\n");
    printf("  Actual:   exit_code = '%c', target_cell = \"%s\", expression = \"%s\"\n\n",
           exit_code, target_cell_buff, exp_buff);
    assert(exit_code != '0');
    free_dummy_spreadsheet(ss);
}

// Test function expression "B2 = SLEEP(2)"
void test_parse_command_function() {
    Spread_Sheet *ss = init_dummy_spreadsheet(10, 10);
    char command_buff[100] = "B2 = SLEEP(2)";
    char target_cell_buff[10] = {0};
    char exp_buff[100] = {0};
    char exit_code = '9';
    parse_command(command_buff, target_cell_buff, exp_buff, ss, &exit_code);
    printf("Test Function Expression (SLEEP):\n");
    printf("  Input Command: \"%s\"\n", command_buff);
    printf("  Expected: exit_code = '0', target_cell = \"B2\", expression = \"SLEEP(2)\"\n");
    printf("  Actual:   exit_code = '%c', target_cell = \"%s\", expression = \"%s\"\n\n",
           exit_code, target_cell_buff, exp_buff);
    assert(exit_code == '0');
    assert(strcmp(target_cell_buff, "B2") == 0);
    assert(strcmp(exp_buff, "SLEEP(2)") == 0);
    free_dummy_spreadsheet(ss);
}

// Test arithmetic expression "C1 = 5+7"
void test_parse_command_arithmetic() {
    Spread_Sheet *ss = init_dummy_spreadsheet(10, 10);
    char command_buff[100] = "C1 = 5+7";
    char target_cell_buff[10] = {0};
    char exp_buff[100] = {0};
    char exit_code = '9';
    parse_command(command_buff, target_cell_buff, exp_buff, ss, &exit_code);
    printf("Test Arithmetic Expression:\n");
    printf("  Input Command: \"%s\"\n", command_buff);
    printf("  Expected: exit_code = '0', target_cell = \"C1\", expression = \"5+7\"\n");
    printf("  Actual:   exit_code = '%c', target_cell = \"%s\", expression = \"%s\"\n\n",
           exit_code, target_cell_buff, exp_buff);
    assert(exit_code == '0');
    assert(strcmp(target_cell_buff, "C1") == 0);
    assert(strcmp(exp_buff, "5+7") == 0);
    free_dummy_spreadsheet(ss);
}

// Test invalid function expression "D1 = SUM(C1)" (missing colon in range)
void test_parse_command_invalid_function() {
    Spread_Sheet *ss = init_dummy_spreadsheet(10, 10);
    char command_buff[100] = "D1 = SUM(C1)";
    char target_cell_buff[10] = {0};
    char exp_buff[100] = {0};
    char exit_code = '0';
    parse_command(command_buff, target_cell_buff, exp_buff, ss, &exit_code);
    printf("Test Invalid Function Expression (Missing Colon):\n");
    printf("  Input Command: \"%s\"\n", command_buff);
    printf("  Expected: exit_code != '0' (error due to invalid range format)\n");
    printf("  Actual:   exit_code = '%c', target_cell = \"%s\", expression = \"%s\"\n\n",
           exit_code, target_cell_buff, exp_buff);
    assert(exit_code != '0');
    free_dummy_spreadsheet(ss);
}

// Test function expression with single-cell range "E1 = SUM(A1:A1)"
void test_parse_command_single_cell_range() {
    Spread_Sheet *ss = init_dummy_spreadsheet(10, 10);
    char command_buff[100] = "E1 = SUM(A1:A1)";
    char target_cell_buff[10] = {0};
    char exp_buff[100] = {0};
    char exit_code = '9';
    parse_command(command_buff, target_cell_buff, exp_buff, ss, &exit_code);
    printf("Test Function Expression (Single-Cell Range):\n");
    printf("  Input Command: \"%s\"\n", command_buff);
    printf("  Expected: exit_code = '0', target_cell = \"E1\", expression = \"SUM(A1:A1)\"\n");
    printf("  Actual:   exit_code = '%c', target_cell = \"%s\", expression = \"%s\"\n\n",
           exit_code, target_cell_buff, exp_buff);
    assert(exit_code == '0');
    assert(strcmp(target_cell_buff, "E1") == 0);
    assert(strcmp(exp_buff, "SUM(A1:A1)") == 0);
    free_dummy_spreadsheet(ss);
}

// ---------------- Additional Test Cases for New Commands ---------------- //

// Test for arithmetic expression with multiple operators "A1 = 1+2+3"
// Our parser supports only a single operator. Therefore, we expect this input to be invalid.
void test_parse_command_multiple_arithmetic() {
    Spread_Sheet *ss = init_dummy_spreadsheet(10, 10);
    char command_buff[100] = "A1 = 1+2+3";
    char target_cell_buff[10] = {0};
    char exp_buff[100] = {0};
    char exit_code = '9';
    parse_command(command_buff, target_cell_buff, exp_buff, ss, &exit_code);
    printf("Test Arithmetic Expression with Multiple Operators:\n");
    printf("  Input Command: \"%s\"\n", command_buff);
    printf("  Expected: exit_code != '0' (error due to multiple operators)\n");
    printf("  Actual:   exit_code = '%c', target_cell = \"%s\", expression = \"%s\"\n\n",
           exit_code, target_cell_buff, exp_buff);
    assert(exit_code != '0');
    free_dummy_spreadsheet(ss);
}

// Test for range function with inverted range "A10 = SUM(D10:D1)"
// Since the range is not top-left to bottom-right, we expect an error.
void test_parse_command_inverted_range() {
    Spread_Sheet *ss = init_dummy_spreadsheet(20, 10);
    char command_buff[100] = "A10 = SUM(D10:D1)";
    char target_cell_buff[10] = {0};
    char exp_buff[100] = {0};
    char exit_code = '9';
    parse_command(command_buff, target_cell_buff, exp_buff, ss, &exit_code);
    printf("Test Function Expression with Inverted Range:\n");
    printf("  Input Command: \"%s\"\n", command_buff);
    printf("  Expected: exit_code != '0' (error due to inverted range)\n");
    printf("  Actual:   exit_code = '%c', target_cell = \"%s\", expression = \"%s\"\n\n",
           exit_code, target_cell_buff, exp_buff);
    assert(exit_code != '0');
    free_dummy_spreadsheet(ss);
}

// --- Unit Tests for parse_arithmetic_expr --- //

void test_parse_arithmetic_expr_constant_op_constant() {
    Spread_Sheet *ss = init_dummy_spreadsheet(10, 10);
    CELL_FORMULA cf;
    char exit_code = '9';
    parse_arithmetic_expr("42+58", &cf, ss, &exit_code);
    printf("Test Arithmetic (constant op constant):\n");
    printf("  Input Expression: \"42+58\"\n");
    printf("  Expected: valid_exp_type = '2', op = '+', left_value = 42, right_value = 58\n");
    printf("  Actual:   valid_exp_type = '%c', op = '%c', left_value = %d, right_value = %d\n\n",
           cf.valid_exp_type, cf.farith_cons_cons.arithmetic_op,
           cf.farith_cons_cons.left_value, cf.farith_cons_cons.right_value);
    assert(exit_code == '0');
    assert(cf.valid_exp_type == '2');
    assert(cf.farith_cons_cons.arithmetic_op == '+');
    assert(cf.farith_cons_cons.left_value == 42);
    assert(cf.farith_cons_cons.right_value == 58);
    free_dummy_spreadsheet(ss);
}

void test_parse_arithmetic_expr_constant_op_cell() {
    // For this test, we assume cell B2 exists.
    Spread_Sheet *ss = init_dummy_spreadsheet(10, 10);
    // Let's set a value for B2 so that the test makes sense.
    SCell *b2 = get_scell_by_coordinates(ss, 1, 1); // A=0, B=1 in 0-based indexing.
    b2->value = 100;
    CELL_FORMULA cf;
    char exit_code = '9';
    parse_arithmetic_expr("42+B2", &cf, ss, &exit_code);
    printf("Test Arithmetic (constant op cell):\n");
    printf("  Input Expression: \"42+B2\"\n");
    printf("  Expected: valid_exp_type = '3', op = '+', left_value = 42, right_cell = (1,1)\n");
    printf("  Actual:   valid_exp_type = '%c', op = '%c', left_value = %d, right_cell = (%d,%d)\n\n",
           cf.valid_exp_type, cf.farith_cons_cell.arithmetic_op,
           cf.farith_cons_cell.left_value,
           cf.farith_cons_cell.right_cell_row, cf.farith_cons_cell.right_cell_col);
    assert(exit_code == '0');
    assert(cf.valid_exp_type == '3');
    assert(cf.farith_cons_cell.arithmetic_op == '+');
    assert(cf.farith_cons_cell.left_value == 42);
    assert(cf.farith_cons_cell.right_cell_row == 1);
    assert(cf.farith_cons_cell.right_cell_col == 1);
    free_dummy_spreadsheet(ss);
}

void test_parse_arithmetic_expr_cell_op_constant() {
    // For this test, we assume cell A1 exists.
    Spread_Sheet *ss = init_dummy_spreadsheet(10, 10);
    SCell *a1 = get_scell_by_coordinates(ss, 0, 0);
    a1->value = 55;
    CELL_FORMULA cf;
    char exit_code = '9';
    parse_arithmetic_expr("A1+58", &cf, ss, &exit_code);
    printf("Test Arithmetic (cell op constant):\n");
    printf("  Input Expression: \"A1+58\"\n");
    printf("  Expected: valid_exp_type = '4', op = '+', left_cell = (0,0), right_value = 58\n");
    printf("  Actual:   valid_exp_type = '%c', op = '%c', left_cell = (%d,%d), right_value = %d\n\n",
           cf.valid_exp_type, cf.farith_cell_cons.arithmetic_op,
           cf.farith_cell_cons.left_cell_row, cf.farith_cell_cons.left_cell_col,
           cf.farith_cell_cons.right_value);
    assert(exit_code == '0');
    assert(cf.valid_exp_type == '4');
    assert(cf.farith_cell_cons.arithmetic_op == '+');
    assert(cf.farith_cell_cons.left_cell_row == 0);
    assert(cf.farith_cell_cons.left_cell_col == 0);
    assert(cf.farith_cell_cons.right_value == 58);
    free_dummy_spreadsheet(ss);
}

void test_parse_arithmetic_expr_cell_op_cell() {
    // For this test, we assume cells A1 and B2 exist.
    Spread_Sheet *ss = init_dummy_spreadsheet(10, 10);
    SCell *a1 = get_scell_by_coordinates(ss, 0, 0);
    SCell *b2 = get_scell_by_coordinates(ss, 1, 1);
    a1->value = 10;
    b2->value = 20;
    CELL_FORMULA cf;
    char exit_code = '9';
    parse_arithmetic_expr("A1+B2", &cf, ss, &exit_code);
    printf("Test Arithmetic (cell op cell):\n");
    printf("  Input Expression: \"A1+B2\"\n");
    printf("  Expected: valid_exp_type = '5', op = '+', left_cell = (0,0), right_cell = (1,1)\n");
    printf("  Actual:   valid_exp_type = '%c', op = '%c', left_cell = (%d,%d), right_cell = (%d,%d)\n\n",
           cf.valid_exp_type, cf.farith_cell_cell.arithmetic_op,
           cf.farith_cell_cell.left_cell_row, cf.farith_cell_cell.left_cell_col,
           cf.farith_cell_cell.right_cell_row, cf.farith_cell_cell.right_cell_col);
    assert(exit_code == '0');
    assert(cf.valid_exp_type == '5');
    assert(cf.farith_cell_cell.arithmetic_op == '+');
    assert(cf.farith_cell_cell.left_cell_row == 0);
    assert(cf.farith_cell_cell.left_cell_col == 0);
    assert(cf.farith_cell_cell.right_cell_row == 1);
    assert(cf.farith_cell_cell.right_cell_col == 1);
    free_dummy_spreadsheet(ss);
}

void test_parse_arithmetic_expr_cell_op_cell_extra() {
    /* We need a spreadsheet large enough to accommodate AA10 and ZZZ999.
       For example, 1000 rows and 20000 columns. */
    Spread_Sheet *ss = init_spread_sheet(1000, 20000);
    // Set dummy values if needed (not used in parsing, but can be set for evaluation)
    // For this test, we only check the parsing, not the evaluated values.
    CELL_FORMULA cf;
    char exit_code = '9';
    parse_arithmetic_expr("AA10+ZZZ999", &cf, ss, &exit_code);
    printf("Test Arithmetic (cell op cell - extra):\n");
    printf("  Input Expression: \"AA10+ZZZ999\"\n");
    printf("  Expected: valid_exp_type = '5', op = '+', left_cell = (9,26), right_cell = (998,18277)\n");
    printf("  Actual:   valid_exp_type = '%c', op = '%c', left_cell = (%d,%d), right_cell = (%d,%d)\n\n",
           cf.valid_exp_type, cf.farith_cell_cell.arithmetic_op,
           cf.farith_cell_cell.left_cell_row, cf.farith_cell_cell.left_cell_col,
           cf.farith_cell_cell.right_cell_row, cf.farith_cell_cell.right_cell_col);
    assert(exit_code == '0');
    assert(cf.valid_exp_type == '5');
    assert(cf.farith_cell_cell.arithmetic_op == '+');
    assert(cf.farith_cell_cell.left_cell_row == 9);      // 10 -> 9 in 0-based indexing
    assert(cf.farith_cell_cell.left_cell_col == 26);       // AA -> 27, 0-based index 26
    assert(cf.farith_cell_cell.right_cell_row == 998);     // 999 -> 998 in 0-based indexing
    assert(cf.farith_cell_cell.right_cell_col == 18277);    // ZZZ -> 18278, 0-based index 18277
    free_dummy_spreadsheet(ss);
}

// --- Unit Tests for parse_function_expr ---

// Test SLEEP with a constant argument.
void test_parse_function_expr_sleep_constant() {
    Spread_Sheet *ss = init_spread_sheet(10, 10);
    CELL_FORMULA cf;
    char exit_code = '9';
    parse_function_expr("SLEEP(5)", &cf, ss, &exit_code);
    printf("Test Function Expression - SLEEP Constant:\n");
    printf("  Input Expression: \"SLEEP(5)\"\n");
    printf("  Expected: valid_exp_type = '6', sleep_time = 5\n");
    printf("  Actual:   valid_exp_type = '%c', sleep_time = %d\n\n",
           cf.valid_exp_type, cf.fsleep_cons.sleep_time);
    assert(exit_code == '0');
    assert(cf.valid_exp_type == '6');
    assert(cf.fsleep_cons.sleep_time == 5);
    free_dummy_spreadsheet(ss);
}

// Test SLEEP with a cell reference argument.
void test_parse_function_expr_sleep_cell() {
    Spread_Sheet *ss = init_spread_sheet(10, 10);
    CELL_FORMULA cf;
    char exit_code = '9';
    parse_function_expr("SLEEP(B2)", &cf, ss, &exit_code);
    printf("Test Function Expression - SLEEP Cell:\n");
    printf("  Input Expression: \"SLEEP(B2)\"\n");
    printf("  Expected: valid_exp_type = '7', sleep_cell = (row=1, col=1)\n");
    printf("  Actual:   valid_exp_type = '%c', sleep_cell = (%d, %d)\n\n",
           cf.valid_exp_type, cf.fsleep_cell.sleep_cell_row, cf.fsleep_cell.sleep_cell_col);
    assert(exit_code == '0');
    assert(cf.valid_exp_type == '7');
    assert(cf.fsleep_cell.sleep_cell_row == 1); // B2: row 2 -> 0-based index 1
    assert(cf.fsleep_cell.sleep_cell_col == 1);  // B2: col B -> 0-based index 1
    free_dummy_spreadsheet(ss);
}

// Test MIN function with a range.
void test_parse_function_expr_min_range() {
    Spread_Sheet *ss = init_spread_sheet(10, 10);
    CELL_FORMULA cf;
    char exit_code = '9';
    parse_function_expr("MIN(A1:B2)", &cf, ss, &exit_code);
    printf("Test Function Expression - MIN Range:\n");
    printf("  Input Expression: \"MIN(A1:B2)\"\n");
    printf("  Expected: valid_exp_type = '8', function = 'M', range = (A1:B2) i.e., start=(0,0), end=(1,1)\n");
    printf("  Actual:   valid_exp_type = '%c', function = '%c', range = (start=(%d,%d), end=(%d,%d))\n\n",
           cf.valid_exp_type, cf.ffunc.function,
           cf.ffunc.start_row, cf.ffunc.start_col,
           cf.ffunc.end_row, cf.ffunc.end_col);
    assert(exit_code == '0');
    assert(cf.valid_exp_type == '8');
    assert(cf.ffunc.function == 'M');  // "MIN" returns 'M'
    assert(cf.ffunc.start_row == 0);     // A1 -> row 0
    assert(cf.ffunc.start_col == 0);     // A1 -> col 0
    assert(cf.ffunc.end_row == 1);       // B2 -> row 1
    assert(cf.ffunc.end_col == 1);       // B2 -> col 1
    free_dummy_spreadsheet(ss);
}

// Test SUM function with a range.
void test_parse_function_expr_sum_range() {
    Spread_Sheet *ss = init_spread_sheet(20, 20);
    CELL_FORMULA cf;
    char exit_code = '9';
    parse_function_expr("SUM(C3:D4)", &cf, ss, &exit_code);
    printf("Test Function Expression - SUM Range:\n");
    printf("  Input Expression: \"SUM(C3:D4)\"\n");
    printf("  Expected: valid_exp_type = '8', function = 'S', range = (C3:D4) i.e., start=(2,2), end=(3,3)\n");
    printf("  Actual:   valid_exp_type = '%c', function = '%c', range = (start=(%d,%d), end=(%d,%d))\n\n",
           cf.valid_exp_type, cf.ffunc.function,
           cf.ffunc.start_row, cf.ffunc.start_col,
           cf.ffunc.end_row, cf.ffunc.end_col);
    assert(exit_code == '0');
    assert(cf.valid_exp_type == '8');
    assert(cf.ffunc.function == 'S');  // "SUM" returns 'S'
    assert(cf.ffunc.start_row == 2);     // C3 -> row 2
    assert(cf.ffunc.start_col == 2);     // C3 -> col 2 (C)
    assert(cf.ffunc.end_row == 3);       // D4 -> row 3
    assert(cf.ffunc.end_col == 3);       // D4 -> col 3 (D)
    free_dummy_spreadsheet(ss);
}

// Test STDEV function with a range.
void test_parse_function_expr_stdev_range() {
    Spread_Sheet *ss = init_spread_sheet(10, 10);
    CELL_FORMULA cf;
    char exit_code = '9';
    parse_function_expr("STDEV(A1:A1)", &cf, ss, &exit_code);
    printf("Test Function Expression - STDEV Range:\n");
    printf("  Input Expression: \"STDEV(A1:A1)\"\n");
    printf("  Expected: valid_exp_type = '8', function = 'D', range = (A1:A1) i.e., start=(0,0), end=(0,0)\n");
    printf("  Actual:   valid_exp_type = '%c', function = '%c', range = (start=(%d,%d), end=(%d,%d))\n\n",
           cf.valid_exp_type, cf.ffunc.function,
           cf.ffunc.start_row, cf.ffunc.start_col,
           cf.ffunc.end_row, cf.ffunc.end_col);
    assert(exit_code == '0');
    assert(cf.valid_exp_type == '8');
    // Note: Because the code uses the first letter of the function name,
    // STDEV will have 'S' (same as SUM) in this implementation.
    assert(cf.ffunc.function == 'D');
    assert(cf.ffunc.start_row == 0);
    assert(cf.ffunc.start_col == 0);
    assert(cf.ffunc.end_row == 0);
    assert(cf.ffunc.end_col == 0);
    free_dummy_spreadsheet(ss);
}

// Test SUM function with a large range.
void test_parse_function_expr_sum_range_large() {
    // Allocate a spreadsheet with at least 999 rows and 18278 columns.
    // (For safety, we allocate a bit larger than needed.)
    Spread_Sheet *ss = init_spread_sheet(1000, 20000);
    CELL_FORMULA cf;
    char exit_code = '9';
    parse_function_expr("SUM(A1:ZZZ999)", &cf, ss, &exit_code);
    printf("Test Function Expression - SUM Large Range:\n");
    printf("  Input Expression: \"SUM(A1:ZZZ999)\"\n");
    printf("  Expected: valid_exp_type = '8', function = 'S', range = (start=(0,0), end=(998,18277))\n");
    printf("  Actual:   valid_exp_type = '%c', function = '%c', range = (start=(%d,%d), end=(%d,%d))\n\n",
           cf.valid_exp_type, cf.ffunc.function,
           cf.ffunc.start_row, cf.ffunc.start_col,
           cf.ffunc.end_row, cf.ffunc.end_col);
    assert(exit_code == '0');
    assert(cf.valid_exp_type == '8');
    assert(cf.ffunc.function == 'S');  // "SUM" returns 'S'
    assert(cf.ffunc.start_row == 0);     // A1 -> row 0
    assert(cf.ffunc.start_col == 0);     // A1 -> col 0
    assert(cf.ffunc.end_row == 998);       // 999 -> row 998
    assert(cf.ffunc.end_col == 18277);     // ZZZ -> 18278 - 1 = 18277
    free_dummy_spreadsheet(ss);
}






// --- Unit Tests for parse_operand_expr ---
//
// Test 1: Constant operand
void test_parse_operand_expr_constant() {
    Spread_Sheet *ss = init_spread_sheet(10, 10);
    CELL_FORMULA cf;
    char exit_code = '9';
    parse_operand_expr("123", &cf, ss, &exit_code);
    printf("Test parse_operand_expr (Constant):\n");
    printf("  Input: \"123\"\n");
    printf("  Expected: valid_exp_type = '0', value = 123\n");
    printf("  Actual:   valid_exp_type = '%c', value = %d\n\n", cf.valid_exp_type, cf.fvcons.value);
    assert(exit_code == '0');
    assert(cf.valid_exp_type == '0');
    assert(cf.fvcons.value == 123);
    free_dummy_spreadsheet(ss);
}

// Test 2: Cell reference operand
void test_parse_operand_expr_cell() {
    Spread_Sheet *ss = init_spread_sheet(10, 10);
    CELL_FORMULA cf;
    char exit_code = '9';
    parse_operand_expr("B2", &cf, ss, &exit_code);
    printf("Test parse_operand_expr (Cell Reference):\n");
    printf("  Input: \"B2\"\n");
    printf("  Expected: valid_exp_type = '1', cell_row = 1, cell_col = 1\n");
    printf("  Actual:   valid_exp_type = '%c', cell_row = %d, cell_col = %d\n\n",
           cf.valid_exp_type, cf.fvcell.cell_row, cf.fvcell.cell_col);
    assert(exit_code == '0');
    assert(cf.valid_exp_type == '1');
    assert(cf.fvcell.cell_row == 1);
    assert(cf.fvcell.cell_col == 1);
    free_dummy_spreadsheet(ss);
}






int main() {
    test_parse_command_valid();
    test_parse_command_invalid();
    test_parse_command_function();
    test_parse_command_arithmetic();
    test_parse_command_invalid_function();
    test_parse_command_single_cell_range();
    test_parse_command_multiple_arithmetic();
    test_parse_command_inverted_range();
    test_parse_arithmetic_expr_constant_op_constant();
    test_parse_arithmetic_expr_constant_op_cell();
    test_parse_arithmetic_expr_cell_op_constant();
    test_parse_arithmetic_expr_cell_op_cell();
    test_parse_arithmetic_expr_cell_op_cell_extra();
    test_parse_function_expr_sleep_constant();
    test_parse_function_expr_sleep_cell();
    test_parse_function_expr_min_range();
    test_parse_function_expr_sum_range();
    test_parse_function_expr_stdev_range();
    test_parse_function_expr_sum_range_large();
    test_parse_operand_expr_constant();
    test_parse_operand_expr_cell();
   
    
    printf("All unit tests passed.\n");
    return 0;
}
