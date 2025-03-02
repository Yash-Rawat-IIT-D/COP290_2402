#include "dfs.h"
#include <stdlib.h>

/*
 * DFS for Topological Sort:
 * If a node hasn't been visited (flag is '0' or '1'), mark it as visited (set to '2' or '3'
 * respectively) and recursively process its dependent cells. Then push it onto the stack.
 */
void dfs_topological(SCell *node, Stack_SCell *stack)
{
    // If already visited (flag '2' or '3'), do nothing.
    if (node->visited_err_flag == '2' || node->visited_err_flag == '3')
    {
        return;
    }
    else
    {
        // Mark as visited, preserving the error bit.
        if (node->visited_err_flag == '0')
        {
            node->visited_err_flag = '2';
        }
        else if (node->visited_err_flag == '1')
        {
            node->visited_err_flag = '3';
        }

        // Process each dependency.
        for (int i = 0; i < node->dependent_scells_size; i++)
        {
            dfs_topological(node->dependent_scells[i], stack);
        }

        // Push the node onto the stack (postorder).
        push_stack(stack, node);
    }
}

/*
 * DFS for Cycle Detection:
 * This function checks whether 'target' is reachable from 'node'.
 * If node equals target, return '1' indicating a cycle.
 * Otherwise, if the node is not yet visited, mark it (adding '2' while preserving error state)
 * and recursively search its dependencies.
 * After processing, restore the node’s original flag.
 */
char dfs_cycle_check(SCell *node, SCell *target)
{
    // If we have reached the target, cycle detected.
    if (node == target)
    {
        return '1';
    }

    // If already visited in this DFS (flag '2' or '3'), skip processing.
    if (node->visited_err_flag == '2' || node->visited_err_flag == '3')
    {
        return '0';
    }

    // Save the original flag ('0' or '1').
    char original_flag = node->visited_err_flag;
    // Mark the node as visited without changing error status.
    if (original_flag == '0')
    {
        node->visited_err_flag = '2';
    }
    else if (original_flag == '1')
    {
        node->visited_err_flag = '3';
    }

    // Recurse on each dependent cell.
    for (int i = 0; i < node->dependent_scells_size; i++)
    {
        if (dfs_cycle_check(node->dependent_scells[i], target) == '1')
        {
            // Restore original flag before returning.
            if (node->visited_err_flag == '2')
                node->visited_err_flag = '0';
            else if (node->visited_err_flag == '3')
                node->visited_err_flag = '1';
            return '1';
        }
    }

    // Restore the original flag after processing.
    if (node->visited_err_flag == '2')
        node->visited_err_flag = '0';
    else if (node->visited_err_flag == '3')
        node->visited_err_flag = '1';

    return '0';
}
