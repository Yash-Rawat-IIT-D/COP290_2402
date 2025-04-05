// Cell Struct Definition and Functions

#include "../header/cell.h"  // Include the header file for declaration

// ------------------------------------------------------------------------- //

void init_cell(Cell *cell, int row, int col, int value)
{
    cell->row = row;
    cell->col = col;
    cell->value = value;
    return;
}

void set_cell_value(Cell *cell, int value)
{
    cell->value = value;
}

int get_cell_value(Cell *cell)
{
    return cell->value;
}

int get_cell_row(Cell *cell)
{
    return cell->row;
}

int get_cell_col(Cell *cell)
{
    return cell->col;
}

SIM_BOOL is_valid_cell_range(Cell_Range *cell_range)
{

    if (cell_range == NULL || cell_range->start_cell == NULL || cell_range->end_cell == NULL) {
        printf("Error in memory allocation of cell_range\n");
        return 2;
    }

    if(cell_range->start_cell->row <= cell_range->end_cell->row )
    {
        if(cell_range->start_cell->col <= cell_range->end_cell->col)
        {
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }
    else
    {
        return FALSE;
    }
}

void debug_print_cell(Cell *cell)
{
    printf("Cell : Row = %d, Col = %d, Value = %d", cell->row, cell->col, cell->value);
    return;
}

void debug_print_formula(Cell_Formula* formula)
{
    if (formula == NULL) {
        printf("Formula is NULL\n");
        return;
    }
    printf("EXP_TYPE : %d\n", formula->valid_exp_type);
    if (formula->valid_exp_type == VALUE)
    {
        printf("Value : %d\n", formula->value);
    }
    else if (formula->valid_exp_type == VALUE_OP_VALUE)
    {
        printf("Left Value : %d\n", formula->left_value);
        printf("Right Value : %d\n", formula->right_value);
    }
    else if (formula->valid_exp_type == FUNCT_ON_RANGE)
    {
        printf("Function : %d\n", formula->function);
        if (formula->cell_range != NULL)
        {
            printf("Cell Range (Top Left) : ");
            debug_print_cell(formula->cell_range->start_cell);
            printf("Cell Range (Bottom Right) : ");
            debug_print_cell(formula->cell_range->end_cell);
        }
    }
    
    return;
}

// ------------------------------------------------------------------------- //