# Set Matrix Zeroes

**Problem Link:** https://leetcode.com/problems/set-matrix-zeroes/
**Platform:** LeetCode
**Difficulty:** Medium
**Topic:** Array / Matrix
**Pattern:** In-place matrix marking (use the matrix itself as auxiliary storage)

---

## Problem

Given an `n x m` integer matrix, if an element is `0`, set its **entire row and entire column** to `0`. The update must be done **in place**.

The trap: you cannot naively set zeroes as you scan, because a freshly written `0` would then trigger *more* rows/columns to be zeroed, corrupting the answer. The zeroing decision must be based only on the **original** zeroes.

### Example 1

**Input:**

```text
matrix = [[1,1,1],
          [1,0,1],
          [1,1,1]]
```

**Output:**

```text
[[1,0,1],
 [0,0,0],
 [1,0,1]]
```

**Explanation:**

The only original `0` is at `(1,1)`, so row `1` and column `1` become `0`.

### Example 2

**Input:**

```text
matrix = [[0,1,2,0],
          [3,4,5,2],
          [1,3,1,5]]
```

**Output:**

```text
[[0,0,0,0],
 [0,4,5,0],
 [0,3,1,0]]
```

**Explanation:**

Original zeroes are at `(0,0)` and `(0,3)`, so rows `{0}` and columns `{0,3}` become `0`.

### Constraints

```text
1 <= n, m <= 200
-2^31 <= matrix[i][j] <= 2^31 - 1
```

---

# Approach 1 — Brute Force (Mark with a sentinel `-1`)

## Intuition

The core difficulty is telling apart an **original** `0` from a `0` we just wrote. The brute force sidesteps this by not writing `0` during the marking pass at all — instead it marks every cell that *should become* `0` with a sentinel value `-1` (only overwriting non-zero cells, so real zeroes stay `0`). A final pass turns every `-1` into `0`.

This works but is wasteful: for each original `0` we walk its whole row **and** whole column, repeating that work for every zero.

> Note: this implementation also declares `rows` and `cols` vectors that go unused — they are leftover scaffolding and do not affect correctness. The marking is done entirely via the `-1` sentinel.

## Algorithm

1. Scan every cell.
2. When a `0` is found at `(i, j)`, mark all non-zero cells in row `i` and column `j` with `-1`.
3. After scanning, replace every `-1` with `0`.

## Visualization

```text
Original            After marking (-1)     After -1 -> 0
1 1 1               -1  0 -1                1 0 1
1 0 1        ->      0  0  0        ->      0 0 0
1 1 1               -1  0 -1                1 0 1
```

The `-1` sentinel lets the second decision pass distinguish "will become 0" from an original `0` while scanning.

## Code

```cpp
// BRUTE FORCE - Mark Rows and Columns using Temporary Value (-1)
void setMatrixRow(int row, vector<vector<int>> &matrix, int m)
{
  for (int col = 0; col < m; col++)
  {
    if (matrix[row][col] != 0)
      matrix[row][col] = -1;
  }
}

void setMatrixCol(int col, vector<vector<int>> &matrix, int n)
{
  for (int row = 0; row < n; row++)
  {
    if (matrix[row][col] != 0)
      matrix[row][col] = -1;
  }
}

void setZeroes(vector<vector<int>> &matrix)
{
  int n = matrix.size();
  int m = matrix[0].size();

  vector<int> rows(n, 1);
  vector<int> cols(m, 1);

  for (int i = 0; i < n; i++)
  {
    for (int j = 0; j < m; j++)
    {
      if (matrix[i][j] == 0)
      {
        setMatrixRow(i, matrix, m);
        setMatrixCol(j, matrix, n);
      }
    }
  }

  for (int i = 0; i < n; i++)
  {
    for (int j = 0; j < m; j++)
    {
      if (matrix[i][j] == -1)
      {
        matrix[i][j] = 0;
      }
    }
  }
}
```

