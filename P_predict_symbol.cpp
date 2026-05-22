// =============================================================================
// P_predict_symbol.cpp
// Symbol-prediction engine for two-candidate cells.
//
// When deterministic filters are exhausted, this module selects the most
// "influential" two-candidate cell and heuristically picks one of its two
// candidates using 17 increasingly complex pattern conditions.
// The prediction is verified against solution_board and corrected if wrong
// (the solver is intentionally non-blind; it uses the known solution to recover
// from errors so that statistics remain valid).
//
// Main entry:  predict_symbol()
// Selector:    select_set()       — chooses which two-option cell to predict
// Conditions:  predict_symbol_condition_1() … predict_symbol_condition_17()
// =============================================================================

#include"filters.h"
using namespace std;

// ── Forward declarations ─────────────────────────────────────────────────────
int select_set(Grid3D& opt_tbl, int size, int pred_id, int& row, int& col);
int predict_symbol_condition_1(Grid3D& opt_tbl, Grid2D& work_buf, int cand1, int cand2, int row, int col, int size, int& predicted_sym);
int predict_symbol_condition_2(Grid3D& opt_tbl, int cand1, int cand2, int row, int col, int size, int block_size, int& predicted_sym);
int predict_symbol_condition_3(Grid3D& opt_tbl, int cand1, int cand2, int row, int col, int size, int block_size, int& predicted_sym);
int predict_symbol_condition_4(Grid3D& opt_tbl, int cand1, int cand2, int row, int col, int size, int block_size, int& predicted_sym);
int predict_symbol_condition_5(Grid3D& opt_tbl, int cand1, int cand2, int row, int col, int size, int block_size, int& predicted_sym);
int predict_symbol_condition_6(Grid3D& opt_tbl, int cand1, int cand2, int row, int col, int size, int block_size, int& predicted_sym);
int predict_symbol_condition_7(Grid3D& opt_tbl, int cand1, int cand2, int row, int col, int size, int block_size, int& predicted_sym);
int predict_symbol_condition_8(Grid3D& opt_tbl, int cand1, int cand2, int row, int col, int size, int block_size, int& predicted_sym);
int predict_symbol_condition_9(Grid3D& opt_tbl, int cand1, int cand2, int row, int col, int size, int block_size, int& predicted_sym);
int predict_symbol_condition_10(Grid3D& opt_tbl, int cand1, int cand2, int row, int col, int size, int block_size, int& predicted_sym);
int predict_symbol_condition_11(Grid3D& opt_tbl, int cand1, int cand2, int row, int col, int size, int block_size, int& predicted_sym);
int predict_symbol_condition_12(Grid3D& opt_tbl, int cand1, int cand2, int row, int col, int size, int block_size, int& predicted_sym);
int predict_symbol_condition_13(Grid3D& opt_tbl, int cand1, int cand2, int row, int col, int size, int block_size, int& predicted_sym);
int predict_symbol_condition_14(Grid3D& opt_tbl, int cand1, int cand2, int row, int col, int size, int block_size, int& predicted_sym);
int predict_symbol_condition_15(Grid3D& opt_tbl, int cand1, int cand2, int row, int col, int size, int block_size, int& predicted_sym);
int predict_symbol_condition_16(Grid3D& opt_tbl, Grid2D& puzzle_board, int cand1, int cand2, int row, int col, int size, int& predicted_sym);
int predict_symbol_condition_17(Grid3D& opt_tbl, int cand1, int cand2, int row, int col, int size, int& predicted_sym);

// =============================================================================
// predict_symbol
// 1. Selects the best two-candidate cell (row, col) via select_set().
// 2. Applies conditions 1–17 in order; the first non-zero prediction wins.
// 3. Cross-checks predicted_sym with solution_board; overrides on mismatch.
// 4. Forces the cell to a single-candidate state for the subsequent filter pass.
// Returns 1 if a prediction was made, 2 if no two-option cell was found.
// =============================================================================
int predict_symbol(Grid3D& opt_tbl, Grid2D& puzzle_board, Grid2D& solution_board, Grid2D& work_buf, int size, int block_size, int& pred_id, int stats[])
{
    int flag = 0, cand1, cand2, row = -1, col = -1, predicted_sym = 0;
    int n = 0;

    // Select the two-option cell most likely to propagate constraints
    select_set(opt_tbl, size, pred_id + 1, row, col);

    if (row > -1)
    {
        // Extract the two candidate symbols for this cell
        cand1 = 1;         while (opt_tbl(row, col, cand1) < 1) { cand1++; }
        cand2 = cand1 + 1; while (opt_tbl(row, col, cand2) < 1) { cand2++; }

        // Apply prediction conditions in ascending order of complexity
        flag = predict_symbol_condition_1(opt_tbl, work_buf, cand1, cand2, row, col, size, predicted_sym);
        if (flag == 0) { flag = predict_symbol_condition_2(opt_tbl, cand1, cand2, row, col, size, block_size, predicted_sym); }
        if (flag == 0) { flag = predict_symbol_condition_3(opt_tbl, cand1, cand2, row, col, size, block_size, predicted_sym); }
        if (flag == 0) { flag = predict_symbol_condition_4(opt_tbl, cand1, cand2, row, col, size, block_size, predicted_sym); }
        if (flag == 0) { flag = predict_symbol_condition_5(opt_tbl, cand1, cand2, row, col, size, block_size, predicted_sym); }
        if (flag == 0) { flag = predict_symbol_condition_6(opt_tbl, cand1, cand2, row, col, size, block_size, predicted_sym); }
        if (flag == 0) { flag = predict_symbol_condition_7(opt_tbl, cand1, cand2, row, col, size, block_size, predicted_sym); }
        if (flag == 0) { flag = predict_symbol_condition_8(opt_tbl, cand1, cand2, row, col, size, block_size, predicted_sym); }
        if (flag == 0) { flag = predict_symbol_condition_9(opt_tbl, cand1, cand2, row, col, size, block_size, predicted_sym); }
        if (flag == 0) { flag = predict_symbol_condition_10(opt_tbl, cand1, cand2, row, col, size, block_size, predicted_sym); }
        if (flag == 0) { flag = predict_symbol_condition_11(opt_tbl, cand1, cand2, row, col, size, block_size, predicted_sym); }
        if (flag == 0) { flag = predict_symbol_condition_12(opt_tbl, cand1, cand2, row, col, size, block_size, predicted_sym); }
        if (flag == 0) { flag = predict_symbol_condition_13(opt_tbl, cand1, cand2, row, col, size, block_size, predicted_sym); }
        if (flag == 0) { flag = predict_symbol_condition_14(opt_tbl, cand1, cand2, row, col, size, block_size, predicted_sym); }
        if (flag == 0) { flag = predict_symbol_condition_15(opt_tbl, cand1, cand2, row, col, size, block_size, predicted_sym); }
        if (flag == 0) { flag = predict_symbol_condition_16(opt_tbl, puzzle_board, cand1, cand2, row, col, size, predicted_sym); }
        if (flag == 0) { flag = predict_symbol_condition_17(opt_tbl, cand1, cand2, row, col, size, predicted_sym); }

        stats[1]++; // one more key-cell prediction attempted

        // Track errors: if predicted_sym doesn't match the known solution, count it
        if (predicted_sym != solution_board(row, col)) { stats[2]++; }

        // Always override with the correct answer (error-correction via solution)
        predicted_sym = solution_board(row, col);

        // Force the cell into a single-candidate state using the verified symbol
        opt_tbl(row, col, cand1)    = 0;
        opt_tbl(row, col, cand2)    = 0;
        opt_tbl(row, col, predicted_sym) = predicted_sym;
        opt_tbl(row, col, 0)        = 1;
        opt_tbl(row, col, size + 1) = pred_id;
        pred_id--;   // decrement step counter (cells predicted earlier have higher abs value)
        flag = 1;
    }
    else
    {
        cout << " Two symbols set not found, process will terminate " << endl;
        flag = 2; // signal: no two-option cell available → terminate
    }

    return flag;
}

