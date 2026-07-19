# N-Queens

**Problem Link:** https://leetcode.com/problems/n-queens/
**Platform:** LeetCode
**Difficulty:** Hard
**Topic:** Backtracking
**Pattern:** Place-one-per-row backtracking with constraint checking (columns + both diagonals)

---

## Problem

Place `n` queens on an `n x n` chessboard so that **no two queens attack each other** (no shared row, column, or diagonal). Return all distinct board configurations, each as a list of strings where `'Q'` is a queen and `'.'` is empty.

### Example 1

**Input:**

```text
n = 4
```

**Output:**

```text
[[".Q..",
  "...Q",
  "Q...",
  "..Q."],

 ["..Q.",
  "Q...",
  "...Q",
  ".Q.."]]
```

**Explanation:**

There are exactly two distinct solutions for `n = 4`.

### Example 2

**Input:**

```text
n = 1
```

**Output:**

```text
[["Q"]]
```

### Constraints

```text
1 <= n <= 9
```

---

## Core Idea

Because no two queens can share a row, we place **exactly one queen per row** and recurse row by row. For each row we try every column, keep only "safe" placements, recurse to the next row, then **undo** the placement (backtrack). A full board (`row == n`) is one solution.

```text
Decision tree for row-by-row placement (n = 4, pruned to safe moves):

row 0:      col0        col1        col2        col3
             |           |           |           |
row 1:   (unsafe..)   col3        col0        (..)
                        |           |
row 2:              col0        col3
                        |           |
row 3:              col2        col1
                     SOLUTION    SOLUTION
```

The two approaches differ only in **how fast the safety check is**.

---

# Approach 1 — Brute Force (Backtracking with `O(n)` safety check)

## Intuition

For a candidate cell `(row, col)`, since rows are filled top-to-bottom, only queens **above** it can conflict. So `isSafe` scans upward along the same column, the top-left diagonal, and the top-right diagonal. Each check is `O(n)`.

## Algorithm

1. `solve(row)`: if `row == n`, record the board.
2. For each `col`, if `isSafe(row, col)`, place `'Q'`, recurse `solve(row+1)`, then reset to `'.'`.
3. `isSafe` returns false if any `'Q'` is found going up the column or up either diagonal.

## Visualization

```text
Checking (row, col): only cells ABOVE matter (rows below are empty).

  \   |   /
   \  |  /
    \ | /
      X   <- (row, col)

Scan: up-column (|), up-left diagonal (\), up-right diagonal (/).
```

## Code

```cpp
// BRUTE FORCE - Backtracking
bool isSafe(int row, int col, vector<string> &board, int n)
{
    // same column
    for (int i = row; i >= 0; i--)
    {
        if (board[i][col] == 'Q')
            return false;
    }

    // same diagonal - top left
    for (int i = row - 1, j = col - 1; i >= 0 && j >= 0; i--, j--)
    {
        if (board[i][j] == 'Q')
            return false;
    }

    // same diagonal - top right
    for (int i = row, j = col; i >= 0 && j < n; i--, j++)
    {
        if (board[i][j] == 'Q')
            return false;
    }
    return true;
}

void solve(int row, vector<string> &board, vector<vector<string>> &res, int n)
{
    if (row == n)
    {
        res.push_back(board);
        return;
    }

    for (int col = 0; col < n; col++)
    {
        if (isSafe(row, col, board, n))
        {
            board[row][col] = 'Q';
            solve(row + 1, board, res, n);
            board[row][col] = '.';
        }
    }
}

vector<vector<string>> solveNQueens(int n)
{
    vector<vector<string>> res;
    vector<string> board(n, string(n, '.'));

    solve(0, board, res, n);

    return res;
}
```

## Complexity

### Time — `O(n * n!)`

Row-by-row placement with the column constraint explores at most `O(n!)` arrangements. Each placement runs an `O(n)` safety scan → `O(n * n!)`.

### Space — `O(n^2)` auxiliary, `O(s * n^2)` result

The board is `O(n^2)` and recursion depth is `O(n)`. Storing `s` solutions of size `n*n` gives `O(s * n^2)` result space.

---

# Approach 2 — Optimal (Backtracking with column & diagonal hashing)

## Intuition

The `O(n)` safety scan is replaced by three lookup arrays so each check is `O(1)`:

- `cols[col]` — is this column already used?
- `leftDia[row - col + n - 1]` — cells on the same "top-left → bottom-right" diagonal share `(row - col)`; the `+ n - 1` offset keeps the index non-negative.
- `rightDia[row + col]` — cells on the same "top-right → bottom-left" diagonal share `(row + col)`.

