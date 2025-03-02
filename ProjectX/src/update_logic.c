#include "update_logic.h"
#include "dfs.h"
#include "topological_sort.h"
#include <stdio.h>

// Forward declarations for helper functions (assumed to be implemented elsewhere):
void remove_dependency_from_cell(SCell *dep, SCell *target);
void add_dependency_to_cell(SCell *dep, SCell *target);

/*
 * remove_old_dependencies:
 * Removes target from the dependency lists of cells referenced in its current (old) formula.
 */
void remove_old_dependencies(Spread_Sheet *ss, SCell *target) {
    if (target->cell_formula == NULL) return;
    char type = target->cell_formula->valid_exp_type;
    SCell *dep;
    if (type == '1') {  // Single cell reference.
        dep = get_scell_by_coordinates(ss, target->cell_formula->fvcell.cell_row,
                                            target->cell_formula->fvcell.cell_col);
        if (dep) {
            remove_dependency_from_cell(dep, target);
        }
    } else if (type == '5') {  // Arithmetic expression involving two cell references.
        dep = get_scell_by_coordinates(ss, target->cell_formula->farith_cell_cell.left_cell_row,
                                            target->cell_formula->farith_cell_cell.left_cell_col);
        if (dep) remove_dependency_from_cell(dep, target);
        dep = get_scell_by_coordinates(ss, target->cell_formula->farith_cell_cell.right_cell_row,
                                            target->cell_formula->farith_cell_cell.right_cell_col);
        if (dep) remove_dependency_from_cell(dep, target);
    } else if (type == '8') {  // Function operating on a range.
        for (int r = target->cell_formula->ffunc.start_row; r <= target->cell_formula->ffunc.end_row; r++) {
            for (int c = target->cell_formula->ffunc.start_col; c <= target->cell_formula->ffunc.end_col; c++) {
                dep = get_scell_by_coordinates(ss, r, c);
                if (dep) remove_dependency_from_cell(dep, target);
            }
        }
    }
    // For types that do not reference other cells, nothing needs to be removed.
}

/*
 * add_new_dependencies:
 * Adds target to the dependency lists of cells referenced in the new formula.
 */
void add_new_dependencies(SCell *target, CELL_FORMULA *new_formula, Spread_Sheet *ss) {
    char type = new_formula->valid_exp_type;
    SCell *dep;
    if (type == '1') {
        dep = get_scell_by_coordinates(ss, new_formula->fvcell.cell_row,
                                            new_formula->fvcell.cell_col);
        if (dep) add_dependency_to_cell(dep, target);
    } else if (type == '5') {
        dep = get_scell_by_coordinates(ss, new_formula->farith_cell_cell.left_cell_row,
                                            new_formula->farith_cell_cell.left_cell_col);
        if (dep) add_dependency_to_cell(dep, target);
        dep = get_scell_by_coordinates(ss, new_formula->farith_cell_cell.right_cell_row,
                                            new_formula->farith_cell_cell.right_cell_col);
        if (dep) add_dependency_to_cell(dep, target);
    } else if (type == '8') {
        for (int r = new_formula->ffunc.start_row; r <= new_formula->ffunc.end_row; r++) {
            for (int c = new_formula->ffunc.start_col; c <= new_formula->ffunc.end_col; c++) {
                dep = get_scell_by_coordinates(ss, r, c);
                if (dep) add_dependency_to_cell(dep, target);
            }
        }
    }
    // Other types (e.g., constants) have no dependencies.
}

/*
 * check_cycle_for_formula:
 * Checks for cycles by, for each dependency in the new formula, performing DFS
 * starting at the target cell and checking if that dependency is already present
 * in the dependency graph.
 *
 * For non-range formulas, we check each dependency (for type '1' or '5').
 * For range formulas (type '8'), we iterate over all cells in the range and
 * check if any cell is encountered during DFS from the target.
 *
 * Returns '1' if a cycle is detected, '0' otherwise.
 */
char check_cycle_for_formula(SCell *target, CELL_FORMULA *formula, Spread_Sheet *ss) {
    char type = formula->valid_exp_type;
    SCell *dep;
    if (type == '1') {  // Single cell dependency.
        dep = get_scell_by_coordinates(ss, formula->fvcell.cell_row, formula->fvcell.cell_col);
        // DFS starting at target, checking for dep.
        if (dep && dfs_cycle_check(target, dep) == '1') {
            return '1';
        }
    } else if (type == '5') {  // Arithmetic expression with two cell dependencies.
        dep = get_scell_by_coordinates(ss, formula->farith_cell_cell.left_cell_row,
                                            formula->farith_cell_cell.left_cell_col);
        if (dep && dfs_cycle_check(target, dep) == '1') {
            return '1';
        }
        dep = get_scell_by_coordinates(ss, formula->farith_cell_cell.right_cell_row,
                                            formula->farith_cell_cell.right_cell_col);
        if (dep && dfs_cycle_check(target, dep) == '1') {
            return '1';
        }
    } else if (type == '8') {  // Range function dependency.
        for (int r = formula->ffunc.start_row; r <= formula->ffunc.end_row; r++) {
            for (int c = formula->ffunc.start_col; c <= formula->ffunc.end_col; c++) {
                dep = get_scell_by_coordinates(ss, r, c);
                // DFS starting at target, checking if any cell in the range is reached.
                if (dep && dfs_cycle_check(target, dep) == '1') {
                    return '1';
                }
            }
        }
    }
    return '0';
}

/*
 * update_logic_unit:
 * Main update logic function.
 * - First, it checks for cycles using DFS (with parameters swapped so that DFS starts at target).
 * - If a cycle is found, it returns '5' (cycle detected) and aborts.
 * - Otherwise, it removes target from the dependency lists of cells referenced in its old formula,
 *   adds target to the dependency lists of cells referenced by the new formula,
 *   updates the target's formula pointer, and then triggers a topological sort–based update.
 */
char update_logic_unit(Spread_Sheet *ss, SCell *target, CELL_FORMULA *new_formula) {
    char exit_code = '0';
    
    // Check for cycle: for each dependency in the new formula, perform DFS starting at the target.
    if (check_cycle_for_formula(target, new_formula, ss) == '1') {
        printf("Cycle Found\n");
        return '5';
    }
    
    // Remove target from dependency lists of cells referenced in its old formula.
    remove_old_dependencies(ss, target);
    
    // Add target to dependency lists of cells referenced in the new formula.
    add_new_dependencies(target, new_formula, ss);
    
    // Update the target's formula pointer.
    target->cell_formula = new_formula;
    
    // Now perform a topological sort starting from the target and update cells in order.
    topological_sort_and_update(target, ss);
    
    return exit_code;
}
