#ifndef TOPOLOGICAL_SORT_H
#define TOPOLOGICAL_SORT_H

#include "spreadsheet.h"
#include "stack.h"

// Performs a topological sort on the dependency subgraph starting at 'start',
// then processes each node (e.g. updating its value) in sorted order.
// The visited portion of each node's flag is reset (i.e. '2' becomes '0' and '3' becomes '1')
// as nodes are popped off the stack.
// It is assumed that before calling this function, all nodes have their visited_err_flag set to
// either '0' (no error) or '1' (error already set).
//
// Note: The function update_cell_value(ss, node) is assumed to exist (or you can inline your update logic).
void topological_sort_and_update(SCell *start, Spread_Sheet *ss);

#endif // TOPOLOGICAL_SORT_H