// =============================================================================
// select_set
// Chooses the two-candidate cell (row, col) that maximises constraint spread:
//   Pass 1 – prefer cells tagged with the current pred_id (most recently touched)
//             and break ties by the count of related cells sharing cand1 or cand2.
//   Pass 2 – if no matching cell found, fall back to the cell with the oldest
//             (most negative) pred_id tag and highest neighbour count.
// =============================================================================
int select_set(Grid3D& opt_tbl, int size, int pred_id, int& row, int& col)
{
    int i, j, k, best_neighbour_count = 0, id = 1, neighbour_count;
    int cand1, cand2;

    row = -1; col = -1;

    // ── Pass 1: find high-influence cell with matching pred_id tag ─────────
    for (i = 0; i < size; i++)
    {
        for (j = 0; j < size; j++)
        {
            if (opt_tbl(i, j, 0) == 2 && opt_tbl(i, j, size + 1) == pred_id)
            {
                neighbour_count = 0;
                cand1 = 1;         while (opt_tbl(i, j, cand1) == 0) { cand1++; }
                cand2 = cand1 + 1; while (opt_tbl(i, j, cand2) == 0) { cand2++; }

                // Count peers (row + column) that still carry cand1 or cand2
                for (k = 0; k < size; k++)
                {
                    if (opt_tbl(i, k, 0) > 0 && opt_tbl(i, k, 0) < 4)
                    {
                        if (opt_tbl(i, k, cand1) > 0 || opt_tbl(i, k, cand2) > 0) { neighbour_count++; }
                    }
                    if (opt_tbl(k, j, 0) > 0 && opt_tbl(k, j, 0) < 4)
                    {
                        if (opt_tbl(k, j, cand1) > 0 || opt_tbl(k, j, cand2) > 0) { neighbour_count++; }
                    }
                }
                if (neighbour_count > best_neighbour_count)
                {
                    best_neighbour_count = neighbour_count;
                    row = i; col = j;
                }
            }
        }
    }

    // ── Pass 2: fallback to oldest-tagged two-option cell ─────────────────
    if (row == -1)
    {
        for (i = 0; i < size; i++)
        {
            for (j = 0; j < size; j++)
            {
                if (opt_tbl(i, j, 0) == 2)
                {
                    if (opt_tbl(i, j, size + 1) < id)
                    {
                        id = opt_tbl(i, j, size + 1);
                        row = i; col = j;
                    }
                    else if (opt_tbl(i, j, size + 1) == id)
                    {
                        // Among same-age cells, prefer highest neighbour count
                        neighbour_count = 0;
                        cand1 = 1;         while (opt_tbl(i, j, cand1) == 0) { cand1++; }
                        cand2 = cand1 + 1; while (opt_tbl(i, j, cand2) == 0) { cand2++; }

                        for (k = 0; k < size; k++)
                        {
                            if (opt_tbl(i, k, 0) > 0 && opt_tbl(i, k, 0) < 4)
                            {
                                if (opt_tbl(i, k, cand1) > 0 || opt_tbl(i, k, cand2) > 0) { neighbour_count++; }
                            }
                            if (opt_tbl(k, j, 0) > 0 && opt_tbl(k, j, 0) < 4)
                            {
                                if (opt_tbl(k, j, cand1) > 0 || opt_tbl(k, j, cand2) > 0) { neighbour_count++; }
                            }
                        }
                        if (neighbour_count > best_neighbour_count) { best_neighbour_count = neighbour_count; row = i; col = j; }
                    }
                }
            }
        }
    }

    return 0;
}

// =============================================================================
// predict_symbol_condition_1
// Frequency analysis on the row and column:
// work_buf[sym][0] = count of row cells that list sym as a candidate.
// work_buf[sym][1] = count of column cells that list sym as a candidate.
// If one symbol has a uniquely smallest frequency among {cand1, cand2}
// in the row OR column, predict it.
// =============================================================================
int predict_symbol_condition_1(Grid3D& opt_tbl, Grid2D& work_buf, int cand1, int cand2, int row, int col, int size, int& predicted_sym)
{
    int i, j;
    int h_min_count = size, v_min_count = size;  // min frequency in row / column
    int h_min_sym   = 0,    v_min_sym   = 0;     // symbol achieving the min
    int c1 = 0, c2 = 0;                          // tie counters

    predicted_sym = 0;

    // Reset work_buf frequency counters for row (col 0) and column (col 1)
    for (i = 0; i < size + 1; i++) { work_buf(i, 0) = 0; work_buf(i, 1) = 0; }

    // Count symbol occurrences across empty cells in the same row and column
    for (i = 0; i < size; i++)
    {
        if (opt_tbl(row, i, 0) > 0) // empty cell in same row
        {
            for (j = 1; j < size + 1; j++) { if (opt_tbl(row, i, j) > 0) { work_buf(j, 0)++; } }
        }
        if (opt_tbl(i, col, 0) > 0) // empty cell in same column
        {
            for (j = 1; j < size + 1; j++) { if (opt_tbl(i, col, j) > 0) { work_buf(j, 1)++; } }
        }
    }

    // Find the symbol with the strictly smallest row frequency
    for (i = 1; i < size + 1; i++)
    {
        if (work_buf(i, 0) > 0)
        {
            if      (work_buf(i, 0) < h_min_count) { h_min_count = work_buf(i, 0); h_min_sym = i; c1 = 1; }
            else if (work_buf(i, 0) == h_min_count) { c1++; }
        }
        if (work_buf(i, 1) > 0)
        {
            if      (work_buf(i, 1) < v_min_count) { v_min_count = work_buf(i, 1); v_min_sym = i; c2 = 1; }
            else if (work_buf(i, 1) == v_min_count) { c2++; }
        }
    }

    // Predict if there is a unique minimum that matches one of the two candidates
    if      (c1 == 1 && (h_min_sym == cand1 || h_min_sym == cand2)) { predicted_sym = h_min_sym; }
    else if (c2 == 1 && (v_min_sym == cand1 || v_min_sym == cand2)) { predicted_sym = v_min_sym; }

    return 0;
}

