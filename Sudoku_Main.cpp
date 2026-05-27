// =============================================================================
// Sudoku_Main.cpp
// Entry point for the Sudoku GSV (Generate, Solve, Verify) application.
//
// Flow:
//   1. User selects board rank (block_size) and generation method (gen_method).
//   2. The chosen method fills puzzle_board (clue cells) and solution_board.
//   3. The puzzle is then solved by SOLVE_SUDOKU to collect statistics.
//   4. Results are written to puzzle.txt and solution.txt.
// =============================================================================

#include"gsv.h"
#include"filters.h"
#include<locale.h>

using namespace std;

// ── Forward declarations ─────────────────────────────────────────────────────
int char2int(int num);
int int2char(int num);

int vr(Grid2D& puzzle_board, Grid2D& solution_board, int size);

int Read_file(char file_name[200], Grid2D& table, int size);

int elapsed_time(time_t start, time_t end);

int Find_output_file_size(Grid2D& table, int size);
int Write_table_to_text_buffer(Grid2D& table, char* out, int size);
void write_file(Grid2D& table, int size, char* filename);

int check_process_halt(Grid3D& opt_tbl, Grid2D& puzzle_board, Grid2D& solution_board, Grid2D& stats_board, int size, int& set2, int& set3);

int print_message_1(int& i);
int print_message_2(const Grid3D& opt_tbl, const Grid2D& stats_board, const Grid2D& solution_board, int size, int set2, int stats[], time_t start, time_t end);
int display_table(Grid2D& table, int size);
int display_p_OT(const Grid3D& opt_tbl, int size, int add);
int display_multiple_solution_list(const Grid3D& opt_tbl, int size);

int SOLVE_SUDOKU(Grid3D& opt_tbl, Grid2D& puzzle_board, Grid2D& solution_board, Grid2D& stats_board, int size, int block_size, int gen_method);
int Set_info(Grid3D& opt_tbl, int size, int block_size, int& max_options, int& empty_cells, int& set2, int& set3);
int remove_excess_key_cells(Grid3D& opt_tbl, Grid2D& puzzle_board, Grid2D& solution_board, int size, int gen_method, int key_cells_count, int pred_id);

int verify_result(const Grid3D& opt_tbl, const Grid2D& puzzle_board, const Grid2D& solution_board, int size);

// =============================================================================
// main()
// =============================================================================
int main()
{
    cout << "Sudoku(GSV)-Generate  Solve and Verify" << endl;
    cout << "shiv.somappa@gmail.com " << endl;
    cout << "___________________________________________________________________________________" << endl;
    cout << endl;

    int flag = 0;
    int i, j, block_size = 0, size, set2 = 0, set3 = 0;
    int gen_method = 0; // generation method chosen by the user (1–4)

    // ── Three 2-D boards ──────────────────────────────────────────────────
    Grid2D puzzle_board;   // clue cells visible to the solver
    Grid2D solution_board; // complete, correct solution
    Grid2D stats_board;    // working copy used to accumulate solving statistics
    // ── 3-D options table: opt_tbl[row][col][0..size] ─────────────────────
    //   [row][col][0]      = number of remaining candidate symbols
    //   [row][col][1..N]   = symbol k is still a candidate iff value == k (else 0)
    //   [row][col][size+1] = prediction-step id that last touched this cell
    Grid3D opt_tbl;

    cout << "Enter rank of sudoku " << endl;
    cout << "starting with number 3" << endl;
    cout << "rank 3 generates 9x9 grid sudoku" << endl;
    cout << "rank 4 generates 16x16 grid sudoku and so on " << endl;

    cin >> block_size;
    if (block_size > 2)
    {
        size = block_size * block_size; // total number of rows/columns

        // ── Memory allocation ──────────────────────────────────────────────
        puzzle_board.resize(size, size);
        solution_board.resize(size, size);
        stats_board.resize(size, size);
        opt_tbl.resize(size, size, size + 5); // extra slots for metadata
        // ──────────────────────────────────────────────────────────────────

        print_message_1(gen_method); // prompt user to choose generation method

        // ── Generate puzzle using the selected method ──────────────────────
        if (gen_method == 1)
        {
            m1_GENERAT_SUDOKU_METHOD(opt_tbl, puzzle_board, solution_board, size, block_size);
        }
        else if (gen_method == 2)
        {
            m2_GENERAT_SUDOKU_METHOD(opt_tbl, puzzle_board, solution_board, size, block_size);
        }
        else if (gen_method == 3)
        {
            m3_GENERAT_SUDOKU_METHOD(opt_tbl, puzzle_board, solution_board, size, block_size);
        }
        else
        {
            m4_GENERAT_SUDOKU_METHOD(opt_tbl, puzzle_board, solution_board, size, block_size);
        }

        // Copy puzzle state into stats_board for statistics collection
        for (i = 0; i < size; i++)
        {
            for (j = 0; j < size; j++) { stats_board(i, j) = puzzle_board(i, j); }
        }

        cout << endl;
        cout << "Puzzle generate complete , solve sudoku process is running to collect statistics" << endl;
        cout << "___________________________________________________________________________________" << endl;

        // ── Solve the puzzle to collect statistics ─────────────────────────
        build_options_table(opt_tbl, puzzle_board, size, block_size);
        if (block_size == 3)
        {
            cout << "PUZZLE BOARD" << endl;
            display_table(puzzle_board, size);
        }
        cout << "Solving puzzle to collect statistics " << endl;
        SOLVE_SUDOKU(opt_tbl, puzzle_board, solution_board, stats_board, size, block_size, gen_method);
        if (block_size == 3)
        {
            cout << "SOLUTION BOARD" << endl;
            display_table(puzzle_board, size);
        }

        // ── Save puzzle and solution to text files ─────────────────────────
        char puzzle[11]   = "puzzle.txt";
        char solution[13] = "solution.txt";
        write_file(puzzle_board,   size, puzzle);
        write_file(solution_board, size, solution);

        cout << "_____________________________________________________________________________________" << endl;
        cout << "Puzzle saved as 'puzzle.txt' in GSV folder" << endl;
        cout << "Ready solution saved as 'solution.txt' in GSV folder" << endl;
        cout << "If the size does not exceed the number of columns in the sheet, the content can be copied to the spreadsheet." << endl;

        cout << "Enter any charecter  to close the window " << endl;
        char exit;
        cin >> exit;
    }
    return 0;
}

