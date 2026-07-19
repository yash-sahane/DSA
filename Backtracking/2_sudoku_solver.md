# Sudoku Solver

**Problem Link:** https://leetcode.com/problems/sudoku-solver/
**Platform:** LeetCode
**Difficulty:** Hard
**Topic:** Backtracking
**Pattern:** Constraint-satisfaction backtracking (try a value, recurse, undo on failure)

---

## Problem

Fill a partially completed `9 x 9` Sudoku board (empty cells are `'.'`) so that every **row**, every **column**, and each of the nine `3 x 3` sub-boxes contains the digits `1–9` exactly once. The board is guaranteed to have a unique solution and must be solved **in place**.

### Example 1

**Input:**

```text
board =
5 3 . | . 7 . | . . .
6 . . | 1 9 5 | . . .
. 9 8 | . . . | . 6 .
------+-------+------
8 . . | . 6 . | . . 3
4 . . | 8 . 3 | . . 1
7 . . | . 2 . | . . 6
------+-------+------
. 6 . | . . . | 2 8 .
. . . | 4 1 9 | . . 5
. . . | . 8 . | . 7 9
```

**Output:**

```text
5 3 4 | 6 7 8 | 9 1 2
6 7 2 | 1 9 5 | 3 4 8
1 9 8 | 3 4 2 | 5 6 7
------+-------+------
8 5 9 | 7 6 1 | 4 2 3
4 2 6 | 8 5 3 | 7 9 1
7 1 3 | 9 2 4 | 8 5 6
------+-------+------
9 6 1 | 5 3 7 | 2 8 4
2 8 7 | 4 1 9 | 6 3 5
3 4 5 | 2 8 6 | 1 7 9
```

### Constraints

```text
board.length == 9, board[i].length == 9
board[i][j] is a digit 1-9 or '.'
It is guaranteed the input has a unique solution.
```

---

## Intuition

Find the first empty cell and try each digit `1–9` that doesn't already violate a row/column/box constraint. Place it, then **recurse** to solve the rest. If the recursion eventually succeeds, we're done; if it gets stuck, **undo** the digit (`backtrack`) and try the next candidate. Returning `false` after exhausting all digits for a cell forces the caller to try a different choice earlier.

The return-value threading is what makes this efficient: as soon as a placement leads to a full valid board, `true` propagates straight back up and stops all further search.

## Algorithm

1. `solve` scans for the first `'.'` cell.
2. For that cell, try `val = '1'..'9'`; if `isPossible(val)`, set the cell, recurse.
   - If the recursive `solve` returns `true`, return `true`.
   - Else reset the cell to `'.'` and try the next digit.
3. If no digit works, return `false` (dead end → caller backtracks).
4. If no empty cell remains, the board is complete → return `true`.
5. `isPossible` checks the digit against the current row, column, and enclosing `3 x 3` box.

## Visualization

```text
Constraints for cell (row, col) = (4, 4):

 row 4:  4 2 6 | 8 . 3 | 7 9 1     <- no duplicate in this row
 col 4:  7,9,.,6,.,2,.,1,8         <- no duplicate down this column
 box:    the 3x3 block containing (4,4):
         . 6 .
         8 . 3      <- no duplicate in this box
         . 2 .

Box origin: sRow = (row/3)*3, sCol = (col/3)*3
            (4/3)*3 = 3, so box spans rows 3..5, cols 3..5.

Try 1..9 -> place first legal digit -> recurse -> undo if it fails.
```

## Code

