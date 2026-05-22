// =============================================================================
// P_filter_symbol_condition_2.cpp
// Pointing Pairs / Box-Line Reduction filter.
//
// If all occurrences of a symbol within a block lie on a single row (or column),
// that symbol can be eliminated from the rest of that row (or column) outside
// the block.  Conversely, if within the block a symbol is confined to one row's
// worth of cells, the other rows of the block can be cleared of it.
//
// Exposed entry point: filter_block_symbol()
// Internal helpers:
//   filter_block_hrz_symbol  – horizontal (row-based) pointing pairs
//   filter_block_vrt_symbol  – vertical (column-based) pointing pairs
// =============================================================================

#include"filters.h"
using namespace std;

// ── Forward declarations ─────────────────────────────────────────────────────
int filter_block_hrz_symbol(Grid3D& opt_tbl, Grid2D& work_buf, int blk_row, int blk_col, int size, int block_size, int pred_id, int stats[]);
int filter_block_vrt_symbol(Grid3D& opt_tbl, Grid2D& work_buf, int blk_row, int blk_col, int size, int block_size, int pred_id, int stats[]);

// =============================================================================
// filter_block_symbol
// Iterates over every block on the board.
// For each block, first builds a per-symbol occurrence count (work_buf[sym][0]),
// then calls the horizontal and vertical helpers.
// Returns 1 if any candidate was eliminated; 0 otherwise.
// =============================================================================
int filter_block_symbol(Grid3D& opt_tbl, Grid2D& work_buf, int size, int block_size, int pred_id, int stats[])
{
    int i, j, k, flag = 0, f;
    int blk_row = 0, blk_col;

    while (blk_row < size && flag == 0)
    {
        blk_col = 0;
        while (blk_col < size && flag == 0)
        {
            f = 0;
            // Reset symbol frequency counter for this block
            for (i = 0; i < size + 1; i++) { work_buf(i, 0) = 0; }

            // Count how many non-zero candidate slots each symbol has in the block
            for (i = blk_row; i < blk_row + block_size; i++)
            {
                for (j = blk_col; j < blk_col + block_size; j++)
                {
                    for (k = 1; k < size + 1; k++)
                    {
                        if (opt_tbl(i, j, k) > 0) { work_buf(k, 0)++; f = 1; }
                    }
                }
            }

            if (f == 1) // at least one empty cell in this block
            {
                f = filter_block_vrt_symbol(opt_tbl, work_buf, blk_row, blk_col, size, block_size, pred_id, stats);
                if (f == 0)
                {
                    f = filter_block_hrz_symbol(opt_tbl, work_buf, blk_row, blk_col, size, block_size, pred_id, stats);
                }
                if (f == 1) { flag = 1; }
            }
            blk_col = blk_col + block_size;
        }
        blk_row = blk_row + block_size;
    }

    return flag;
}