// =============================================================================
// SOLVE_SUDOKU
// Drives the two-phase solving pipeline:
//   Phase 1 – deterministic filters (filter_symbol_cts_method) narrow options.
//   Phase 2 – symbol prediction for stubborn two-option cells, iterated until
//              check_process_halt signals completion or stagnation.
// Populates stats[] with cell-fill counts, prediction counts, error counts, etc.
// =============================================================================
int SOLVE_SUDOKU(Grid3D& opt_tbl, Grid2D& puzzle_board, Grid2D& solution_board, Grid2D& stats_board, int size, int block_size, int gen_method)
{
    time_t t_start, t_end;
    int i, j, set_3, set_2, flag = 1;
    int err_count = 0;
    // max_options  – largest candidate-set size seen in any empty cell
    // empty_cell_count – number of empty cells at the start
    // key_cell_count   – number of key cells (prediction targets)
    // filled_cell_count – cells filled so far
    int max_options = 0, empty_cell_count = 0, key_cell_count = 0, filled_cell_count = 0;
    int pred_id = -1;                // prediction-step counter (decrements each step)
    int stats[10] = { 0 };          // statistics array (see print_message_2 for index map)

    // ── Find max candidate count and total empty cells ─────────────────────
    for (i = 0; i < size; i++)
    {
        for (j = 0; j < size; j++)
        {
            opt_tbl(i, j, size + 1) = 0; // reset prediction-step tag
            if (opt_tbl(i, j, 0) > 0)
            {
                empty_cell_count++;
                if (opt_tbl(i, j, 0) > max_options) { max_options = opt_tbl(i, j, 0); }
            }
        }
    }
     max_options= max_options+2;
    stats[0] = empty_cell_count; // store baseline empty-cell count

    // ── Allocate working buffers sized to the board ────────────────────────
    Grid2D tally_buf; // used by tally-based filter methods (rows = symbols, cols = metadata)
        tally_buf.resize(size + 1, max_options + 3);
    Grid2D work_buf;  // general-purpose scratch buffer (rows = size+1, cols = 5)
        work_buf.resize(size + 1, 5);

    // ── Phase 1: deterministic filtering ──────────────────────────────────
    time(&t_start);
    filter_symbol_cts_method(opt_tbl, stats_board, tally_buf, work_buf, size, block_size, max_options, 0, stats);
    flag = check_process_halt(opt_tbl, puzzle_board, solution_board, stats_board, size, set_2, set_3);

    cout << endl;
    cout << "Solving sudoku wihth symbol prediction method " << endl;
    cout << "Cells filled " << "  " << "key cells prediction" << "  " << "prediction errors" << endl;

    // ── Phase 2: prediction loop ───────────────────────────────────────────
    // Continues until check_process_halt returns 0 (done) or 2 (stagnated)
    while (flag == 1)
    {
        // Predict a symbol for one two-option cell, cross-check with solution_board
        flag = predict_symbol(opt_tbl, puzzle_board, solution_board, work_buf, size, block_size, pred_id, stats);

        if (flag > 0)
        {
            // Re-run deterministic filters after the prediction
            filter_symbol_cts_method(opt_tbl, stats_board, tally_buf, work_buf, size, block_size, max_options, pred_id + 1, stats);
        }
        flag = check_process_halt(opt_tbl, puzzle_board, solution_board, stats_board, size, set_2, set_3);

        // Live progress display (carriage-return overwrites the same line)
        cout << stats[3] << "             " << stats[1] << "                         " << stats[2] << "  " << '\r';
        cout.flush();
    }

    time(&t_end);
    cout << endl;
    cout << endl;

    // ── Verify and print final statistics ─────────────────────────────────
    if (verify_result(opt_tbl, stats_board, solution_board, size) == 1)
    {
        print_message_2(opt_tbl, stats_board, solution_board, size, set_2, stats, t_start, t_end);
    }

    return 0;
}

