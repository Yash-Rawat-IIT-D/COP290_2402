#include <stdio.h>
#include <stdlib.h>
#include "../header/node.h"
#include "../header/spreadsheet.h"

typedef struct Spread_Sheet
{
    Node ** arr;
    const int SS_ROWS;
    const int SS_COLS;

} Spread_Sheet;

// Allocate memory for the Spread_Sheet and its array of Nodes.
// Assumes that SS_ROWS and SS_COLS are already set.

void init_Spread_Sheet(Spread_Sheet *ss)
{
    ss->arr = (Node **)malloc(ss->SS_ROWS * sizeof(Node *));
    
    if (ss->arr == NULL)
    {
        printf("Memory allocation failed - Allocating Spread_Sheet Memory\n");
        exit(1); // Need to discuss this implementation with the team
    }

    for (int i = 0; i < ss->SS_ROWS; i++)
    {
        ss->arr[i] = (Node *)malloc(ss->SS_COLS * sizeof(Node));
        if (ss->arr[i] == NULL)
        {
            return;
        }
    }

    return ;
}


