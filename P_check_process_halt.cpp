// =============================================================================
// P_check_process_halt.cpp
// Legacy / alternative halt-check used by some generation methods.
//
// process_halt() inspects opt_tbl and reports whether any empty cells remain:
//   - Returns 2 (done)   when all cells are filled or only two-option cells are
//     left and no other group is present.
//   - Returns 1 (continue) otherwise.
// =============================================================================

#include"filters.h"
using namespace std;

// =============================================================================
// process_halt
// Scans opt_tbl for cells that still need to be filled:
//   set2 – cells with exactly 2 candidates (potential ambiguity)
//   large_opt_cells – cells with more than 2 candidates (still solvable deterministically)
// Returns 2 when no further solving is possible/needed, 1 to keep iterating.
// =============================================================================
int process_halt(Grid3D& opt_tbl, int size)
{
    int i = 0, j = 0, set2 = 0, large_opt_cells = 0;

    // Count two-option cells and cells with more than 2 options
    for (i = 0; i < size; i++)
    {
        for (j = 0; j < size; j++)
        {
            if (opt_tbl(i, j, 0) > 0)
            {
                if (opt_tbl(i, j, 0) == 2) { set2++; }
                else                        { large_opt_cells++; }
            }
        }
    }

    if (large_opt_cells == 0)
    {
        // No cell has more than 2 candidates → puzzle is effectively solved
        if (set2 > 0) { cout << " puzzle solve and puzzle has multiple solutions" << endl; }
        else          { cout << "Puzzle solved and puzzle has unique solution " << endl; }
        return 2;
    }
    else if (set2 == 0)
    {
        // No two-option cells remain but larger sets exist → cannot continue
        cout << " Two symbol set not found process will terminate" << endl;
        return 2;
    }
    else { return 1; } // still progressing
}