// =============================================================================
// check_process_halt
// Inspects opt_tbl to decide whether solving can continue.
//   Returns 0 – solved (or dominated by 2-option cells; applies shortcut fill)
//   Returns 1 – progress still possible; keep iterating
//   Returns 2 – single distinct option-set size; terminate
//
// When two-option cells (set2) outnumber distinct option groups (set3), all
// cells with more than 2 candidates are filled directly from solution_board.
// =============================================================================
int check_process_halt(Grid3D& opt_tbl, Grid2D& puzzle_board, Grid2D& solution_board, Grid2D& stats_board, int size, int& set2, int& set3)
{
    int i, j, c = 0;

    set3 = 0; set2 = 0;

    // Count two-option cells (set2) and distinct candidate-set sizes (set3)
    for (i = 0; i < size; i++)
    {
        for (j = 0; j < size; j++)
        {
            if (opt_tbl(i, j, 0) > 0)
            {
                if (opt_tbl(i, j, 0) > 0)
                {
                    if (opt_tbl(i, j, 0) == 2) { set2++; }
                    if (opt_tbl(i, j, 0) != c) { c = opt_tbl(i, j, 0); set3++; }
                }
            }
        }
    }

    if (set3 > 1)
    {
        if (set2 > set3)
        {
            // Two-option cells dominate: shortcut by filling all >2-option cells
            // directly from solution_board and clearing their candidates
            for (i = 0; i < size; i++)
            {
                for (j = 0; j < size; j++)
                {
                    if (opt_tbl(i, j, 0) > 2)
                    {
                        puzzle_board(i, j) = solution_board(i, j);
                        stats_board(i, j)  = solution_board(i, j);
                        opt_tbl(i, j, 0)   = 0; // mark cell as filled
                    }
                }
            }
            set3 = 0;
            return 0; // signal: done via shortcut
        }
        else { return 1; } // still progressing normally
    }
    else { return 2; } // single group left; terminate
}

// =============================================================================
// print_message_1
// Displays the generation-method menu and reads the user's choice into `i`.
// =============================================================================
int print_message_1(int& i)
{
    cout << endl;
    cout << "Enter :1   For Number of key cells- Zero " << endl;
    cout << endl;
    cout << "Enter :2   For Number of key cells- Less than N" << endl;
    cout << endl;
    cout << "Enter :3   For Number of key cells- greater than N" << endl;
    cout << "           minimum initial symbols and partila implementation " << endl;
    cout << "           of sudoku properties" << endl;
    cout << endl;
    cout << "Enter :4   For completly random initial symbol placement " << endl;
    cout << endl;

    cout << "Note: there may be variation in number of key cells " << endl;
    cin >> i;
    return 0;
}