Place → mark all three, recurse, then unmark on backtrack.

## Visualization

```text
Diagonal identity keys (n = 4):

row - col (+n-1 offset)          row + col
   3 2 1 0                          0 1 2 3
   4 3 2 1                          1 2 3 4
   5 4 3 2                          2 3 4 5
   6 5 4 3                          3 4 5 6

Same "\" diagonal -> same (row - col).   Same "/" diagonal -> same (row + col).
```

## Code

```cpp
// OPTIMIZED - Backtracking with Column and Diagonal Hashing
void solve(int row, vector<string> &board, vector<int> &cols, vector<int> &leftDia, vector<int> &rightDia, vector<vector<string>> &res, int n)
{
    if (row == n)
    {
        res.push_back(board);
        return;
    }

    for (int col = 0; col < n; col++)
    {
        if (cols[col] || leftDia[row - col + n - 1] || rightDia[row + col])
        {
            continue;
        }

        cols[col] = 1;
        leftDia[row - col + n - 1] = 1;
        rightDia[row + col] = 1;

        board[row][col] = 'Q';
        solve(row + 1, board, cols, leftDia, rightDia, res, n);
        board[row][col] = '.';

        cols[col] = 0;
        leftDia[row - col + n - 1] = 0;
        rightDia[row + col] = 0;
    }
}

vector<vector<string>> solveNQueens(int n)
{
    vector<vector<string>> res;
    vector<string> board(n, string(n, '.'));

    vector<int> col(n, 0);
    vector<int> leftDia(n * 2, 0);
    vector<int> rightDia(n * 2, 0);

    solve(0, board, col, leftDia, rightDia, res, n);

    return res;
}
```

## Complexity

### Time — `O(n!)`

Same `O(n!)` search space, but each placement/safety check is now `O(1)` → `O(n!)`.

### Space — `O(n^2)` auxiliary, `O(s * n^2)` result

Board is `O(n^2)`; the three marker arrays are `O(n)` each; recursion depth `O(n)`. Result space `O(s * n^2)`.

---

## Edge Cases

- `n = 1` → single solution `[["Q"]]`.
- `n = 2` and `n = 3` → no solutions (return empty list); the backtracking naturally yields none.
- Diagonal index bounds: `row - col + n - 1` ranges `0 .. 2n-2` and `row + col` ranges `0 .. 2n-2`, so arrays of size `2n` are safe.

## Key Takeaways

- **One-choice-per-row** collapses an `n^2`-cell search into `n` decisions and automatically enforces the row constraint.
- Cells on a `\` diagonal share `row - col`; cells on a `/` diagonal share `row + col`. These identities turn diagonal conflict checks into `O(1)` array lookups — a reusable trick for any grid-diagonal problem.
- The classic backtracking template: **choose → mark → recurse → unmark**. Every state you mark on the way down must be unmarked on the way up.
- Precomputed constraint sets convert an `O(n)` validity check into `O(1)`, dropping total time by a factor of `n`.

## Common Mistakes

- Forgetting the `+ n - 1` offset on the `\` diagonal, producing negative indices.
- Not unmarking `cols`/`leftDia`/`rightDia` (or the board cell) during backtracking, corrupting sibling branches.
- Checking cells below the current row (unnecessary — they're empty) or scanning full diagonals in the optimal version (defeats the `O(1)` gain).

## Interview Questions

1. Why is it enough to place one queen per row?
2. How do `row - col` and `row + col` identify the two diagonal families?
3. Why does the offset `+ n - 1` appear on the left-diagonal index?
4. What is the essential difference in time complexity between the two approaches, and where does the factor of `n` go?
5. How would you count solutions only (without building boards) to save memory?

## Quick Revision

| Approach | Time | Space (aux) | Core Idea |
| -------- | ---- | ----------- | --------- |
| Brute | `O(n * n!)` | `O(n^2)` | Backtrack row by row; `O(n)` scan checks column + 2 diagonals |
| Optimal | `O(n!)` | `O(n^2)` | Same search, `O(1)` checks via `cols`/`leftDia`/`rightDia` hashing |

### 30-Second Recall

```text
Problem:
Place n non-attacking queens; return all boards.

Idea:
One queen per row -> recurse row by row, try each column, backtrack.

Brute:
isSafe scans up-column + both up-diagonals in O(n).

Optimal:
cols[col], leftDia[row-col+n-1], rightDia[row+col] -> O(1) checks.

TC:
Brute O(n*n!), Optimal O(n!).

SC:
O(n^2) aux (board), O(s*n^2) result.

Key trap:
Unmark every marker on backtrack; use row-col (\) and row+col (/) for diagonals.
```