// =============================================================================
// predict_symbol_condition_2
// Neighbourhood imbalance across row, column, and block:
// Count how many other two-option cells contain cand1 vs cand2.
// If cand1 appears in very few (1) other cells and cand2 in many (>2), predict cand1.
// =============================================================================
int predict_symbol_condition_2(Grid3D& opt_tbl, int cand1, int cand2, int row, int col, int size, int block_size, int& predicted_sym)
{
    int i, j;
    int blk_row = (row / block_size) * block_size;
    int blk_col = (col / block_size) * block_size;
    int cand1_dual_count = 0, cand2_dual_count = 0; // count in other two-option cells

    predicted_sym = 0;

    // Scan row peers
    for (i = 0; i < size; i++)
    {
        if (i != col && opt_tbl(row, i, 0) == 2)
        {
            if (opt_tbl(row, i, cand1) > 0) { cand1_dual_count++; }
            if (opt_tbl(row, i, cand2) > 0) { cand2_dual_count++; }
        }
        if (i != row && opt_tbl(i, col, 0) == 2)
        {
            if (opt_tbl(i, col, cand1) > 0) { cand1_dual_count++; }
            if (opt_tbl(i, col, cand2) > 0) { cand2_dual_count++; }
        }
    }

    // Scan block peers
    for (i = blk_row; i < blk_row + block_size; i++)
    {
        for (j = blk_col; j < blk_col + block_size; j++)
        {
            if (i != row && j != col && opt_tbl(i, j, 0) == 2)
            {
                if (opt_tbl(i, j, cand1) > 0) { cand1_dual_count++; }
                if (opt_tbl(i, j, cand2) > 0) { cand2_dual_count++; }
            }
        }
    }

    // Strong imbalance → predict the rarer symbol
    if      (cand1_dual_count == 1 && cand2_dual_count > 2) { predicted_sym = cand1; }
    else if (cand2_dual_count == 1 && cand1_dual_count > 2) { predicted_sym = cand2; }

    return 0;
}

// =============================================================================
// predict_symbol_condition_3
// Row-only constraint check:
// Counts how many OTHER two-option cells in the same row also carry cand1/cand2.
// If cand1 is absent from all other row cells and cand2 is present in >1, predict cand1.
// Also handles the case where the neighbour has a third symbol with wider spread.
// =============================================================================
int predict_symbol_condition_3(Grid3D& opt_tbl, int cand1, int cand2, int row, int col, int size, int block_size, int& predicted_sym)
{
    int i, j, s3, neighbour_col;
    int blk_row = (row / block_size) * block_size;
    int blk_col = (col / block_size) * block_size;
    int cand1_row_count = 0, cand2_row_count = 0, s3_dual_count = 0;

    predicted_sym = 0;

    // Count other two-option row cells carrying cand1 / cand2
    for (i = 0; i < size; i++)
    {
        if (i != col && opt_tbl(row, i, 0) == 2)
        {
            if (opt_tbl(row, i, cand1) > 0) { cand1_row_count++; neighbour_col = i; }
            if (opt_tbl(row, i, cand2) > 0) { cand2_row_count++; neighbour_col = i; }
        }
    }

    if      (cand1_row_count == 0 && cand2_row_count > 1) { predicted_sym = cand1; }
    else if (cand2_row_count == 0 && cand1_row_count > 1) { predicted_sym = cand2; }
    else if (cand1_row_count == 0 && cand2_row_count == 1)
    {
        // One row neighbour shares cand2; find its other symbol and check its spread
        s3 = 1; while (opt_tbl(row, neighbour_col, s3) == 0 || opt_tbl(row, neighbour_col, s3) == cand2) { s3++; }
        for (i = 0; i < size; i++) { if (opt_tbl(row, i, 0) == 2 && opt_tbl(row, i, s3) > 0) { s3_dual_count++; } }
        if (s3_dual_count > 1) { predicted_sym = cand1; }
    }
    else if (cand2_row_count == 0 && cand1_row_count == 1)
    {
        s3 = 1; while (opt_tbl(row, neighbour_col, s3) == 0 || opt_tbl(row, neighbour_col, s3) == cand1) { s3++; }
        for (i = 0; i < size; i++) { if (opt_tbl(row, i, 0) == 2 && opt_tbl(row, i, s3) > 0) { s3_dual_count++; } }
        if (s3_dual_count > 1) { predicted_sym = cand2; }
    }

    return 0;
}

// =============================================================================
// predict_symbol_condition_4
// Column-only constraint check (mirror of condition 3 for columns).
// =============================================================================
int predict_symbol_condition_4(Grid3D& opt_tbl, int cand1, int cand2, int row, int col, int size, int block_size, int& predicted_sym)
{
    int i, j, s3, neighbour_row;
    int blk_row = (row / block_size) * block_size;
    int blk_col = (col / block_size) * block_size;
    int cand1_col_count = 0, cand2_col_count = 0, s3_dual_count = 0;

    predicted_sym = 0;

    for (i = 0; i < size; i++)
    {
        if (i != row && opt_tbl(i, col, 0) == 2)
        {
            if (opt_tbl(i, col, cand1) > 0) { cand1_col_count++; neighbour_row = i; }
            if (opt_tbl(i, col, cand2) > 0) { cand2_col_count++; neighbour_row = i; }
        }
    }

    if      (cand1_col_count == 0 && cand2_col_count > 1) { predicted_sym = cand1; }
    else if (cand2_col_count == 0 && cand1_col_count > 1) { predicted_sym = cand2; }
    else if (cand1_col_count == 0 && cand2_col_count == 1)
    {
        s3 = 1; while (opt_tbl(neighbour_row, col, s3) == 0 || opt_tbl(neighbour_row, col, s3) == cand2) { s3++; }
        for (i = 0; i < size; i++) { if (opt_tbl(row, i, 0) == 2 && opt_tbl(row, i, s3) > 0) { s3_dual_count++; } }
        if (s3_dual_count > 1) { predicted_sym = cand1; }
    }
    else if (cand2_col_count == 0 && cand1_col_count == 1)
    {
        s3 = 1; while (opt_tbl(neighbour_row, col, s3) == 0 || opt_tbl(neighbour_row, col, s3) == cand1) { s3++; }
        for (i = 0; i < size; i++) { if (opt_tbl(row, i, 0) == 2 && opt_tbl(row, i, s3) > 0) { s3_dual_count++; } }
        if (s3_dual_count > 1) { predicted_sym = cand2; }
    }

    return 0;
}

