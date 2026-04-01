CTST Sudoku Solver — Cell-to-Symbol Tally for Generalised N×N Sudoku

A practical, lightweight constraint-propagation solver for generalised Sudoku puzzles (ranks 3–20+), implementing the **Cell-to-Symbol Tally (CTST)** method described in:

> *A Cell-to-Symbol Tally Approach for the ≤3-Candidate Subclass of Generalised Sudoku* — Shiv Bali

---

Overview

Standard Sudoku solvers rely on global matching (bipartite graphs, SAT encodings) or pure backtracking. CTST takes a different approach: it isolates and directly operationalises two structural regimes that existing methods handle only implicitly:

- **Near-tight subsets** (`s−c = 1`) — handled by Rule 8, a dedicated lightweight filter
- **Symbol-first tight detection** — handled by Rule 7, a bound-sets accumulation algorithm

Within the **≤3-Candidate Class** (puzzles where at every deterministic fixed point at least one cell has ≤3 candidates), CTST solves puzzles from 9×9 up to 400×400 without requiring bipartite matching or clause learning.

---

Features

- **Eight deterministic filtering rules** (Rules 1–8) applied in a first-trigger priority pipeline
- **Predict-and-rectify** with conflict-directed backjumping and hint-preserving rollback
- **Scales to rank 20** (400×400 grids, 160,000 cells)
- Puzzle input via tab-separated text files; works for any rank where `N = n²`

---

The Eight Rules

| Rule | Name | Regime | Direction |
|------|------|--------|-----------|
| 1 | Naked Singles | Single cell | Cell → Value |
| 2 | Hidden Singles | Single symbol per unit | Symbol → Cell |
| 3 | Naked Pairs *(fast-path)* | Two-cell tight (`k=2` case of Rule 5) | Cell → Symbol |
| 4 | Pointing Pairs / Box-Line Reduction | Row/col × block intersection | Symbol → Block |
| 5 | Ascending Tally | Tight subsets, all sizes (naked) | Cell → Symbol |
| 6 | Descending Tally | Tight subsets, all sizes (hidden) | Cell → Symbol |
| 7 | Bound-Sets Filter | Tight, symbol-first accumulation | Symbol → Cell |
| 8 | Near-Tight Analysis | Near-tight (`s−c = 1`) | Cell → Symbol |

Rules execute in order. Any elimination restarts from Rule 1 (first-trigger restart).

---

Repository Structure

ctst-sudoku/
├── src/
│   └── ctst_solver.cpp
├── puzzles/
│   ├── sample_9x9.txt
│   ├── sample_16x16.txt
│   └── sample_81x81.txt
├── paper/
│   └── CTST_preprint.pdf
└── README.md

---

Building

Requires C++14 or later. Tested with GCC 9+, Clang 10+, and MSVC 2015+.

**GCC / Clang**

g++ -std=c++14 -O2 -o ctst_solver src/ctst_solver.cpp

**MSVC (Developer Command Prompt)**

cl /std:c++14 /O2 /EHsc src\ctst_solver.cpp

**Visual Studio (GUI)**
1. File → New → Project → Console App (C++)
2. Add the .cpp file
3. Project → Properties → C/C++ → Language → ISO C++14
4. Project → Properties → C/C++ → Optimization → Maximize Speed (/O2)
5. Build Solution (F7)

---

Puzzle File Format

Tab-separated values, one row per line, 0 for empty cells. Values 1 to N, where N is the grid side length.

- 9×9: 9 rows × 9 values
- 16×16: 16 rows × 16 values
- 81×81: 81 rows × 81 values

---

Running

./ctst_solver puzzle.txt

The solver prints a progress display during solving and outputs the completed grid on success.

---

Performance

The detailed per-rank performance tables are provided in Appendix B of the paper.

**Important limitation of the reported benchmarks:** The performance data in the paper was collected without the full autonomous error rectification procedure. Whenever a prediction error occurred during benchmarking, rectification was performed by comparing against a pre-computed known solution rather than by the solver's internal conflict-directed backjumping mechanism. The reported solve times therefore reflect the deterministic phase and prediction costs accurately, but do not fully capture the overhead of autonomous error rectification on harder instances.

**An independent solver with the complete error rectification procedure implemented will be uploaded to this repository shortly.** Benchmarks obtained with that version will supersede the figures in the paper for the prediction phase.

Summary (deterministic phase, no prediction errors):