// =============================================================================
// filter_block_hrz_symbol  (Box-Line Reduction – row direction)
// For each symbol that still has candidates in this block:
//   If all candidates lie in one row of the block (c3 == 0 for every other row),
//   remove the symbol from all cells in that row that are outside the block.
//   Additionally, if only one row of the block contains candidates at all (c1==1),
//   remove the symbol from the other cells in that same block row.
// =============================================================================
int filter_block_hrz_symbol(Grid3D& opt_tbl, Grid2D& work_buf, int blk_row, int blk_col, int size, int block_size, int pred_id, int stats[])
{
    int i, j, k, pivot_row, c1, c2, c3, flag = 0;

    for (k = 1; k < size + 1; k++)
    {
        if (work_buf(k, 0) > 0) // symbol k is present in this block
        {
            c1 = 0; // count of block rows that contain symbol k
            for (i = blk_row; i < blk_row + block_size; i++)
            {
                c2 = 0; // count of k inside the block-column on this row
                c3 = 0; // count of k outside the block-column on this row

                for (j = 0; j < size; j++)
                {
                    if (opt_tbl(i, j, k) > 0)
                    {
                        if ((j / block_size) * block_size == blk_col) { c2++; }
                        else                                           { c3++; }
                    }
                }

                if (c2 > 0)
                {
                    pivot_row = i;
                    if (c3 == 0)
                    {
                        // All occurrences of k on this row are inside the block.
                        // If this is the ONLY such row (c1 will be set to size as sentinel),
                        // we can clear k from other rows inside the block.
                        c1 = size;
                        for (i = blk_row; i < blk_row + block_size; i++)
                        {
                            for (j = blk_col; j < blk_col + block_size; j++)
                            {
                                if (i != pivot_row && opt_tbl(i, j, k) > 0)
                                {
                                    opt_tbl(i, j, k)      = 0;
                                    opt_tbl(i, j, 0)--;
                                    opt_tbl(i, j, size + 1) = pred_id;
                                    stats[5]++;
                                    flag = 1;
                                }
                            }
                        }
                    }
                    else { c1++; } // more than one row contributes
                }
            }

            // If exactly one row of the block holds symbol k's candidates,
            // remove k from cells in that row that are outside the block.
            if (c1 == 1)
            {
                for (j = 0; j < size; j++)
                {
                    if ((j / block_size) * block_size != blk_col && opt_tbl(pivot_row, j, k) > 0)
                    {
                        opt_tbl(pivot_row, j, k)      = 0;
                        opt_tbl(pivot_row, j, 0)--;
                        opt_tbl(pivot_row, j, size + 1) = pred_id;
                        stats[5]++;
                        flag = 1;
                    }
                }
            }
        }
    }
    return flag;
}

// =============================================================================
// filter_block_vrt_symbol  (Box-Line Reduction – column direction)
// Mirror of filter_block_hrz_symbol operating on columns instead of rows.
// =============================================================================
int filter_block_vrt_symbol(Grid3D& opt_tbl, Grid2D& work_buf, int blk_row, int blk_col, int size, int block_size, int pred_id, int stats[])
{
    int i, j, k, pivot_col, c1, c2, c3, flag = 0;

    for (k = 1; k < size + 1; k++)
    {
        if (work_buf(k, 0) > 0)
        {
            c1 = 0;
            for (j = blk_col; j < blk_col + block_size; j++)
            {
                c2 = 0; // candidate count inside the block-row for this column
                c3 = 0; // candidate count outside the block-row for this column

                for (i = 0; i < size; i++)
                {
                    if (opt_tbl(i, j, k) > 0)
                    {
                        if ((i / block_size) * block_size == blk_row) { c2++; }
                        else                                           { c3++; }
                    }
                }

                if (c2 > 0)
                {
                    pivot_col = j;
                    if (c3 == 0)
                    {
                        // All occurrences in this column are inside the block
                        c1 = size;
                        for (j = blk_col; j < blk_col + block_size; j++)
                        {
                            for (i = blk_row; i < blk_row + block_size; i++)
                            {
                                if (j != pivot_col && opt_tbl(i, j, k) > 0)
                                {
                                    opt_tbl(i, j, k)      = 0;
                                    opt_tbl(i, j, 0)--;
                                    opt_tbl(i, j, size + 1) = pred_id;
                                    stats[5]++;
                                    flag = 1;
                                }
                            }
                        }
                    }
                    else { c1++; }
                }
            }

            if (c1 == 1)
            {
                for (i = 0; i < size; i++)
                {
                    if ((i / block_size) * block_size != blk_row && opt_tbl(i, pivot_col, k) > 0)
                    {
                        opt_tbl(i, pivot_col, k)      = 0;
                        opt_tbl(i, pivot_col, 0)--;
                        opt_tbl(i, pivot_col, size + 1) = pred_id;
                        stats[5]++;
                        flag = 1;
                    }
                }
            }
        }
    }
    return flag;
}
