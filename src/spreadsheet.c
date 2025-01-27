#include <stdio.h>
#include <stdlib.h>
#include "../header/node.h"
#include "../header/spreadsheet.h"

// Allocate memory for the Spread_Sheet and its array of Nodes.
// Assumes that SS_ROWS and SS_COLS are already set.

void init_spread_sheet(Spread_Sheet *ss)
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
            printf("Memory allocation failed - Allocating Spread_Sheet Memory\n");
            exit(1); // Need to discuss this implementation with the team
        }
        else
        {
            for (int j = 0; j < ss->SS_COLS; j++)
            {
                init_node(&(ss->arr[i][j]), i, j, 0.0);
                printf("Initialising %d %d\n",i,j);
            }
        }
    }
    

    return;
}
