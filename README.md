# CTST Sudoku — GSV (Generate · Solve · Verify)

C++ implementation of the Cell-to-Symbol Tally method for generalised N×N Sudoku.  
This repository contains the **GSV project**: it generates a puzzle, solves it using the CTST filtering and prediction pipeline (cross-checking against a pre-built reference solution), and collects the statistics reported in the paper.

> **Coming in Version 2 — Independent Solver**  
> A standalone solver with no reference solution, plus runtime comparisons against DLX and SAT baselines across ranks 3–20. See [Planned: Independent Solver](#planned-independent-solver) below.

---

## Build

```bash
# Compile all source files together (GCC example)
g++ -O2 -o gsv \
    Sudoku_Main.cpp \
    Build_options_table.cpp \
    Create_sudoku_M1.cpp \
    Create_sudoku_M2.cpp \
    Create_sudoku_M3.cpp \
    Create_sudoku_M4.cpp \
    P_filter_symbol_cts_methods.cpp \
    P_filter_symbol_condition_1.cpp \
    P_filter_symbol_condition_2.cpp \
    P_filter_symbol_condition_3.cpp \
    P_filter_symbol_condition_4.cpp \
    P_filter_symbol_condition_5.cpp \
    P_filter_symbol_condition_6.cpp \
    P_filter_symbol_condition_7.cpp \
    P_predict_symbol.cpp \
    P_check_process_halt.cpp \
    three_way_sort.cpp \
    -lm

./gsv
```

---

## Running

The program runs interactively from the console.

**Step 1 — Enter rank**
```
Enter rank of sudoku
starting with number 3
rank 3 generates 9x9 grid sudoku
rank 4 generates 16x16 grid sudoku and so on
```

**Step 2 — Choose puzzle difficulty (key-cell count)**
```
Enter :1   For Number of key cells - Zero
Enter :2   For Number of key cells - Less than N       (minimum key cells)
Enter :3   For Number of key cells - Greater than N    (maximum key cells, partial implementation)
Enter :4   For completely random initial symbol placement
```

The solver then generates the puzzle, solves it, prints statistics, and writes two output files to the working directory:
- `s2.txt` — the generated puzzle (empty cells as 0)
- `solution.txt` — the reference solution

For rank 3 (9×9), the puzzle board, solution board, and options table are also printed to the console.

---

## How the GSV Pipeline Works

```
Generate → Solve (with reference check) → Print statistics
```

### 1. Generate

One of four generation methods is called based on the user's choice:

| Option | Function | What it produces |
|---|---|---|
| 1 | `m1_GENERAT_SUDOKU_METHOD` | Zero key cells — fully deterministic solve possible |
| 2 | `m2_GENERAT_SUDOKU_METHOD` | Minimum key cells — few predictions needed |
| 3 | `m3_GENERAT_SUDOKU_METHOD` | Maximum key cells — many predictions needed |
| 4 | `m4_GENERAT_SUDOKU_METHOD` | Random initial placement |

Each method builds a complete solution (`s_table`), then removes symbols to create the puzzle (`p_table`).

### 2. Solve

`SOLVE_SUDOKU` runs the CTST pipeline to fill in empty cells and collect statistics:

```
build_options_table()           ← initialise candidate sets (p_OT)
filter_symbol_cts_method()      ← run Rules 1–8 to deterministic fixed point
while not complete:
    predict_symbol()            ← pick key cell, guess a value (cross-check with s_table)
    filter_symbol_cts_method()  ← propagate after prediction
    check_process_halt()        ← test for completion or stuck state
verify_result()                 ← confirm solution matches s_table
print_message_2()               ← print statistics
```

> **Note on verification:** In this GSV version the solver uses `s_table` (the reference solution) to verify each prediction. This is what makes it a *data-collection* tool rather than an independent solver — errors are detected by comparison, not by structural inconsistency alone.

### 3. Statistics printed

```
Puzzle has unique / multiple solution
Number of empty cells to be filled
Number of key cells (predictions made)
Number of cells filled by CELL TO SYMBOL TALLY method
Number of symbol prediction errors
Elapsed time
```

---

## Source File Reference

### Entry point and orchestration

| File | Role |
|---|---|
| `Sudoku_Main.cpp` | `main()`, `SOLVE_SUDOKU()`, `check_process_halt()`, display and file I/O |

### Headers

| File | Declares |
|---|---|
| `gsv.h` | Generation method signatures, display/file utilities |
| `filters.h` | All 8 filter functions + `predict_symbol()` |

### Puzzle generation

| File | Method |
|---|---|
| `Create_sudoku_M1.cpp` | M1 — zero key cells |
| `Create_sudoku_M2.cpp` | M2 — minimum key cells |
| `Create_sudoku_M3.cpp` | M3 — maximum key cells |
| `Create_sudoku_M4.cpp` | M4 — random placement |

### CTST filtering (Rules 1–8)

| File | Rule | Function |
|---|---|---|
| `Build_options_table.cpp` | Init | `build_options_table()` — build candidate sets from the puzzle board |
| `P_filter_symbol_cts_methods.cpp` | Pipeline | `filter_symbol_cts_method()` — runs all rules in order; restarts from Rule 1 on any elimination |
| `P_filter_symbol_condition_1.cpp` | Rule 1 | Naked Singles — `initialise_insert_symbol_into_puzzle_table()` |
| `P_filter_symbol_condition_2.cpp` | Rule 2 | Hidden Singles — `find_hidden_symbol()` |
| `P_filter_symbol_condition_3.cpp` | Rule 3 | Naked Pairs (fast path) — `find_twin_set()` |
| `P_filter_symbol_condition_4.cpp` | Rule 4 | Pointing Pairs / Box-Line Reduction — `filter_block_symbol()` |
| `P_filter_symbol_condition_5.cpp` | Rule 5 | Ascending Tally (Naked Subsets) — `find_n_symbol_n_cell_tally_asc()` |
| `P_filter_symbol_condition_6.cpp` | Rule 6 | Descending Tally (Hidden Subsets) — `find_n_symbol_n_cell_tally_dsc()` |
| `P_filter_symbol_condition_7.cpp` | Rule 7 | Bound-Sets Filter — `remove_invalid_options_from_bound_cells()` |
| `P_filter_symbol_cts_methods.cpp` | Rule 8 | Near-Tight Analysis — `filter_symbol_sets_group_method()` |

### Prediction and control

| File | Role |
|---|---|
| `P_predict_symbol.cpp` | `predict_symbol()` — selects key cell, tries candidates using 17 ordering conditions, cross-checks with `s_table` |
| `P_check_process_halt.cpp` | `check_process_halt()` — checks whether all cells are resolved or the solver is stuck |
| `three_way_sort.cpp` | Sort utility used by the tally rules |

---

## Key Data Structures

```cpp
int**  p_table           // puzzle board      [size][size]
int**  s_table           // reference solution [size][size]  ← used for prediction verification in GSV
int**  t_table           // temp board for statistics collection [size][size]
int*** p_OT              // candidate (options) table [size][size][size+5]
                         // p_OT[i][j][0]      = number of remaining candidates for cell (i,j)
                         // p_OT[i][j][1..N]   = candidate symbols
                         // p_OT[i][j][N+1..N+4] = metadata (prediction ID, trigger level, etc.)
```

---

## Planned: Independent Solver

Version 2 of this project will replace the reference-solution cross-check with full autonomous error detection and backjumping, making it a true standalone solver. Planned additions:

- [ ] `p_ET` and `p_PS` dual-table consistency check (Definition 7 in paper)
- [ ] `triggerLv` conflict-directed backjumping — no reference solution needed
- [ ] Hint-preserving rollback
- [ ] Rule 8 extended to s−c ≥ 3
- [ ] Runtime comparison against DLX and SAT baselines across ranks 3–20
- [ ] Ranks 19–20 maximum-key-cell benchmarks

---

## Reference

*A Cell-to-Symbol Tally Approach for Generalised Sudoku* — Shiv Bali, April 2026  
Contact: shiv.somappa@gmail.com