// =============================================================================
// print_message_2
// Displays the post-solve statistics report.
// stats[] index map:
//   [0] empty cells to fill   [1] key-cell predictions  [2] prediction errors
//   [3] total cells filled    [4] hidden singles         [5] pointing-pair removals
//   [6] naked-pair removals   [7] two-pass tally (s-c=0) [8] near-tight tally
//   [9] bound-set removals
// =============================================================================
int print_message_2(const Grid3D& opt_tbl,
    const Grid2D& stats_board,
    const Grid2D& solution_board,
    int size,
    int set2,
    int stats[],
    time_t start,
    time_t end)
{
    // Display option table (only for 9x9 — larger boards are too wide)
    if (size == 9)
    {
        cout << "OPTION TABLE\n";
        display_p_OT(opt_tbl, size, 0);
    }

    time(&end); // capture end time

    cout << "\n\nSTATISTICS\n";

    if (set2 > 0)
        cout << "Puzzle has multiple solution\n";
    else
        cout << "Puzzle has unique solution\n";

    cout << "\n";
    cout << "Number of empty cells to be filled - " << stats[0] << "\n\n";
    cout << "Number of key cells                - " << stats[1] << "\n\n";
    cout << "Number of symbol prediction errors - " << stats[2] << "\n\n";

    // Naked singles = total fills minus key-cell predictions minus hidden singles
    cout << "Number of single naked             - "
         << stats[3] - stats[1] - stats[4] << "\n\n";

    cout << "Number of hidden singles           - " << stats[4] << "\n\n";

    cout << "______________________________________________________________________________\n";
    cout << "Deterministic filters data:\n";

    cout << "Symbols filtered by Pointing Pairs and Box-Line Reduction : " << stats[5] << "\n";
    cout << "Symbols filtered by naked pair (twin sets)                : " << stats[6] << "\n";
    cout << "Symbols filtered by two-pass tally (s-c = 0)              : " << stats[7] << "\n";
    cout << "Symbols filtered by near tight tally (s-c = n)            : " << stats[8] << "\n";
    cout << "Symbols filtered by bound set (symbol first)              : " << stats[9] << "\n";

    elapsed_time(start, end);

    cout << "\n\nNote:\n";
    cout << "Except error rectification procedure, all solving procedures are used for statistics.\n";
    cout << "Errors are fixed by cross-checking predicted symbols with the ready solution.\n\n";

    // Show multiple-solution cell list for small boards
    if (set2 > 0 && size < 82)
    {
        display_multiple_solution_list(opt_tbl, size);
        cout << "\n";
    }

    return 0;
}

// =============================================================================
// display_table
// Prints a 2-D board to stdout with block separators.
// =============================================================================
int display_table(Grid2D& table, int size)
{
    int i = 0, j = 0, bh, bv, blk;
    blk = int(sqrt(size)); bh = blk - 1; bv = blk - 1;
    for (i = 0; i < size; i++)
    {
        for (j = 0; j < size; j++)
        {
            if (size < 10)
            {
                // Single-digit symbols
                if (table(i, j) == 0) { cout << "  " << "-"; }
                else { cout << "  " << table(i, j); }
                if (j == bh) { cout << "  "; bh = bh + blk; }
            }
            else
            {
                // Two-digit symbols (boards 10x10 and larger)
                if (table(i, j) == 0) { cout << "  " << "--"; }
                else if (table(i, j) < 10) { cout << "   " << table(i, j); }
                else { cout << "  " << table(i, j); }
                if (j == bh) { cout << "  "; bh = bh + blk; }
            }
        }
        bh = blk - 1;
        if (i == bv) { cout << endl; bv = bv + blk; }
        cout << endl;
    }
    return 0;
}

// =============================================================================
// verify_result
// Cross-checks puzzle_board and opt_tbl against solution_board.
// Returns 1 if no errors found, 0 otherwise.
// =============================================================================
int verify_result(const Grid3D& opt_tbl, const Grid2D& puzzle_board, const Grid2D& solution_board, int size)
{
    int i, j, s, flag = 0;

    for (i = 0; i < size; i++)
    {
        for (j = 0; j < size; j++)
        {
            if (puzzle_board(i, j) > 0)
            {
                // Filled cell: value must match solution
                if (puzzle_board(i, j) != solution_board(i, j)) { flag = 1; }
            }
            else
            {
                // Empty cell: solution symbol must still be a candidate
                s = solution_board(i, j);
                if (opt_tbl(i, j, s) == 0) { flag = 1; }
            }
        }
    }

    if (flag == 1) { cout << "Verified result Error found" << endl; return 0; }
    else { cout << "Verified result Error not found" << endl; return 1; }
}