// =============================================================================
// predict_symbol_condition_5
// Block confinement + row/column cross-check:
// Analyses whether cand1 or cand2 is confined to fewer positions within the
// target block (vs outside) and applies row/column imbalance as a secondary tie-
// breaker.
// =============================================================================
int predict_symbol_condition_5(Grid3D& opt_tbl, int cand1, int cand2, int row, int col, int size, int block_size, int& predicted_sym)
{
    int i, j;
    int blk_row = (row / block_size) * block_size;
    int blk_col = (col / block_size) * block_size;
    // cand1/2_count = occurrences of cand inside the block but NOT in target cell
    // cand1/2_row   = occurrences in row outside the block
    int cand1_blk_others = 0, cand2_blk_others = 0;
    int cand1_row_outer  = 0, cand2_row_outer  = 0;
    int cand1_row_inner  = 0, cand2_row_inner  = 0; // inside block on this row
    int cand1_dual_count = 0, cand2_dual_count = 0; // two-option cells in block

    predicted_sym = 0;

    // Count occurrences in the block (excluding target cell)
    for (i = blk_row; i < blk_row + block_size; i++)
    {
        for (j = blk_col; j < blk_col + block_size; j++)
        {
            if (opt_tbl(i, j, 0) == 2)
            {
                if (opt_tbl(i, j, cand1) > 0) { cand1_dual_count++; }
                if (opt_tbl(i, j, cand2) > 0) { cand2_dual_count++; }
            }

            if (i != row && j != col && opt_tbl(i, j, 0) > 0)
            {
                if (opt_tbl(i, j, cand1) > 0) { cand1_blk_others++; }
                if (opt_tbl(i, j, cand2) > 0) { cand2_blk_others++; }
            }
        }
    }

    // Simple block confinement rule
    if      (cand1_blk_others == 0 && cand2_blk_others > 0) { predicted_sym = cand1; }
    else if (cand2_blk_others == 0 && cand1_blk_others > 0) { predicted_sym = cand2; }
    else
    {
        // No clear block winner — try row analysis
        cand1_row_outer = 0; cand2_row_outer = 0;
        for (i = 0; i < size; i++)
        {
            if (i / block_size != col / block_size)
            {
                if (opt_tbl(row, i, cand1) > 0) { cand1_row_outer++; }
                if (opt_tbl(row, i, cand2) > 0) { cand2_row_outer++; }
            }
            else
            {
                if (opt_tbl(row, i, cand1) > 0) { cand1_row_inner++; }
                if (opt_tbl(row, i, cand2) > 0) { cand2_row_inner++; }
            }
        }

        if (cand1_row_outer == 0 && cand2_row_outer > 0)
        {
            if (cand1_dual_count == 1 || cand2_dual_count > 1) { predicted_sym = cand1; }
            else { predicted_sym = cand2; }
        }
        else if (cand2_row_outer == 0 && cand1_row_outer > 0)
        {
            if (cand2_dual_count == 1 || cand1_dual_count > 1) { predicted_sym = cand2; }
            else { predicted_sym = cand1; }
        }
        else if (cand1_row_outer == 0 && cand2_row_outer == 0)
        {
            if      (cand1_dual_count > cand2_dual_count) { predicted_sym = cand2; }
            else if (cand2_dual_count > cand1_dual_count) { predicted_sym = cand1; }
            else if (cand1_row_inner > cand2_row_inner)   { predicted_sym = cand1; }
            else                                           { predicted_sym = cand2; }
        }

        // If still undecided, try column analysis
        if (predicted_sym == 0)
        {
            cand1_row_inner  = 0; cand2_row_inner  = 0;
            cand1_row_outer  = 0; cand2_row_outer  = 0;
            for (i = 0; i < size; i++)
            {
                if (i / block_size != row / block_size)
                {
                    if (opt_tbl(i, col, cand1) > 0) { cand1_row_outer++; }
                    if (opt_tbl(i, col, cand2) > 0) { cand2_row_outer++; }
                }
                else
                {
                    if (opt_tbl(row, i, cand1) > 0) { cand1_row_inner++; }
                    if (opt_tbl(row, i, cand2) > 0) { cand2_row_inner++; }
                }
            }

            if (cand1_row_outer == 0 && cand2_row_outer > 0)
            {
                if (cand1_dual_count == 1 || cand2_dual_count > 1) { predicted_sym = cand1; }
                else { predicted_sym = cand2; }
            }
            else if (cand2_row_outer == 0 && cand1_row_outer > 0)
            {
                if (cand2_dual_count == 1 || cand1_dual_count > 1) { predicted_sym = cand2; }
                else { predicted_sym = cand1; }
            }
            else if (cand1_row_outer == 0 && cand2_row_outer == 0)
            {
                if      (cand1_dual_count > cand2_dual_count) { predicted_sym = cand2; }
                else if (cand2_dual_count > cand1_dual_count) { predicted_sym = cand1; }
                else if (cand1_row_inner > cand2_row_inner)   { predicted_sym = cand1; }
                else                                           { predicted_sym = cand2; }
            }
        }
    }

    return 0;
}

// =============================================================================
// Conditions 6–17: increasingly specialised block/band/stack pattern checks.
// Each follows the same template:
//   - Compute candidate occurrence counts across related regions.
//   - Apply a dominance or confinement rule.
//   - Set predicted_sym to the winner, or leave 0 if inconclusive.
// See the source comments within each function for the specific heuristic.
// =============================================================================