> ⚠️ Caveat: The `-1` sentinel only works because the problem's values are being treated as non-negative here. If the matrix can legitimately contain `-1`, a different sentinel (or the Better/Optimal approach) is required. LeetCode's constraints allow negative values, so prefer the approaches below in an interview.

## Complexity

### Time — `O(n * m * (n + m))`

Every cell is visited once (`O(n*m)`). Each original `0` triggers a full row walk (`O(m)`) plus a full column walk (`O(n)`). In the worst case (all zeroes) this compounds to `O(n*m*(n+m))`.

### Space — `O(1)`

Marking is done inside the matrix with the `-1` sentinel; no auxiliary structure is truly needed (the declared `rows`/`cols` are unused).

---

# Approach 2 — Better (Row & Column Hashing)

## Intuition

Instead of re-walking a row/column every time we see a `0`, record **once** which rows and which columns contain a zero. Two boolean-style arrays `rows[]` and `cols[]` remember this. A second pass zeroes any cell whose row or column was flagged.

This removes the repeated row/column walks of the brute force, dropping time from cubic-ish to linear in the number of cells.

## Algorithm

1. Initialize `rows[n]` and `cols[m]` to `1` (meaning "keep").
2. First pass: for every `0` at `(i, j)`, set `rows[i] = 0` and `cols[j] = 0`.
3. Second pass: set `matrix[i][j] = 0` whenever `rows[i] == 0` **or** `cols[j] == 0`.

## Visualization

```text
matrix          rows[]   cols[]
1 1 1             1        1 0 1
1 0 1     ->      0
1 1 1             1

Zero cell (i,j) whenever rows[i]==0 OR cols[j]==0.
```

## Code

```cpp
// BETTER - Row and Column Hashing
void setZeroes(vector<vector<int>> &matrix)
{
  int n = matrix.size();
  int m = matrix[0].size();

  vector<int> rows(n, 1);
  vector<int> cols(m, 1);

  for (int i = 0; i < n; i++)
  {
    for (int j = 0; j < m; j++)
    {
      if (matrix[i][j] == 0)
      {
        rows[i] = 0;
        cols[j] = 0;
      }
    }
  }

  for (int i = 0; i < n; i++)
  {
    for (int j = 0; j < m; j++)
    {
      if (rows[i] == 0 || cols[j] == 0)
      {
        matrix[i][j] = 0;
      }
    }
  }
}
```

## Complexity

### Time — `O(n * m)`

Two independent passes over all cells: `O(n*m) + O(n*m) = O(n*m)`.

### Space — `O(n + m)`

The `rows[]` and `cols[]` marker arrays.

---

# Approach 3 — Optimal (First Row & First Column as Markers)

## Intuition

The `O(n+m)` markers of the Better approach can be folded **into the matrix itself**. Use row `0` as the `cols[]` array and column `0` as the `rows[]` array. The only clash is cell `(0,0)`, which is shared by both — so a single extra variable `col0` separately tracks whether the **first column** must be zeroed.

Key ordering insight: during the writing pass we iterate **bottom-up and right-to-left**, so the marker cells in row `0` / column `0` are read before they get overwritten.

## Algorithm

1. `col0 = 1`. Scan every row `i`:
   - If `matrix[i][0] == 0`, set `col0 = 0` (first column must be zeroed).
   - For `j` from `1` to `m-1`: if `matrix[i][j] == 0`, mark `matrix[i][0] = 0` and `matrix[0][j] = 0`.
2. Traverse from bottom-right to top-left. For `j >= 1`, zero `matrix[i][j]` if `matrix[i][0] == 0` or `matrix[0][j] == 0`.
3. After finishing a row, if `col0 == 0`, zero `matrix[i][0]`.

Why `col0` is separate: `matrix[0][0]` doubles as both the row-0 marker and column-0 marker. Overloading it would lose information, so the first column's fate is stored in `col0` and applied last per row.

## Visualization