// =============================================================================
// display_p_OT
// Prints the options table (opt_tbl) in a formatted grid with block separators.
// `add` extends the depth slice shown beyond size+1 (for debugging extra slots).
// =============================================================================
int display_p_OT(const Grid3D& opt_tbl, int size, int add)
{
    int i = 0, j = 0, k = 0, bh, bv, blk;
    blk = int(sqrt(size)); bh = blk - 1; bv = blk - 1;
    for (i = 0; i < size; i++)
    {
        for (j = 0; j < size; j++)
        {
            for (k = 0; k < size + 1 + add; k++)
            {
                if (opt_tbl(i, j, k) == 0) { cout << ","; }
                else if (opt_tbl(i, j, k) < 10) { cout << opt_tbl(i, j, k); }
                else { cout << opt_tbl(i, j, k); }
            }
            if (j == bh) { cout << "  "; bh = bh + blk; }
            cout << " ";
        }
        bh = blk - 1;
        if (i == bv) { cout << endl; bv = bv + blk; }
        cout << endl;
    }
    return 0;
}

// =============================================================================
// display_multiple_solution_list
// When set2 > 0, scans opt_tbl for cells that still have exactly 2 candidates
// and prints their row, column, and the two candidate symbols.
// =============================================================================
int display_multiple_solution_list(const Grid3D& opt_tbl, int size)
{
    int i = 0, j, s1, s2, c = 0, c1 = 0;

    // Detect the first ambiguous cell to confirm multiple-solution condition
    while (i < size && c == 0)
    {
        j = 0;
        while (j < size && c == 0)
        {
            if (opt_tbl(i, j, 0) > 0)
            {
                c = opt_tbl(i, j, 0);
                c1++;
            }
            j++;
        }
        i++;
    }

    if (c == 2 && c1 < 50)
    {
        cout << "Multiple solutions - Cell options\n";
        cout << "=====================================================\n";

        int col_count = 0;

        for (i = 0; i < size; i++)
        {
            for (j = 0; j < size; j++)
            {
                if (opt_tbl(i, j, 0) > 0)
                {
                    // Find the two remaining candidates in this cell
                    s1 = 1;
                    while (opt_tbl(i, j, s1) == 0) { s1++; }

                    s2 = s1 + 1;
                    while (opt_tbl(i, j, s2) == 0) { s2++; }

                    // Aligned print: [row, col]{sym1, sym2}
                    cout << "[";
                    cout << setw(2) << i << ",";
                    cout << setw(2) << j << "]";
                    cout << "{";
                    cout << setw(2) << s1 << ",";
                    cout << setw(2) << s2 << "} ";

                    col_count++;

                    // 10 entries per row
                    if (col_count == 10)
                    {
                        cout << "\n";
                        col_count = 0;
                    }
                }
            }
        }

        if (col_count != 0) cout << "\n";
    }

    cout << "\n\n";
    cout << "If we pre-fill the puzzle board with these symbols, "
         << "the board will become a unique solution board.\n";

    return 0;
}

// =============================================================================
// Find_output_file_size
// Computes the number of bytes needed to serialise `table` as text
// (each cell value formatted as decimal + separator byte per column,
//  plus CR+LF per row).
// =============================================================================
int Find_output_file_size(Grid2D& table, int size)
{
    int i, j, buf_size = size; // starts with one byte per row (for newline)
    for (i = 0; i < size; i++)
    {
        for (j = 0; j < size; j++)
        {
            // Each value takes (digits + 1 separator) bytes
            if      (table(i, j) < 10)    { buf_size = buf_size + 2; }
            else if (table(i, j) < 100)   { buf_size = buf_size + 3; }
            else if (table(i, j) < 1000)  { buf_size = buf_size + 4; }
            else if (table(i, j) < 10000) { buf_size = buf_size + 5; }
            else                          { buf_size = buf_size + 6; }
        }
    }
    return buf_size;
}