int predict_symbol_condition_6(Grid3D& opt_tbl, int cand1, int cand2, int row, int col, int size, int block_size, int& predicted_sym)
{
    int i, j, k, end = size - block_size;
    int blk_row = (row / block_size) * block_size;
    int blk_col = (col / block_size) * block_size;
    // Track which band-column-block has the MOST candidates (row direction)
    int cand1_max_band_blk = 0, cand2_max_band_blk = 0;
    int cand1_best_band_col = 0, cand2_best_band_col = 0;
    // Track which stack-row-block has the MOST candidates (column direction)
    int cand1_best_stk_row = 0, cand2_best_stk_row = 0;
    int h_count1 = 0, h_count2 = 0; // tie count (horizontal)
    int v_count1 = 0, v_count2 = 0; // tie count (vertical)
    int c1 = 0, c2 = 0;
    predicted_sym = 0;

    // Horizontal sweep: count cand1/2 in each block along the same band-row
    for (k = 0; k < size; k = k + block_size)
    {
        c1 = 0; c2 = 0;
        for (i = blk_row; i < blk_row + block_size; i++)
        {
            for (j = k; j < k + block_size; j++)
            {
                if (opt_tbl(i, j, cand1) > 0) { c1++; }
                if (opt_tbl(i, j, cand2) > 0) { c2++; }
            }
        }

        if (c1 > cand1_max_band_blk) { cand1_max_band_blk = c1; h_count1 = 1; cand1_best_band_col = k; }
        else if (c1 == cand1_max_band_blk) { h_count1++; }

        if (c2 > cand2_max_band_blk) { cand2_max_band_blk = c2; h_count2 = 1; cand2_best_band_col = k; }
        else if (c2 == cand2_max_band_blk) { h_count2++; }
    }

    cand1_max_band_blk = 0; cand2_max_band_blk = 0;

    // Vertical sweep: count cand1/2 in each block along the same stack-column
    for (k = 0; k < size; k = k + block_size)
    {
        c1 = 0; c2 = 0;
        for (i = k; i < k + block_size; i++)
        {
            for (j = blk_col; j < blk_col + block_size; j++)
            {
                if (opt_tbl(i, j, cand1) > 0) { c1++; }
                if (opt_tbl(i, j, cand2) > 0) { c2++; }
            }
        }

        if (c1 > cand1_max_band_blk) { cand1_max_band_blk = c1; v_count1 = 1; cand1_best_stk_row = k; }
        else if (c1 == cand1_max_band_blk) { v_count1++; }

        if (c2 > cand2_max_band_blk) { cand2_max_band_blk = c2; v_count2 = 1; cand2_best_stk_row = k; }
        else if (c2 == cand2_max_band_blk) { v_count2++; }
    }

    // If cand1 is uniquely dominant in both dimensions and cand2 is not, predict cand2
    if (h_count1 == 1 && cand1_best_band_col == blk_col && v_count1 == 1 && cand1_best_stk_row == 1)
    {
        if (h_count2 > 1 || v_count2 > 1 || cand2_best_band_col != blk_col || cand2_best_stk_row != blk_row) { predicted_sym = cand2; }
    }
    else if (h_count2 == 1 && cand2_best_band_col == blk_col && v_count2 == 1 && cand2_best_stk_row == 1)
    {
        if (h_count1 > 1 || v_count1 > 1 || cand1_best_band_col != blk_col || cand1_best_stk_row != blk_row) { predicted_sym = cand1; }
    }

    return 0;
}

int predict_symbol_condition_7(Grid3D& opt_tbl, int cand1, int cand2, int row, int col, int size, int block_size, int& predicted_sym)
{
    // Row/column count == 2 means the symbol only appears in two cells of that line.
    // If BOTH row and column counts equal 2 for one candidate but not the other, predict it.
    int i, hc1 = 0, hc2 = 0, vc1 = 0, vc2 = 0;

    predicted_sym = 0;
    for (i = 0; i < size; i++)
    {
        if (opt_tbl(row, i, cand1) > 0) { hc1++; }
        if (opt_tbl(row, i, cand2) > 0) { hc2++; }
        if (opt_tbl(i, col, cand1) > 0) { vc1++; }
        if (opt_tbl(i, col, cand2) > 0) { vc2++; }
    }
    if (hc1 == 2 && vc1 == 2)
    {
        if (hc2 > 2 || vc2 > 2) { predicted_sym = cand2; }
    }
    else if (hc2 == 2 && vc2 == 2)
    {
        if (hc1 > 2 || vc1 > 2) { predicted_sym = cand1; }
    }

    return 0;
}

int predict_symbol_condition_8(Grid3D& opt_tbl, int cand1, int cand2, int row, int col, int size, int block_size, int& predicted_sym)
{
    // Block-bound column analysis: checks if cand1/2 in the row's cells are confined
    // to a single block column (outside the target block row), and whether a two-option
    // cell in that column also carries the symbol.
    int i, j, k;
    int cand1_block_bound = 0, cand2_block_bound = 0, c;
    int f1 = 0, f2 = 0, f3 = 0, f4 = 0; // flags for two-option cell presence
    int blk_row = (row / block_size) * block_size;
    int blk_col = (col / block_size) * block_size;
    predicted_sym = 0;

    // Row direction: find columns in the row where cand1 is confined to the target block row
    for (j = 0; j < size; j++)
    {
        if (opt_tbl(row, j, cand1) > 0)
        {
            c = 0;
            for (i = 0; i < size; i++)
            {
                if (i / block_size != row / block_size && opt_tbl(i, j, cand1) > 0) { c++; }
            }

            if (c == 0)
            {
                cand1_block_bound++;
                if (opt_tbl(row, j, 0) == 2) { f1 = 1; }
                else
                {
                    for (i = blk_row; i < blk_row + block_size; i++)
                    {
                        if (opt_tbl(i, j, 0) == 2 && opt_tbl(i, j, cand1) > 0) { f2 = 1; }
                    }
                }
            }
        }
        if (opt_tbl(row, j, cand2) > 0)
        {
            c = 0;
            for (i = 0; i < size; i++)
            {
                if (i / block_size != row / block_size && opt_tbl(i, j, cand2) > 0) { c++; }
            }
            if (c == 0)
            {
                cand2_block_bound++;
                if (opt_tbl(row, j, 0) == 2) { f3 = 1; }
                else
                {
                    for (i = blk_row; i < blk_row + block_size; i++)
                    {
                        if (opt_tbl(i, j, 0) == 2 && opt_tbl(i, j, cand2) > 0) { f4 = 1; }
                    }
                }
            }
        }
    }

    if (cand1_block_bound > 0 && cand2_block_bound == 0)
    {
        if (f2 > 0 && f1 == 0) { predicted_sym = cand1; }
        else { predicted_sym = cand2; }
    }
    else if (cand2_block_bound > 0 && cand1_block_bound == 0)
    {
        if (f4 > 0 && f3 == 0) { predicted_sym = cand2; }
        else { predicted_sym = cand1; }
    }
    else
    {
        // Row inconclusive — try column direction
        f1 = 0; f2 = 0; f3 = 0; f4 = 0;
        cand1_block_bound = 0; cand2_block_bound = 0;
        for (i = 0; i < size; i++)
        {
            if (opt_tbl(i, col, cand1) > 0)
            {
                c = 0;
                for (j = 0; j < size; j++)
                {
                    if (j / block_size != col / block_size && opt_tbl(i, j, cand1) > 0) { c++; }
                }
                if (c == 0)
                {
                    cand1_block_bound++;
                    if (opt_tbl(i, col, 0) == 2) { f1 = 1; }
                    else
                    {
                        for (j = blk_col; j < blk_col + block_size; j++)
                        {
                            if (opt_tbl(i, j, 0) == 2 && opt_tbl(i, j, cand1) > 0) { f2 = 1; }
                        }
                    }
                }
            }
            if (opt_tbl(i, col, cand2) > 0)
            {
                c = 0;
                for (j = 0; j < size; j++)
                {
                    if (j / block_size != col / block_size && opt_tbl(i, j, cand2) > 0) { c++; }
                }
                if (c == 0)
                {
                    cand2_block_bound++;
                    if (opt_tbl(i, col, 0) == 2) { f3 = 1; }
                    else
                    {
                        for (j = blk_col; j < blk_col + block_size; j++)
                        {
                            if (opt_tbl(i, j, 0) == 2 && opt_tbl(i, j, cand2) > 0) { f4 = 1; }
                        }
                    }
                }
            }
        }

        if (cand1_block_bound > 0 && cand2_block_bound == 0)
        {
            if (f1 == 0 && f2 > 0) { predicted_sym = cand1; }
            else { predicted_sym = cand2; }
        }
        else if (cand2_block_bound > 0 && cand1_block_bound == 0)
        {
            if (f3 == 0 && f4 == 1) { predicted_sym = cand2; }
            else { predicted_sym = cand1; }
        }
    }

    return 0;
}

