// =============================================================================
// filters.h
// Shared includes and declarations for all Sudoku filter and prediction modules.
//
// The "options table" (opt_tbl, Grid3D) is the core data structure:
//   opt_tbl[row][col][0]        = number of remaining candidate symbols (0 = filled)
//   opt_tbl[row][col][1..N]     = symbol k is a candidate iff value == k (else 0)
//   opt_tbl[row][col][N+1]      = prediction-step id of last modification
//   opt_tbl[row][col][N+2..N+4] = reserved metadata slots
//
// stats[] index map (passed through most filter functions):
//   [0] initial empty-cell count     [1] key-cell predictions
//   [2] prediction errors            [3] total cells filled
//   [4] hidden singles               [5] pointing-pair / box-line removals
//   [6] naked-pair removals          [7] two-pass tally (s-c = 0) removals
//   [8] near-tight tally removals    [9] bound-set removals
// =============================================================================

#include <iostream>
#include <stdlib.h>
#include <string.h>
#include "time.h"
#include <fstream>
#include <iomanip>
#include <math.h>
#include "Grid2D.h"
#include "Grid3D.h"

using namespace std;

// ── Options-table builder ────────────────────────────────────────────────────
int build_options_table(Grid3D& opt_tbl, Grid2D& puzzle_board, int size, int block_size);

// ── Continuous filter pipeline ───────────────────────────────────────────────
int filter_symbol_cts_method(Grid3D& opt_tbl, Grid2D& puzzle_board, Grid2D& tally_buf, Grid2D& work_buf, int size, int block_size, int max_options, int pred_id, int stats[]);

// ── Individual filter techniques ─────────────────────────────────────────────
int find_hidden_symbol(Grid3D& opt_tbl, Grid2D& work_buf, int size, int block_size, int pred_id, int stats[]);
int filter_block_symbol(Grid3D& opt_tbl, Grid2D& work_buf, int size, int block_size, int pred_id, int stats[]);
int find_twin_set(Grid3D& opt_tbl, int size, int block_size, int pred_id, int stats[]);
int find_n_symbol_n_cell_tally_dsc(Grid3D& opt_tbl, Grid2D& tally_buf, Grid2D& work_buf, int size, int block_size, int max_options, int pred_id, int stats[]);
int find_n_symbol_n_cell_tally_asc(Grid3D& opt_tbl, Grid2D& tally_buf, Grid2D& work_buf, int size, int block_size, int max_options, int pred_id, int stats[]);
int filter_symbol_sets_group_method(Grid3D& opt_tbl, Grid2D& tally_buf, Grid2D& work_buf, int size, int block_size, int max_options, int pred_id, int stats[]);
int remove_invalid_options_from_bound_cells(Grid3D& opt_tbl, Grid2D& work_buf, int size, int block_size, int pred_id, int stats[]);

// ── Symbol predictor ─────────────────────────────────────────────────────────
int predict_symbol(Grid3D& opt_tbl, Grid2D& puzzle_board, Grid2D& solution_board, Grid2D& work_buf, int size, int block_size, int& pred_id, int stats[]);