```cpp
// BRUTE FORCE - Backtracking
bool isPossible(vector<vector<char>> &board, char val, int row, int col, int n)
{
  // same row
  for (int c = 0; c < n; c++)
  {
    if (board[row][c] == val)
      return false;
  }

  // same col
  for (int r = 0; r < n; r++)
  {
    if (board[r][col] == val)
      return false;
  }

  // same grid
  int sRow = (row / 3) * 3;
  int sCol = (col / 3) * 3;
  for (int i = sRow; i < sRow + 3; i++)
  {
    for (int j = sCol; j < sCol + 3; j++)
    {
      if (board[i][j] == val)
      {
        return false;
      }
    }
  }

  return true;
}

bool solve(vector<vector<char>> &board, int n)
{
  for (int i = 0; i < n; i++)
  {
    for (int j = 0; j < n; j++)
    {
      if (board[i][j] == '.')
      {
        for (char val = '1'; val <= '9'; val++)
        {
          if (isPossible(board, val, i, j, n))
          {
            board[i][j] = val;
            if (solve(board, n))
              return true;
            board[i][j] = '.';
          }
        }

        return false;
      }
    }
  }

  return true;
}

void solveSudoku(vector<vector<char>> &board)
{
  int n = board.size();

  solve(board, n);
}
```

## Complexity

Let `E` be the number of empty cells (`E <= 81`).

### Time — `O(n^2 * 9^E)`

Each empty cell branches up to `9` ways → `O(9^E)` states. Every recursive call rescans the board (`O(n^2)`) to find the next empty cell; `isPossible` is `O(n)`, dominated by the scan. For the fixed `9 x 9` board `n` is constant, so this simplifies to `O(9^E)`.

### Space — `O(E)`

Recursion depth is at most `E`; the board is edited in place with no extra structures. For a `9 x 9` board this is effectively `O(1)`.

---

## Edge Cases

- Board with only one empty cell → single digit trial.
- The problem guarantees a **unique solution**, so the first `true` returned is the answer; no need to search further.
- Box-origin arithmetic `(row/3)*3, (col/3)*3` must use integer division — critical for locating the correct `3 x 3` block.

## Key Takeaways

- Returning `bool` up the recursion is the clean way to **stop as soon as a solution is found** — contrast with N-Queens/Rat-in-a-Maze which enumerate *all* solutions and therefore return `void` and keep searching.
- Box index from cell: `sRow = (row/3)*3`, `sCol = (col/3)*3` — a reusable pattern for any grid partitioned into fixed blocks.
- The **choose → recurse → undo** template again; the `board[i][j] = '.'` reset is the undo step.
- Constraint checking can be sped to `O(1)` by maintaining `rowUsed[9][9]`, `colUsed[9][9]`, `boxUsed[9][9]` bitsets — a common follow-up optimization.

## Common Mistakes

- Not resetting `board[i][j] = '.'` on failure, corrupting the board for sibling attempts.
- Returning `void` and continuing to search after the solution is found (wastes time; may overwrite a solved cell).
- Computing the box origin without integer flooring (e.g. `row/3*3` mis-parenthesized) so the wrong `3 x 3` block is validated.
- Forgetting the "no empty cell left → return true" terminal case.

## Interview Questions

1. Why does `solve` return `bool` here but N-Queens' `solve` returns `void`?
2. Derive the `3 x 3` box origin formula for an arbitrary cell.
3. How would you make `isPossible` run in `O(1)` instead of `O(n)`?
4. Would a "most-constrained cell first" (minimum-remaining-values) heuristic help, and why?
5. What changes if the board is not guaranteed to have a unique (or any) solution?

## Quick Revision

| Approach | Time | Space (aux) | Core Idea |
| -------- | ---- | ----------- | --------- |
| Backtracking | `O(n^2 * 9^E)` (≈ `O(9^E)`) | `O(E)` (≈ `O(1)`) | Fill first empty cell with a legal digit, recurse, undo on dead end |

### 30-Second Recall

```text
Problem:
Fill a 9x9 Sudoku so every row/col/3x3-box has 1-9 once. In place.

Idea:
Find first '.', try 1-9 that don't clash, recurse; undo if the branch fails.
Return bool so success short-circuits the whole search.

Check:
row scan + col scan + 3x3 box (origin = (row/3)*3, (col/3)*3).

TC:
O(9^E), E = empty cells (n^2 scan per state on fixed 9x9).

SC:
O(E) recursion depth, in-place board.

Key trap:
Reset the cell to '.' on failure; return bool (not void) to stop at first solution.
```
