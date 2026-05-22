// =============================================================================
// P_filter_symbol_condition_3.cpp
// Naked-pair (twin-set) filter.
//
// If two cells in the same row, column, or block share exactly the same two
// candidates {s1, s2}, those two symbols can be eliminated from every other
// cell in that row/column/block.
//
// Exposed entry point: find_twin_set()
// Internal helpers:
//   filter_twin_sets_hrz  – naked pairs in the same row
//   filter_twin_sets_vrt  – naked pairs in the same column
//   filter_twin_sets_blk  – naked pairs in the same block
// =============================================================================

#include"filters.h"
using namespace std;

// ── Forward declarations ─────────────────────────────────────────────────────
int filter_twin_sets_hrz(Grid3D& opt_tbl, int cand1, int cand2, int row, int col, int size, int block_size, int pred_id, int stats[]);
int filter_twin_sets_vrt(Grid3D& opt_tbl, int cand1, int cand2, int row, int col, int size, int block_size, int pred_id, int stats[]);
int filter_twin_sets_blk(Grid3D& opt_tbl, int cand1, int cand2, int row, int col, int size, int block_size, int pred_id, int stats[]);

// =============================================================================
// find_twin_set
// Scans every cell with exactly 2 candidates and attempts to find a matching
// twin in the same row, column, or block.  Returns 1 on the first elimination.
// =============================================================================
int find_twin_set(Grid3D& opt_tbl, int size, int block_size, int pred_id, int stats[])
{
    int i = 0, j, cand1, cand2, flag = 0;

    while (i < size && flag == 0)
    {
        j = 0;
        while (j < size && flag == 0)
        {
            if (opt_tbl(i, j, 0) == 2) // cell has exactly two candidates
            {
                // Extract the two candidate symbols
                cand1 = 1; while (opt_tbl(i, j, cand1) < 1) { cand1++; }
                cand2 = cand1 + 1; while (opt_tbl(i, j, cand2) < 1) { cand2++; }

                // Try to find a twin in the same row, then column, then block
                flag = filter_twin_sets_hrz(opt_tbl, cand1, cand2, i, j, size, block_size, pred_id, stats);

                if (flag == 0)
                {
                    flag = filter_twin_sets_vrt(opt_tbl, cand1, cand2, i, j, size, block_size, pred_id, stats);
                }
                if (flag == 0)
                {
                    flag = filter_twin_sets_blk(opt_tbl, cand1, cand2, i, j, size, block_size, pred_id, stats);
                }
            }
            j++;
        }
        i++;
    }
    return flag;
}

// =============================================================================
// filter_twin_sets_hrz
// Looks for a second cell in the same row (row `row`) that contains exactly
// the same two candidates {cand1, cand2}.  If found and at least one of the
// two cells was touched at the current prediction step, removes cand1 and cand2
// from all other cells in the row.
// =============================================================================
int filter_twin_sets_hrz(Grid3D& opt_tbl, int cand1, int cand2, int row, int col, int size, int block_size, int pred_id, int stats[])
{
    int i, flag = 0, twin_col, c = 0;

    // Search for a matching twin in the same row
    for (i = 0; i < size; i++)
    {
        if (i != col && opt_tbl(row, i, 0) == 2 &&
            opt_tbl(row, i, cand1) > 0 && opt_tbl(row, i, cand2) > 0)
        {
            flag = 1; twin_col = i;
        }
    }

    // Only eliminate if at least one twin was modified at the current step
    if (flag == 1 && (opt_tbl(row, col, size + 1) == pred_id ||
                      opt_tbl(row, twin_col, size + 1) == pred_id))
    {
        flag = 0;
        for (i = 0; i < size; i++)
        {
            if (i != col && i != twin_col && opt_tbl(row, i, 0) > 0)
            {
                if (opt_tbl(row, i, cand1) > 0)
                {
                    opt_tbl(row, i, cand1)      = 0;
                    opt_tbl(row, i, 0)--;
                    opt_tbl(row, i, size + 1)   = pred_id;
                    stats[6]++;
                    flag = 1;
                }
                if (opt_tbl(row, i, cand2) > 0)
                {
                    opt_tbl(row, i, cand2)      = 0;
                    opt_tbl(row, i, 0)--;
                    opt_tbl(row, i, size + 1)   = pred_id;
                    stats[6]++;
                    flag = 1;
                }
            }
        }
    }
    else { flag = 0; }
    return flag;
}

