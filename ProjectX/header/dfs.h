#ifndef DFS_H
#define DFS_H

#include "spreadsheet.h"
#include "stack.h"

/*
 * dfs_topological:
 * Recursively processes the dependency graph for topological sorting.
 * Uses visited_err_flag as follows:
 *   - If flag is '0' or '1' (not visited), mark as visited:
 *         '0' becomes '2'; '1' becomes '3'.
 *   - If flag is already '2' or '3', the node is skipped.
 * Once all dependencies are processed, the node is pushed onto the stack.
 */
void dfs_topological(SCell *node, Stack_SCell *stack);

/*
 * dfs_cycle_check:
 * Recursively checks whether the 'target' SCell is reachable from 'node'.
 * Uses the same visited_err_flag convention:
 *   - Before processing a node, if its flag is '0' or '1', mark it as visited
 *     (i.e. '0' becomes '2' and '1' becomes '3') temporarily.
 *   - After processing, restore the node’s flag (i.e. '2' becomes '0' and '3' becomes '1').
 * Returns '1' if the target is found (cycle detected), and '0' otherwise.
 */
char dfs_cycle_check(SCell *node, SCell *target);

#endif // DFS_H