// =============================================================================
// Write_table_to_text_buffer
// Serialises `table` into a raw byte buffer `out` using tab (ASCII 9) as the
// cell separator and CR (13) + LF (10) as the row terminator.
// Returns the number of bytes written.
// =============================================================================
int Write_table_to_text_buffer(Grid2D& table, char* out, int size)
{
    int i, j, k = 0, v; // k = write position in `out`
    for (i = 0; i < size; i++)
    {
        for (j = 0; j < size; j++)
        {
            if (table(i, j) < 10)
            {
                v = table(i, j);
                out[k + 1] = 9;           // tab separator
                out[k]     = int2char(v);
                k = k + 2;
            }
            else if (table(i, j) < 100)
            {
                v = table(i, j);
                out[k + 2] = 9;
                out[k + 1] = int2char(v % 10); v = v / 10;
                out[k]     = int2char(v);
                k = k + 3;
            }
            else if (table(i, j) < 1000)
            {
                v = table(i, j);
                out[k + 3] = 9;
                out[k + 2] = int2char(v % 10); v = v / 10;
                out[k + 1] = int2char(v % 10); v = v / 10;
                out[k]     = int2char(v);
                k = k + 4;
            }
            else if (table(i, j) < 10000)
            {
                v = table(i, j);
                out[k + 4] = 9;
                out[k + 3] = int2char(v % 10); v = v / 10;
                out[k + 2] = int2char(v % 10); v = v / 10;
                out[k + 1] = int2char(v % 10); v = v / 10;
                out[k]     = int2char(v);
                k = k + 5;
            }
            else if (table(i, j) < 100000)
            {
                v = table(i, j);
                out[k + 5] = 9;
                out[k + 4] = int2char(v % 10); v = v / 10;
                out[k + 3] = int2char(v % 10); v = v / 10;
                out[k + 2] = int2char(v % 10); v = v / 10;
                out[k + 1] = int2char(v % 10); v = v / 10;
                out[k]     = int2char(v);
                k = k + 6;
            }
        }
        // End-of-row: overwrite last tab with CR, then append LF
        out[k - 1] = 13; out[k] = 10; k++;
    }
    return k;
}

// =============================================================================
// write_file
// Allocates a buffer, serialises `table` into it, and writes the binary file.
// =============================================================================
void write_file(Grid2D& table, int size, char* filename)
{
    int buf_size;
    char* out = NULL;
    buf_size = Find_output_file_size(table, size);
    out = new char[buf_size];
    fstream output;

    Write_table_to_text_buffer(table, out, size);
    output.open(filename, ios::out | ios::binary);
    output.write((char*)out, sizeof(char) * buf_size);
    output.close();

    delete[] out;
}

// =============================================================================
// char2int / int2char
// Lookup-table conversions between ASCII digit characters and integer values.
// =============================================================================
int char2int(int num)
{
    switch (num)
    {
        case 48: return 0; break;
        case 49: return 1; break;
        case 50: return 2; break;
        case 51: return 3; break;
        case 52: return 4; break;
        case 53: return 5; break;
        case 54: return 6; break;
        case 55: return 7; break;
        case 56: return 8; break;
        case 57: return 9; break;
    }
}

int int2char(int num)
{
    switch (num)
    {
        case 0: return 48; break;
        case 1: return 49; break;
        case 2: return 50; break;
        case 3: return 51; break;
        case 4: return 52; break;
        case 5: return 53; break;
        case 6: return 54; break;
        case 7: return 55; break;
        case 8: return 56; break;
        case 9: return 57; break;
    }
}

// =============================================================================
// dis_format
// Right-pads `num` with leading spaces to occupy a fixed 10-character column.
// =============================================================================
void dis_format(unsigned int num)
{
    if      (num > 999999999) { cout << num; }
    else if (num > 99999999)  { cout << " "          << num; }
    else if (num > 9999999)   { cout << "  "         << num; }
    else if (num > 999999)    { cout << "   "        << num; }
    else if (num > 99999)     { cout << "    "       << num; }
    else if (num > 9999)      { cout << "     "      << num; }
    else if (num > 999)       { cout << "      "     << num; }
    else if (num > 99)        { cout << "       "    << num; }
    else if (num > 9)         { cout << "        "   << num; }
    else                      { cout << "         "  << num; }
}

// =============================================================================
// elapsed_time
// Computes and prints elapsed time in HH-MM-SS format from two time_t values.
// =============================================================================
int elapsed_time(time_t start, time_t end)
{
    int h1, h2, m1, m2, s1, s2, dif = end - start;

    // Decompose seconds into H/M/S and split each into two display digits
    s1 = dif % 60; dif = dif / 60; s2 = s1 % 10; s1 = s1 / 10;
    m1 = dif % 60; dif = dif / 60; m2 = m1 % 10; m1 = m1 / 10;
    h1 = dif;      h2  = h1 % 10;  h1 = h1 / 10;

    cout << endl;
    cout << " Elapsed time : " << h1 << h2 << " hrs - " << m1 << m2 << " mins - " << s1 << s2 << " sec " << endl;
    cout << endl;
    return 0;
}
