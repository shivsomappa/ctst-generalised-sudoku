// =============================================================================
// P_filter_symbol_cts_methods.cpp
// Continuous (iterative) deterministic filtering pipeline.
//
// filter_symbol_cts_method() loops over a chain of filter techniques until no
// further progress is made (all filters return 0 in one pass).  Each filter
// returns 1 if it eliminated at least one candidate, 0 otherwise.
//
// Filter execution order (weakest → strongest to minimise wasted work):
//   1. insert_symbol_into_table  – naked singles (one candidate left)
//   2. find_hidden_symbol        – hidden singles (symbol appears in only one cell)
//   3. filter_block_symbol       – pointing pairs / box-line reduction
//   4. find_twin_set             – naked pairs
//   5. find_n_symbol_n_cell_tally_dsc – N-symbol N-cell tally (descending)
//   6. find_n_symbol_n_cell_tally_asc – N-symbol N-cell tally (ascending)
//   7. filter_symbol_sets_group_method – group method
//   8. remove_invalid_options_from_bound_cells – bound-set filter
// =============================================================================

#include"filters.h"
using namespace std;

// ── Forward declarations ─────────────────────────────────────────────────────
int insert_symbol_into_table(Grid3D& opt_tbl, Grid2D& puzzle_board, int size, int block_size, int stats[]);
int insert_symbol_into_table_fx(Grid3D& opt_tbl, int sym, int row, int col, int size, int block_size);

// =============================================================================
// filter_symbol_cts_method
// Runs the filter chain repeatedly until a full pass produces no progress.
// pred_id – the prediction-step identifier; used to tag which step changed a cell.
// stats[] – accumulated statistics (candidate-removal counts per technique).
// =============================================================================
int filter_symbol_cts_method(Grid3D& opt_tbl, Grid2D& puzzle_board, Grid2D& tally_buf, Grid2D& work_buf, int size, int block_size, int max_options, int pred_id, int stats[])
{
    int flag = 1; // non-zero while any filter made progress this pass

    while (flag == 1)
    {
        // Try each filter in order; stop as soon as one makes progress
        flag = insert_symbol_into_table(opt_tbl, puzzle_board, size, block_size, stats);
        if (flag == 0) { flag = find_hidden_symbol(opt_tbl, work_buf, size, block_size, pred_id, stats); }
        if (flag == 0) { filter_block_symbol(opt_tbl, work_buf, size, block_size, pred_id, stats); }
        if (flag == 0) { flag = find_twin_set(opt_tbl, size, block_size, pred_id, stats); }
        if (flag == 0) { flag = find_n_symbol_n_cell_tally_dsc(opt_tbl, tally_buf, work_buf, size, block_size, max_options, pred_id, stats); }
        if (flag == 0) { flag = find_n_symbol_n_cell_tally_asc(opt_tbl, tally_buf, work_buf, size, block_size, max_options, pred_id, stats); }
        if (flag == 0) { flag = filter_symbol_sets_group_method(opt_tbl, tally_buf, work_buf, size, block_size, max_options, pred_id, stats); }
        if (flag == 0) { flag = remove_invalid_options_from_bound_cells(opt_tbl, work_buf, size, block_size, pred_id, stats); }

        // Progress display during the initial (pred_id == 0) filtering stage
        if (pred_id == 0)
        {
            cout << "Celled filled by CTST method in initial stage            " << stats[3] << "  " << '\r';
            cout.flush();
        }
    }

    return flag;
}

// =============================================================================
// insert_symbol_into_table
// Naked-single pass: any empty cell with exactly one remaining candidate gets
// that symbol placed immediately, then that symbol is propagated (removed from
// peers) via insert_symbol_into_table_fx.
// Loops until no naked single remains in this pass.
// Returns 1 if at least one cell was filled, 0 otherwise.
// =============================================================================
int insert_symbol_into_table(Grid3D& opt_tbl, Grid2D& puzzle_board, int size, int block_size, int stats[])
{
    int i = 0, j, sym, flag = 1;

    while (flag == 1)
    {
        flag = 0; i = 0;
        for (i = 0; i < size; i++)
        {
            for (j = 0; j < size; j++)
            {
                if (opt_tbl(i, j, 0) == 1) // exactly one candidate left
                {
                    // Find that lone candidate
                    sym = 1;
                    while (opt_tbl(i, j, sym) == 0) { sym++; }

                    // Place symbol and clear the cell's option slots
                    puzzle_board(i, j)      = sym;
                    opt_tbl(i, j, sym)      = 0;
                    opt_tbl(i, j, 0)        = 0;
                    stats[3]++;              // increment total cells-filled count
                    flag = 1;                // mark progress

                    // Propagate: remove sym from row, column, and block peers
                    insert_symbol_into_table_fx(opt_tbl, sym, i, j, size, block_size);
                }
            }
        }
    }

    return 0;
}

// =============================================================================
// insert_symbol_into_table_fx
// Propagates a newly placed symbol (sym) at (row, col) by removing it from the
// candidate lists of all peers in the same row, column, and block.
// Also tags each modified cell with the current prediction step (pred_id).
// =============================================================================
int insert_symbol_into_table_fx(Grid3D& opt_tbl, int sym, int row, int col, int size, int block_size)
{
    int i = 0, j = 0;
    int block_row = 0, block_col = 0;
    int pred_id = opt_tbl(row, col, size + 1); // inherit prediction tag from placed cell

    // Remove sym from all cells in the same column
    for (i = 0; i < size; i++)
    {
        if (opt_tbl(i, col, sym) > 0)
        {
            opt_tbl(i, col, sym)      = 0;
            opt_tbl(i, col, 0)--;
            opt_tbl(i, col, size + 1) = pred_id;
        }
    }

    // Remove sym from all cells in the same row
    for (i = 0; i < size; i++)
    {
        if (opt_tbl(row, i, sym) > 0)
        {
            opt_tbl(row, i, sym)      = 0;
            opt_tbl(row, i, 0)--;
            opt_tbl(row, i, size + 1) = pred_id;
        }
    }

    // Remove sym from all cells in the same block
    block_row = (row / block_size) * block_size;
    block_col = (col / block_size) * block_size;

    for (i = block_row; i < block_row + block_size; i++)
    {
        for (j = block_col; j < block_col + block_size; j++)
        {
            if (opt_tbl(i, j, sym) > 0)
            {
                opt_tbl(i, j, sym)      = 0;
                opt_tbl(i, j, 0)--;
                opt_tbl(i, j, size + 1) = pred_id;
            }
        }
    }
    return 0;
}
