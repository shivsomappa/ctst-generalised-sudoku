// =============================================================================
// gsv.h
// Top-level includes and declarations for the GSV (Generate, Solve, Verify)
// application.  Included by Sudoku_Main.cpp and the Create_sudoku_M*.cpp files.
// =============================================================================

#include "iostream"
#include <stdlib.h>
#include <string.h>
#include "time.h"
#include <fstream>
#include <math.h>
#include "Grid2D.h"
#include "Grid3D.h"

using namespace std;

// ── Display helpers ──────────────────────────────────────────────────────────
int display_table(Grid2D& puzzle_board, int size);
int display_options_table(Grid3D& opt_tbl, int size, int add);

// ── File I/O ─────────────────────────────────────────────────────────────────
void write_file(Grid2D& puzzle_board, int size, char* filename);

// ── Puzzle generation methods (M1–M4) ────────────────────────────────────────
// Each method fills puzzle_board (clue cells) and solution_board with a valid
// Sudoku configuration using a different key-cell placement strategy.
int m1_GENERAT_SUDOKU_METHOD(Grid3D& opt_tbl, Grid2D& puzzle_board, Grid2D& solution_board, int size, int block_size);
int m2_GENERAT_SUDOKU_METHOD(Grid3D& opt_tbl, Grid2D& puzzle_board, Grid2D& solution_board, int size, int block_size);
int m3_GENERAT_SUDOKU_METHOD(Grid3D& opt_tbl, Grid2D& puzzle_board, Grid2D& solution_board, int size, int block_size);
int m4_GENERAT_SUDOKU_METHOD(Grid3D& opt_tbl, Grid2D& puzzle_board, Grid2D& solution_board, int size, int block_size);

// ── Multiple-solution display ────────────────────────────────────────────────
int display_multiple_solution_list(Grid3D& opt_tbl, int size);
