// =============================================================================
// Build_options_table.cpp
// Populates the 3-D options table (opt_tbl) for a given puzzle board.
//
// opt_tbl[row][col][0]      = number of candidate symbols still available
// opt_tbl[row][col][1..N]   = symbol k is a candidate iff value == k (else 0)
// opt_tbl[row][col][size+1..size+4] = metadata slots (prediction id, flags …)
//
// For each empty cell the function eliminates symbols already used in the same
// row, column, and block, then stores the remaining candidate count in [0].
// Filled cells get all slots zeroed out.
// =============================================================================

#include"filters.h"

using namespace std;

// Forward declaration of the per-cell helper
int build_options_table_fx(Grid3D& opt_tbl, Grid2D& puzzle_board, int row, int col, int size, int block_size);

// =============================================================================
// build_options_table
// Iterates over every cell:
//   - Empty cells  → delegates to build_options_table_fx to compute candidates.
//   - Filled cells → zeros out all candidate slots (no options needed).
// Metadata slots [size+1]..[size+4] are always cleared.
// =============================================================================
int build_options_table(Grid3D& opt_tbl, Grid2D& puzzle_board, int size, int block_size)
{
    int i = 0, j = 0, k = 0, id;

    for (i = 0; i < size; i++)
    {
        for (j = 0; j < size; j++)
        {
            if (puzzle_board(i, j) == 0)
            {
                // Empty cell: compute valid candidate symbols
                id = 0;
                build_options_table_fx(opt_tbl, puzzle_board, i, j, size, block_size);
            }
            else
            {
                // Filled cell: zero all candidate slots
                for (k = 0; k < size + 1; k++) { opt_tbl(i, j, k) = 0; }
            }

            // Clear metadata slots for every cell
            opt_tbl(i, j, size + 1) = 0;
            opt_tbl(i, j, size + 2) = 0;
            opt_tbl(i, j, size + 3) = 0;
            opt_tbl(i, j, size + 4) = 0;
        }
    }
    return 0;
}

// =============================================================================
// build_options_table_fx
// Computes candidate symbols for the single empty cell at (row, col).
//
// Algorithm:
//   1. Initialise slots [0..size] so that slot k holds value k.
//   2. Scan the cell's row, column, and block; for each filled neighbour with
//      symbol s, set opt_tbl[row][col][s] = 0 to eliminate s.
//   3. Count remaining non-zero slots (1..size) and store in [0].
// =============================================================================
int build_options_table_fx(Grid3D& opt_tbl, Grid2D& puzzle_board, int row, int col, int size, int block_size)
{
    int i, j, s;
    // block_row / block_col = top-left corner of the block containing (row, col)
    int block_row = (row / block_size) * block_size;
    int block_col = (col / block_size) * block_size;

    // Step 1: pre-fill slots with their own index (all symbols are candidates)
    for (i = 0; i < size + 1; i++) { opt_tbl(row, col, i) = i; }

    // Step 2a: eliminate symbols used in the same column
    for (i = 0; i < size; i++)
    {
        if (puzzle_board(i, col) > 0) { s = puzzle_board(i, col); opt_tbl(row, col, s) = 0; }
    }

    // Step 2b: eliminate symbols used in the same row
    for (j = 0; j < size; j++)
    {
        if (puzzle_board(row, j) > 0) { s = puzzle_board(row, j); opt_tbl(row, col, s) = 0; }
    }

    // Step 2c: eliminate symbols used in the same block
    for (i = block_row; i < block_row + block_size; i++)
    {
        for (j = block_col; j < block_col + block_size; j++)
        {
            if (puzzle_board(i, j) > 0) { s = puzzle_board(i, j); opt_tbl(row, col, s) = 0; }
        }
    }

    // Step 3: count remaining candidates and store in slot [0]
    j = 0;
    for (i = 1; i < size + 1; i++)
    {
        if (opt_tbl(row, col, i) > 0) { j++; }
    }

    opt_tbl(row, col, 0) = j;
    return 0;
}
