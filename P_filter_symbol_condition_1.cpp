// =============================================================================
// P_filter_symbol_condition_1.cpp
// Hidden-singles filter: if a symbol appears as a candidate in exactly one cell
// of a row, column, or block, that cell must contain that symbol.
//
// Exposed entry point: find_hidden_symbol()
// Internal helpers:
//   filter_hidden_symbol_hrz  – scan each row
//   filter_hidden_symbol_vrt  – scan each column
//   filter_hidden_symbol_blk  – scan each block
// =============================================================================

#include"filters.h"
using namespace std;

// ── Forward declarations ─────────────────────────────────────────────────────
int filter_hidden_symbol_hrz(Grid3D& opt_tbl, Grid2D& work_buf, int row, int size, int block_size, int pred_id, int stats[]);
int filter_hidden_symbol_vrt(Grid3D& opt_tbl, Grid2D& work_buf, int col, int size, int block_size, int pred_id, int stats[]);
int filter_hidden_symbol_blk(Grid3D& opt_tbl, Grid2D& work_buf, int blk_row, int blk_col, int size, int block_size, int pred_id, int stats[]);

// =============================================================================
// find_hidden_symbol
// Tries hidden-single detection in rows first, then columns, then blocks.
// Returns 1 on the first hit (re-entering the main filter loop), 0 if none.
// =============================================================================
int find_hidden_symbol(Grid3D& opt_tbl, Grid2D& work_buf, int size, int block_size, int pred_id, int stats[])
{
    int row = 0, col = 0, flag = 0;

    // ── Scan rows ──────────────────────────────────────────────────────────
    while (row < size && flag == 0)
    {
        flag = filter_hidden_symbol_hrz(opt_tbl, work_buf, row, size, block_size, pred_id, stats);
        row++;
    }

    // ── Scan columns (only if no row hit found) ────────────────────────────
    if (flag == 0)
    {
        while (col < size && flag == 0)
        {
            flag = filter_hidden_symbol_vrt(opt_tbl, work_buf, col, size, block_size, pred_id, stats);
            col++;
        }
    }

    // ── Scan blocks (only if no row or column hit found) ──────────────────
    if (flag == 0)
    {
        row = 0;
        while (row < size && flag == 0)
        {
            col = 0;
            while (col < size && flag == 0)
            {
                flag = filter_hidden_symbol_blk(opt_tbl, work_buf, row, col, size, block_size, pred_id, stats);
                col = col + block_size; // advance to next block column
            }
            row = row + block_size;     // advance to next block row
        }
    }

    return flag;
}

// =============================================================================
// filter_hidden_symbol_hrz
// Counts how many empty cells in row `row` have each symbol as a candidate.
// If any symbol appears in exactly one cell, collapse that cell to that symbol.
// work_buf[sym][0] = candidate count for symbol sym across the row.
// =============================================================================
int filter_hidden_symbol_hrz(Grid3D& opt_tbl, Grid2D& work_buf, int row, int size, int block_size, int pred_id, int stats[])
{
    int i, j = 0, k, found_col, flag = 0;

    // Clear the frequency counter for every symbol
    for (i = 0; i < size + 1; i++) { work_buf(i, 0) = 0; }

    // Count occurrences of each symbol across the row's empty cells
    for (i = 0; i < size; i++)
    {
        if (opt_tbl(row, i, 0) > 0) // cell is empty
        {
            for (k = 1; k < size + 1; k++)
            {
                if (opt_tbl(row, i, k) > 0) { work_buf(k, 0)++; }
            }
        }
    }

    // Find the first symbol that occurs in exactly one cell
    k = 1;
    while (k < size + 1 && flag == 0)
    {
        if (work_buf(k, 0) == 1)
        {
            // Locate which cell holds this unique candidate
            for (i = 0; i < size; i++) { if (opt_tbl(row, i, k) > 0) { found_col = i; } }

            // Collapse the cell: clear all other candidates, keep only k
            for (i = 1; i < size + 1; i++) { opt_tbl(row, found_col, i) = 0; }
            opt_tbl(row, found_col, size + 1) = pred_id; // tag prediction step
            opt_tbl(row, found_col, k)         = k;
            opt_tbl(row, found_col, 0)         = 1;
            stats[4]++; // hidden-single count
            flag = 1;
        }
        k++;
    }
    return flag;
}

// =============================================================================
// filter_hidden_symbol_vrt
// Same as filter_hidden_symbol_hrz but scans column `col` instead of a row.
// =============================================================================
int filter_hidden_symbol_vrt(Grid3D& opt_tbl, Grid2D& work_buf, int col, int size, int block_size, int pred_id, int stats[])
{
    int i = 0, k, found_row, flag = 0;

    for (i = 0; i < size + 1; i++) { work_buf(i, 0) = 0; }

    // Count symbol occurrences down the column
    for (i = 0; i < size; i++)
    {
        if (opt_tbl(i, col, 0) > 0)
        {
            for (k = 1; k < size + 1; k++) { if (opt_tbl(i, col, k) > 0) { work_buf(k, 0)++; } }
        }
    }

    k = 1;
    while (k < size + 1 && flag == 0)
    {
        if (work_buf(k, 0) == 1)
        {
            for (i = 0; i < size; i++) { if (opt_tbl(i, col, k) > 0) { found_row = i; } }

            for (i = 1; i < size + 1; i++) { opt_tbl(found_row, col, i) = 0; }
            opt_tbl(found_row, col, size + 1) = pred_id;
            opt_tbl(found_row, col, k)         = k;
            opt_tbl(found_row, col, 0)         = 1;
            stats[4]++;
            flag = 1;
        }
        k++;
    }
    return flag;
}

// =============================================================================
// filter_hidden_symbol_blk
// Same logic applied to the block whose top-left corner is (blk_row, blk_col).
// =============================================================================
int filter_hidden_symbol_blk(Grid3D& opt_tbl, Grid2D& work_buf, int blk_row, int blk_col, int size, int block_size, int pred_id, int stats[])
{
    int i = 0, j, k, found_row, found_col, flag = 0;

    for (i = 0; i < size + 1; i++) { work_buf(i, 0) = 0; }

    // Count symbol occurrences across all cells in the block
    for (i = blk_row; i < blk_row + block_size; i++)
    {
        for (j = blk_col; j < blk_col + block_size; j++)
        {
            if (opt_tbl(i, j, 0) > 0)
            {
                for (k = 1; k < size + 1; k++) { if (opt_tbl(i, j, k) > 0) { work_buf(k, 0)++; } }
            }
        }
    }

    k = 1;
    while (k < size + 1 && flag == 0)
    {
        if (work_buf(k, 0) == 1)
        {
            // Locate the unique cell
            for (i = blk_row; i < blk_row + block_size; i++)
            {
                for (j = blk_col; j < blk_col + block_size; j++)
                {
                    if (opt_tbl(i, j, k) > 0) { found_row = i; found_col = j; }
                }
            }

            for (i = 1; i < size + 1; i++) { opt_tbl(found_row, found_col, i) = 0; }
            opt_tbl(found_row, found_col, size + 1) = pred_id;
            opt_tbl(found_row, found_col, k)         = k;
            opt_tbl(found_row, found_col, 0)         = 1;
            stats[4]++;
            flag = 1;
        }
        k++;
    }
    return flag;
}
