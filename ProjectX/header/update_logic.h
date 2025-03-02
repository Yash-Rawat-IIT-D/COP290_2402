#ifndef UPDATE_LOGIC_H
#define UPDATE_LOGIC_H

#include "spreadsheet.h"

// update_logic_unit:
// Updates the logic for the target cell given a new parsed formula.
//  - First, for each dependency in new_formula, it runs a DFS starting from that dependency
//    to check whether the target cell is reachable (i.e. if a cycle exists).
//  - If a cycle is found, it returns '5' (cycle detected) and aborts the update.
//  - Otherwise, it removes target from the dependency lists of cells referenced by its old formula,
//    adds target to the dependency lists of cells referenced by the new formula,
//    updates the target's formula pointer, and then triggers a topological sort–based update.
// Returns: '0' on success, '5' if a cycle is detected.
char update_logic_unit(Spread_Sheet *ss, SCell *target, CELL_FORMULA *new_formula);

#endif // UPDATE_LOGIC_H