| Grid | 9×9 | 16×16 | 25×25 | 49×49 | 64×64 | 81×81 |
|------|-----|-------|-------|-------|-------|-------|
| Samples | 50 | 50 | 50 | 50 | 20 | 20 |
| Avg. solve time (s) | 0.001 | 0.01 | 0.25 | 0.35 | 1 | 3 |

---

Algorithmic Details

**Complexity**

| Phase | Complexity |
|-------|------------|
| Single convergence pass (Rules 1–4) | O(N³√N) |
| Single convergence pass (Rules 5–8, dominant) | O(N⁵) |
| Full deterministic phase | O(R·N⁵), R small in practice |
| Per prediction step | O(N⁵) |
| Full solve (K prediction steps) | O(K·N⁵) |
| Space | O(N³) |

In practice K ≪ N²: median K = 2 on standard 9×9 puzzles; K ≈ 12,859 at rank 18 (324×324), roughly 1/8 of N².

**Predict-and-Rectify**

When the deterministic phase reaches a fixed point, the solver selects the cell with the fewest candidates (≤3 within the tested class) and predicts a value. Errors are detected via Rule 8 Case B, Rules 5–6 singleton collision, and the consistency condition P_et[i,j] ⊄ P_ps[i,j]. On error, conflict-directed backjumping traces triggerLv records to the originating prediction. Symbols predicted after the root cause are saved as hints and replayed after rollback.

**≤3-Candidate Class**

CTST is designed for the ≤3-Candidate Class: puzzles where, at every deterministic fixed point requiring prediction, at least one unresolved cell has ≤3 candidates. This is a class-defining assumption. If no such cell exists, the solver halts and reports a class-membership failure rather than predicting from a larger set. No class-membership failure was encountered across all tested instances (ranks 3–20).

---

Rule 8 — Near-Tight Analysis

Rule 8 is the primary novel contribution. It targets near-tight groups (s−c = 1) — configurations where a set of c cells has exactly c+1 distinct candidate symbols — and provides a dedicated lightweight procedure not identified in the surveyed constraint-propagation literature.

An exhaustive search of AAAI, IJCAI, CP, ECAI, the Constraints journal, and the Artificial Intelligence journal (1994–2024) found no prior work with a dedicated algorithm for this regime. Existing methods (e.g. Régin's GAC algorithm) subsume its deductions but handle the near-tight case only implicitly within a full matching pass.

Case table:

| Case | solo(c) distribution | Outcome |
|------|----------------------|---------|
| B | Any cell: solo > 2 | Immediate error → rollback |
| A-resolve | Exactly one c* with solo = 2; qualifying 2-element right-side cell found | Filter: remove surplus, collapse to tight subset |
| A-error | Both singletons have qualifying right-side cells simultaneously | Immediate error → rollback |
| A-key | Exactly one c* with solo = 2; no qualifying right-side cell | Add c* to prediction queue |
| D | No cell with solo ≥ 2 | Skip — insufficient information |

---

Ablation Study: Rules 7 and 8

Removing Rules 7 and 8 causes:

- M4 (Naked Pair) ↑ — 29% increase in 49×49 (88,364 → 113,884)
- M7 (Tight Group) ↓ — 67% reduction in 49×49 (32,428 → 10,450)
- Increased computation at lower-order stages, reduced structured inference

This confirms that Rules 7 and 8 are not cosmetic — they compress candidate structures in ways that enable the tally rules (5–6) to operate effectively.

---

References

1. T. Yato and T. Seta, "Complexity and completeness of finding another solution," IEICE Trans. Fundamentals, vol. E86-A, no. 5, pp. 1052–1060, 2003.
2. P. Norvig, "Solving every Sudoku puzzle," 2006. https://norvig.com/sudoku.html
3. J.-C. Régin, "A filtering algorithm for constraints of difference in CSPs," in Proc. AAAI-94, pp. 362–367, 1994.
4. P. Hall, "On representatives of subsets," Journal of the London Mathematical Society, vol. 10, pp. 26–30, 1935.
5. G. Royle, "Minimum Sudoku — 17-clue puzzle corpus," University of Western Australia. http://staffhome.ecm.uwa.edu.au/~00013890/sudokumin.php

---

Author

Shiv Bali — shiv.somappa@gmail.com

---

License

This project is released for academic and research use. Please cite the accompanying paper if you use this code or method in your work.