int predict_symbol_condition_9(Grid3D& opt_tbl, int cand1, int cand2, int row, int col, int size, int block_size, int& predicted_sym)
{
    // Band block-bound: counts how many adjacent blocks in the same band (row-group)
    // have cand1/2 confined to a single row or column within that block.
    int i, j, k, m;
    int blk_row = (row / block_size) * block_size;
    int blk_col = (col / block_size) * block_size;
    int cand1_band_bound = 0, cand2_band_bound = 0;
    int c1, c2, c3, c4;
    predicted_sym = 0;

    for (k = 0; k < size; k = k + block_size)
    {
        c1 = 0; c2 = 0;
        for (i = blk_row; i < blk_row + block_size; i++)
        {
            c3 = 0; c4 = 0;
            for (j = k; j < k + block_size; j++)
            {
                if (opt_tbl(i, j, cand1) > 0) { c3++; }
                if (opt_tbl(i, j, cand2) > 0) { c4++; }
            }
            if (c3 > 0) { c1++; }
            if (c4 > 0) { c2++; }
        }
        if (c1 == 1) { cand1_band_bound++; }
        else
        {
            c1 = 0;
            for (j = k; j < k + block_size; j++)
            {
                c3 = 0;
                for (i = blk_row; i < blk_row + block_size; i++)
                {
                    if (opt_tbl(i, j, cand1) > 0) { c3++; }
                }
                if (c3 > 0) { c1++; }
            }
            if (c1 == 1) { cand1_band_bound++; }
        }

        if (c2 == 1) { cand2_band_bound++; }
        else
        {
            c2 = 0;
            for (j = k; j < k + block_size; j++)
            {
                c4 = 0;
                for (i = blk_row; i < blk_row + block_size; i++)
                {
                    if (opt_tbl(i, j, cand2) > 0) { c4++; }
                }
                if (c4 > 0) { c2++; }
            }
            if (c2 == 1) { cand2_band_bound++; }
        }
    }
    if      (cand1_band_bound > 0 && cand2_band_bound == 0) { predicted_sym = cand1; }
    else if (cand2_band_bound > 0 && cand1_band_bound == 0) { predicted_sym = cand2; }

    return 0;
}

int predict_symbol_condition_10(Grid3D& opt_tbl, int cand1, int cand2, int row, int col, int size, int block_size, int& predicted_sym)
{
    // Stack block-bound: mirror of condition 9, operating on the column stack instead of the row band.
    int i, j, k, m;
    int blk_row = (row / block_size) * block_size;
    int blk_col = (col / block_size) * block_size;
    int cand1_stk_bound = 0, cand2_stk_bound = 0;
    int c1, c2, c3, c4;

    predicted_sym = 0;
    for (k = 0; k < size; k = k + block_size)
    {
        c1 = 0; c2 = 0;
        for (j = blk_col; j < blk_col + block_size; j++)
        {
            c3 = 0; c4 = 0;
            for (i = k; i < k + block_size; i++)
            {
                if (opt_tbl(i, j, cand1) > 0) { c3++; }
                if (opt_tbl(i, j, cand2) > 0) { c4++; }
            }
            if (c3 > 0) { c1++; }
            if (c4 > 0) { c2++; }
        }

        if (c1 == 1) { cand1_stk_bound++; }
        else
        {
            c1 = 0;
            for (i = k; i < k + block_size; i++)
            {
                c3 = 0;
                for (j = blk_col; j < blk_col + block_size; j++)
                {
                    if (opt_tbl(i, j, cand1) > 0) { c3++; }
                }
                if (c3 > 0) { c1++; }
            }
            if (c1 == 1) { cand1_stk_bound++; }
        }

        if (c2 == 1) { cand2_stk_bound++; }
        else
        {
            c2 = 0;
            for (i = k; i < k + block_size; i++)
            {
                c4 = 0;
                for (j = blk_col; j < blk_col + block_size; j++)
                {
                    if (opt_tbl(i, j, cand2) > 0) { c4++; }
                }
                if (c4 > 0) { c2++; }
            }
            if (c2 == 1) { cand2_stk_bound++; }
        }
    }

    if      (cand1_stk_bound > 0 && cand2_stk_bound == 0) { predicted_sym = cand1; }
    else if (cand2_stk_bound > 0 && cand1_stk_bound == 0) { predicted_sym = cand2; }

    return 0;
}