```text
Markers live inside the matrix:

   c0 -> column 0 marks each ROW
   |
   v
 [ (0,0) 0 0 0 ]  <- row 0 marks each COLUMN
 [   0        ]
 [   0        ]

(0,0) is shared -> first column tracked separately by `col0`.
Write pass goes bottom-up, right-to-left so markers survive until read.
```

## Code

```cpp
// OPTIMIZED - First Row and First Column as Markers
void setZeroes(vector<vector<int>> &matrix)
{
  int n = matrix.size();
  int m = matrix[0].size();

  int col0 = 1;

  for (int i = 0; i < n; i++)
  {
    if (matrix[i][0] == 0)
      col0 = 0;
    for (int j = 1; j < m; j++)
    {
      if (matrix[i][j] == 0)
      {
        matrix[i][0] = 0;
        matrix[0][j] = 0;
      }
    }
  }

  for (int i = n - 1; i >= 0; i--)
  {
    for (int j = m - 1; j >= 1; j--)
    {
      if (matrix[i][0] == 0 || matrix[0][j] == 0)
      {
        matrix[i][j] = 0;
      }
    }
    if (col0 == 0)
      matrix[i][0] = 0;
  }
}
```

## Complexity

### Time — `O(n * m)`

Two passes over all cells, all marker checks are `O(1)`.

### Space — `O(1)`

Only the single `col0` variable is auxiliary; markers are stored in the matrix's own first row/column.

---

## Edge Cases

- A `0` in the **first row or first column** — this is exactly why `col0` exists and why the write pass runs right-to-left / bottom-up.
- Single-row or single-column matrix.
- No zeroes at all — matrix must be returned unchanged.
- Entire matrix is zero.

## Key Takeaways

- When asked to reduce `O(n+m)` extra space to `O(1)`, ask: **can the input itself store the metadata?** Here the first row/column become the marker arrays.
- Overlapping markers (cell `(0,0)`) need one extra scalar to disambiguate.
- Traversal **direction matters** when you read and write the same cells — go in the order that consumes markers before clobbering them.
- Sentinel-value tricks (`-1`) are fragile when the sentinel is a legal input value.

## Common Mistakes

- Zeroing cells during the first scan, which cascades and corrupts later decisions.
- In the optimal approach, iterating top-to-bottom / left-to-right in the write pass, destroying markers before they are read.
- Forgetting the `(0,0)` overlap and zeroing the first column incorrectly.
- Using `-1` as a sentinel when the matrix may actually contain `-1`.

## Interview Questions

1. Why can't you set a cell to `0` immediately when you find a `0`?
2. How does using the first row/column as storage create a conflict, and how is it resolved?
3. Why must the optimal write pass go bottom-up and right-to-left?
4. What breaks in the brute-force sentinel approach if the matrix legally contains the sentinel value?

## Quick Revision

| Approach | Time | Space | Core Idea |
| -------- | ---- | ----- | --------- |
| Brute | `O(n*m*(n+m))` | `O(1)` | Mark to-be-zeroed cells with sentinel `-1`, then convert |
| Better | `O(n*m)` | `O(n+m)` | `rows[]`/`cols[]` flag which lines contain a zero |
| Optimal | `O(n*m)` | `O(1)` | First row/column act as the marker arrays; `col0` scalar for overlap |

### 30-Second Recall

```text
Problem:
If matrix[i][j] == 0, zero its whole row and column, in place.

Brute:
Mark should-be-zero cells with -1 (skip real 0s), then -1 -> 0.

Better:
rows[] and cols[] remember which lines had a 0; second pass applies them.

Optimal:
Use row 0 and col 0 as those marker arrays; col0 scalar handles the (0,0) clash.
Write bottom-up, right-to-left.

TC:
Brute O(n*m*(n+m)), Better/Optimal O(n*m).

SC:
Brute O(1) (sentinel), Better O(n+m), Optimal O(1).

Key trap:
Don't cascade fresh zeroes; direction of the write pass matters in the optimal version.
```