// =============================================================================
// filter_twin_sets_vrt
// Same as filter_twin_sets_hrz but operates on column `col`.
// =============================================================================
int filter_twin_sets_vrt(Grid3D& opt_tbl, int cand1, int cand2, int row, int col, int size, int block_size, int pred_id, int stats[])
{
    int i, flag = 0, c = 0, twin_row;

    for (i = 0; i < size; i++)
    {
        if (i != row && opt_tbl(i, col, 0) == 2 &&
            opt_tbl(i, col, cand1) > 0 && opt_tbl(i, col, cand2) > 0)
        {
            flag = 1; twin_row = i;
        }
    }

    if (flag == 1 && (opt_tbl(row, col, size + 1) == pred_id ||
                      opt_tbl(twin_row, col, size + 1) == pred_id))
    {
        flag = 0;
        for (i = 0; i < size; i++)
        {
            if (i != row && i != twin_row && opt_tbl(i, col, 0) > 0)
            {
                if (opt_tbl(i, col, cand1) > 0)
                {
                    opt_tbl(i, col, cand1)      = 0;
                    opt_tbl(i, col, 0)--;
                    opt_tbl(i, col, size + 1)   = pred_id;
                    stats[6]++;
                    flag = 1;
                }
                if (opt_tbl(i, col, cand2) > 0)
                {
                    opt_tbl(i, col, cand2)      = 0;
                    opt_tbl(i, col, 0)--;
                    opt_tbl(i, col, size + 1)   = pred_id;
                    stats[6]++;
                    flag = 1;
                }
            }
        }
    }
    else { flag = 0; }
    return flag;
}

// =============================================================================
// filter_twin_sets_blk
// Same as filter_twin_sets_hrz but operates on the block containing (row, col).
// =============================================================================
int filter_twin_sets_blk(Grid3D& opt_tbl, int cand1, int cand2, int row, int col, int size, int block_size, int pred_id, int stats[])
{
    int i, j, flag = 0, twin_row, twin_col;
    int blk_row = (row / block_size) * block_size;
    int blk_col = (col / block_size) * block_size;

    for (i = blk_row; i < blk_row + block_size; i++)
    {
        for (j = blk_col; j < blk_col + block_size; j++)
        {
            if ((i != row || j != col) && opt_tbl(i, j, 0) == 2 &&
                opt_tbl(i, j, cand1) > 0 && opt_tbl(i, j, cand2) > 0)
            {
                flag = 1; twin_row = i; twin_col = j;
            }
        }
    }

    if (flag == 1 && (opt_tbl(row, col, size + 1) == pred_id ||
                      opt_tbl(twin_row, twin_col, size + 1) == pred_id))
    {
        flag = 0;
        for (i = blk_row; i < blk_row + block_size; i++)
        {
            for (j = blk_col; j < blk_col + block_size; j++)
            {
                if ((i != row || j != col) && (i != twin_row || j != twin_col) &&
                    opt_tbl(i, j, 0) > 0)
                {
                    if (opt_tbl(i, j, cand1) > 0)
                    {
                        opt_tbl(i, j, cand1)      = 0;
                        opt_tbl(i, j, 0)--;
                        opt_tbl(i, j, size + 1)   = pred_id;
                        stats[6]++;
                        flag = 1;
                    }
                    if (opt_tbl(i, j, cand2) > 0)
                    {
                        opt_tbl(i, j, cand2)      = 0;
                        opt_tbl(i, j, 0)--;
                        opt_tbl(i, j, size + 1)   = pred_id;
                        stats[6]++;
                        flag = 1;
                    }
                }
            }
        }
    }
    else { flag = 0; }
    return flag;
}
