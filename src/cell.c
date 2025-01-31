// Cell Struct Definition and Functions

#include "../header/cell.h"  // Include the header file for declaration

// ------------------------------------------------------------------------- //

void init_cell(Cell *cell, int row, int col, double value)
{
    cell->row = row;
    cell->col = col;
    cell->value = value;
    cell->cell_range_left = NULL;
    cell->cell_range_right = NULL;
    return;
}

void set_cell_value(Cell *cell, double value)
{
    cell->value = value;
}

double get_cell_value(Cell *cell)
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

Cell_Range * get_cell_left(Cell *cell)
{
    return cell->cell_range_left;
}

Cell_Range * get_cell_right(Cell *cell)
{
    return cell->cell_range_right;
}

int is_valid_cell_range(Cell_Range *cell_range)
{

    if (cell_range == NULL || cell_range->start_cell == NULL || cell_range->end_cell == NULL) {
        printf("Error in memory allocation of cell_range\n");
        return 2;
    }

    if(cell_range->start_cell->row <= cell_range->end_cell->row )
    {
        if(cell_range->start_cell->col <= cell_range->end_cell->col)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        return 0;
    }
}

// ------------------------------------------------------------------------- //