int predict_symbol_condition_11(Grid3D& opt_tbl, int cand1, int cand2, int row, int col, int size, int block_size, int& predicted_sym)
{
    // Combined band/stack row-count and column-count analysis:
    // Counts: target-block occupied rows (tbrc), target-block occupied columns (tbcc),
    //         band blocks needed (hc), stack blocks needed (vc).
    // Predicts based on which candidate requires fewer blocks overall.
    int i, j, k;
    int blk_row = (row / block_size) * block_size;
    int blk_col = (col / block_size) * block_size;
    int cand1_tbrc = 0, cand2_tbrc = 0;   // rows occupied in target block
    int cand1_tbcc = 0, cand2_tbcc = 0;   // cols occupied in target block
    int cand1_hc   = 0, cand2_hc   = 0;   // band blocks containing symbol
    int cand1_vc   = 0, cand2_vc   = 0;   // stack blocks containing symbol
    int cand1_set2 = 0, cand2_set2 = 0;   // two-option cells in target block
    int c1 = 0, c2 = 0;

    predicted_sym = 0;

    // Count occupied rows in target block
    for (i = blk_row; i < blk_row + block_size; i++)
    {
        c1 = 0; c2 = 0;
        for (j = blk_col; j < blk_col + block_size; j++)
        {
            if (opt_tbl(i, j, cand1) > 0) { c1++; if (opt_tbl(i, j, 0) == 2) { cand1_set2++; } }
            if (opt_tbl(i, j, cand2) > 0) { c2++; if (opt_tbl(i, j, 0) == 2) { cand2_set2++; } }
        }
        if (c1 > 0) { cand1_tbrc++; }
        if (c2 > 0) { cand2_tbrc++; }
    }

    // Count occupied columns in target block
    for (j = blk_col; j < blk_col + block_size; j++)
    {
        c1 = 0; c2 = 0;
        for (i = blk_row; i < blk_row + block_size; i++)
        {
            if (opt_tbl(i, j, cand1) > 0) { c1++; }
            if (opt_tbl(i, j, cand2) > 0) { c2++; }
        }
        if (c1 > 0) { cand1_tbcc++; }
        if (c2 > 0) { cand2_tbcc++; }
    }

    // Count band blocks (horizontal) containing each symbol
    for (k = 0; k < size; k = k + block_size)
    {
        c1 = 0; c2 = 0;
        for (i = blk_row; i < blk_row + block_size; i++)
        {
            for (j = k; j < k + block_size; j++)
            {
                if (opt_tbl(i, j, cand1) > 0) { c1++; }
                if (opt_tbl(i, j, cand2) > 0) { c2++; }
            }
        }
        if (c1 > 0) { cand1_hc++; }
        if (c2 > 0) { cand2_hc++; }
    }

    // Count stack blocks (vertical) containing each symbol
    for (k = 0; k < size; k = k + block_size)
    {
        c1 = 0; c2 = 0;
        for (j = blk_col; j < blk_col + block_size; j++)
        {
            for (i = k; i < k + block_size; i++)
            {
                if (opt_tbl(i, j, cand1) > 0) { c1++; }
                if (opt_tbl(i, j, cand2) > 0) { c2++; }
            }
        }
        if (c1 > 0) { cand1_vc++; }
        if (c2 > 0) { cand2_vc++; }
    }

    // Decision rules (in priority order)
    if      (cand1_tbrc == 2 && cand1_hc == 2 && cand2_hc > 2 && cand2_vc > 2) { predicted_sym = cand1; }
    else if (cand1_tbcc == 2 && cand1_vc == 2 && cand2_hc > 2 && cand2_vc > 2) { predicted_sym = cand1; }
    else if (cand2_tbrc == 2 && cand2_hc == 2 && cand1_hc > 2 && cand1_vc > 2) { predicted_sym = cand2; }
    else if (cand2_tbcc == 2 && cand2_vc == 2 && cand1_hc > 2 && cand1_vc > 2) { predicted_sym = cand2; }
    else if (cand1_tbrc < cand1_hc && cand1_tbcc < cand1_vc  && (cand2_tbrc == cand2_hc || cand2_tbcc == cand2_vc)) { predicted_sym = cand1; }
    else if (cand2_tbrc < cand2_hc && cand2_tbcc < cand2_vc  && (cand1_tbrc == cand1_hc || cand1_tbcc == cand1_vc)) { predicted_sym = cand2; }
    else if ((cand1_hc < cand2_hc || cand1_vc < cand2_vc) && cand2_set2 > 1 && cand1_set2 == 1) { predicted_sym = cand1; }
    else if ((cand2_hc < cand1_hc || cand2_vc < cand1_vc) && cand1_set2 > 1 && cand2_set2 == 1) { predicted_sym = cand2; }
    else if ((cand1_tbrc < cand1_hc || cand1_tbcc < cand1_vc) && cand2_tbrc == cand2_hc && cand2_tbcc == cand2_vc) { predicted_sym = cand1; }
    else if ((cand2_tbrc < cand2_hc || cand2_tbcc < cand2_vc) && cand1_tbrc == cand1_hc && cand1_tbcc == cand1_vc) { predicted_sym = cand2; }

    return 0;
}

int predict_symbol_condition_12(Grid3D& opt_tbl, int cand1, int cand2, int row, int col, int size, int block_size, int& predicted_sym)
{
    // Compares target-block candidate count against band/stack block counts.
    // If more adjacent blocks exceed the target count than fall below it for
    // one candidate (and the reverse for the other), predict accordingly.
    int i, j, k;
    int blk_row = (row / block_size) * block_size;
    int blk_col = (col / block_size) * block_size;
    int cand1_tbc = 0, cand2_tbc = 0;              // target block counts
    int cand1_max_blks = 0, cand2_max_blks = 0;    // blocks exceeding target count
    int cand1_min_blks = 0, cand2_min_blks = 0;    // blocks <= target count
    int c1, c2;

    predicted_sym = 0;

    // Count candidates in target block
    for (i = blk_row; i < blk_row + block_size; i++)
    {
        for (j = blk_col; j < blk_col + block_size; j++)
        {
            if (opt_tbl(i, j, cand1) > 0) { cand1_tbc++; }
            if (opt_tbl(i, j, cand2) > 0) { cand2_tbc++; }
        }
    }

    // Compare each band/stack block against the target block count
    for (k = 0; k < size; k = k + block_size)
    {
        c1 = 0; c2 = 0;
        for (i = blk_row; i < blk_row + block_size; i++)
        {
            for (j = k; j < k + block_size; j++)
            {
                if (opt_tbl(i, j, cand1) > 0) { c1++; }
                if (opt_tbl(i, j, cand2) > 0) { c2++; }
            }
        }
        if (c1 > cand1_tbc) { cand1_max_blks++; }
        else if (c1 > 0 && c1 <= cand1_tbc) { cand1_min_blks++; }

        if (c2 > cand2_tbc) { cand2_max_blks++; }
        else if (c2 > 0 && c2 <= cand2_tbc) { cand2_min_blks++; }

        c1 = 0; c2 = 0;
        for (j = blk_col; j < blk_col + block_size; j++)
        {
            for (i = k; i < k + block_size; i++)
            {
                if (opt_tbl(i, j, cand1) > 0) { c1++; }
                if (opt_tbl(i, j, cand2) > 0) { c2++; }
            }
        }

        if (c1 > cand1_tbc) { cand1_max_blks++; }
        else if (c1 > 0 && c1 <= cand1_tbc) { cand1_min_blks++; }

        if (c2 > cand2_tbc) { cand2_max_blks++; }
        else if (c2 > 0 && c2 <= cand2_tbc) { cand2_min_blks++; }
    }

    if      (cand1_max_blks > cand1_min_blks && cand2_max_blks <= cand2_min_blks) { predicted_sym = cand1; }
    else if (cand2_max_blks > cand2_min_blks && cand1_max_blks <= cand1_min_blks) { predicted_sym = cand2; }
    else if (cand1_max_blks < cand1_min_blks && cand2_max_blks >= cand2_min_blks) { predicted_sym = cand2; }
    else if (cand2_max_blks < cand2_min_blks && cand1_max_blks >= cand1_min_blks) { predicted_sym = cand1; }

    return 0;
}

int predict_symbol_condition_13(Grid3D& opt_tbl, int cand1, int cand2, int row, int col, int size, int block_size, int& predicted_sym)
{
    // Asymmetry between horizontal and vertical block counts.
    // If one candidate needs different numbers of band vs stack blocks (|d| > 0)
    // and the other is symmetric (d == 0), predict the asymmetric one.
    int i, j, k;
    int blk_row = (row / block_size) * block_size;
    int blk_col = (col / block_size) * block_size;
    int h_cand1_bc = 0, h_cand2_bc = 0; // band blocks needed
    int v_cand1_bc = 0, v_cand2_bc = 0; // stack blocks needed
    int d1 = 0, d2 = 0;                 // |band - stack| difference
    int c1, c2;

    predicted_sym = 0;

    for (k = 0; k < size; k = k + block_size)
    {
        c1 = 0; c2 = 0;
        for (i = blk_row; i < blk_row + block_size; i++)
        {
            for (j = k; j < k + block_size; j++)
            {
                if (opt_tbl(i, j, cand1) > 0) { c1++; }
                if (opt_tbl(i, j, cand2) > 0) { c2++; }
            }
        }
        if (c1 > 0) { h_cand1_bc++; }
        if (c2 > 0) { h_cand2_bc++; }

        c1 = 0; c2 = 0;
        for (j = blk_col; j < blk_col + block_size; j++)
        {
            for (i = k; i < k + block_size; i++)
            {
                if (opt_tbl(i, j, cand1) > 0) { c1++; }
                if (opt_tbl(i, j, cand2) > 0) { c2++; }
            }
        }
        if (c1 > 0) { v_cand1_bc++; }
        if (c2 > 0) { v_cand2_bc++; }
    }

    d1 = (h_cand1_bc > v_cand1_bc) ? h_cand1_bc - v_cand1_bc : v_cand1_bc - h_cand1_bc;
    d2 = (h_cand2_bc > v_cand2_bc) ? h_cand2_bc - v_cand2_bc : v_cand2_bc - h_cand2_bc;

    if      (d1 > 0 && d2 == 0) { predicted_sym = cand1; }
    else if (d2 > 0 && d1 == 0) { predicted_sym = cand2; }

    return 0;
}

int predict_symbol_condition_14(Grid3D& opt_tbl, int cand1, int cand2, int row, int col, int size, int block_size, int& predicted_sym)
{
    // Total band + stack block count comparison.
    // The symbol appearing in MORE blocks overall is harder to place → predict it.
    int i, j, k;
    int blk_row = (row / block_size) * block_size;
    int blk_col = (col / block_size) * block_size;
    int cand1_total_blks = 0, cand2_total_blks = 0;
    int c1, c2;

    predicted_sym = 0;

    for (k = 0; k < size; k = k + block_size)
    {
        c1 = 0; c2 = 0;
        for (i = blk_row; i < blk_row + block_size; i++)
        {
            for (j = k; j < k + block_size; j++)
            {
                if (opt_tbl(i, j, cand1) > 0) { c1++; }
                if (opt_tbl(i, j, cand2) > 0) { c2++; }
            }
        }
        if (c1 > 0) { cand1_total_blks++; }
        if (c2 > 0) { cand2_total_blks++; }

        c1 = 0; c2 = 0;
        for (j = blk_col; j < blk_col + block_size; j++)
        {
            for (i = k; i < k + block_size; i++)
            {
                if (opt_tbl(i, j, cand1) > 0) { c1++; }
                if (opt_tbl(i, j, cand2) > 0) { c2++; }
            }
        }
        if (c1 > 0) { cand1_total_blks++; }
        if (c2 > 0) { cand2_total_blks++; }
    }

    if      (cand1_total_blks > cand2_total_blks) { predicted_sym = cand1; }
    else if (cand2_total_blks > cand1_total_blks) { predicted_sym = cand2; }

    return 0;
}

int predict_symbol_condition_15(Grid3D& opt_tbl, int cand1, int cand2, int row, int col, int size, int block_size, int& predicted_sym)
{
    // Target-block candidate count: the symbol appearing in FEWER cells of the
    // target block is the harder placement → predict it.
    int i, j;
    int blk_row = (row / block_size) * block_size;
    int blk_col = (col / block_size) * block_size;
    int cand1_tbc = 0, cand2_tbc = 0;

    predicted_sym = 0;
    for (i = blk_row; i < blk_row + block_size; i++)
    {
        for (j = blk_col; j < blk_col + block_size; j++)
        {
            if (opt_tbl(i, j, cand1) > 0) { cand1_tbc++; }
            if (opt_tbl(i, j, cand2) > 0) { cand2_tbc++; }
        }
    }

    if      (cand1_tbc < cand2_tbc) { predicted_sym = cand1; }
    else if (cand2_tbc < cand1_tbc) { predicted_sym = cand2; }

    return 0;
}

int predict_symbol_condition_16(Grid3D& opt_tbl, Grid2D& puzzle_board, int cand1, int cand2, int row, int col, int size, int& predicted_sym)
{
    // Global frequency in puzzle_board: whichever symbol has been placed MORE
    // times already is less needed elsewhere → predict the less-placed one.
    int i, j, cand1_placed = 0, cand2_placed = 0;

    predicted_sym = 0;
    for (i = 0; i < size; i++)
    {
        for (j = 0; j < size; j++)
        {
            if      (puzzle_board(i, j) == cand1) { cand1_placed++; }
            else if (puzzle_board(i, j) == cand2) { cand2_placed++; }
        }
    }

    if      (cand1_placed > cand2_placed) { predicted_sym = cand2; }
    else if (cand2_placed > cand1_placed) { predicted_sym = cand1; }

    return 0;
}

int predict_symbol_condition_17(Grid3D& opt_tbl, int cand1, int cand2, int row, int col, int size, int& predicted_sym)
{
    // Fallback: count how many small-set (< 4) cells in the target row AND column
    // carry each candidate.  The symbol with more such neighbours is predicted.
    int i, cand1_nbr = 0, cand2_nbr = 0;

    predicted_sym = 0;
    for (i = 0; i < size; i++)
    {
        if (opt_tbl(row, i, 0) < 4) // small candidate set in row
        {
            if (opt_tbl(row, i, cand1) > 0) { cand1_nbr++; }
            if (opt_tbl(row, i, cand2) > 0) { cand2_nbr++; }
        }
        if (opt_tbl(i, col, 0) < 4) // small candidate set in column
        {
            if (opt_tbl(i, col, cand1) > 0) { cand1_nbr++; }
            if (opt_tbl(i, col, cand2) > 0) { cand2_nbr++; }
        }
    }

    // More neighbours → predict that symbol (or default to cand1 on tie)
    if (cand1_nbr > cand2_nbr) { predicted_sym = cand2; }
    else                        { predicted_sym = cand1; }

    return 0;
}
