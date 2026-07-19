# Rat in a Maze

**Problem Link:** https://www.geeksforgeeks.org/problems/rat-in-a-maze-problem/1
**Platform:** GeeksforGeeks
**Topic:** Backtracking
**Pattern:** Grid path enumeration with visited-marking backtracking

---

## Problem

Given an `n x m` maze of `0`s and `1`s, a rat starts at the top-left cell `(0, 0)` and must reach the bottom-right cell `(n-1, m-1)`. The rat can move in four directions — **D**own, **L**eft, **R**ight, **U**p — but only onto cells containing `1` (a `0` is a wall), and it cannot revisit a cell on the same path. Return **all** paths as strings of the moves taken, in lexicographic order.

### Example 1

**Input:**

```text
maze = [[1, 0, 0, 0],
        [1, 1, 0, 1],
        [1, 1, 0, 0],
        [0, 1, 1, 1]]
```

**Output:**

```text
["DDRDRR", "DRDDRR"]
```

**Explanation:**

Both listed move-sequences lead the rat from `(0,0)` to `(3,3)` stepping only on `1`s without revisiting a cell.

### Example 2

**Input:**

```text
maze = [[1, 0],
        [1, 1]]
```

**Output:**

```text
["DR"]
```

### Constraints

```text
2 <= n, m
maze[i][j] is 0 or 1
maze[0][0] == 1 for any path to exist
```

---

## Intuition

From the current cell, recursively try every direction. A move is invalid if it steps off the grid or onto a `0` (wall or already-visited cell). Mark the current cell as visited (set it to `0`) before recursing so the same path can't loop through it, then **unmark** it (`1`) after all four directions return — this frees the cell for other paths. Reaching the destination records the accumulated path string.

The directions are tried in the order **D, L, R, U**, which is alphabetical, so paths are naturally produced in lexicographic order.

## Algorithm

1. `solve(row, col, path)`:
   - If out of bounds or `maze[row][col] == 0`, return.
   - If `(row, col)` is the destination, push `path` and return.
   - Mark `maze[row][col] = 0` (visited).
   - Recurse Down (`+D`), Left (`+L`), Right (`+R`), Up (`+U`).
   - Unmark `maze[row][col] = 1` (backtrack).

## Visualization

```text
maze (1 = open, 0 = wall):        Directions tried per cell (alphabetical):
  1 0 0 0                              D (row+1)
  1 1 0 1                          L (col-1)  R (col+1)
  1 1 0 0                              U (row-1)
  0 1 1 1

One accepted path "DDRDRR":
 (0,0)-D->(1,0)-D->(2,0) ... -R->(2,1)-D->(3,1)-R->(3,2)-R->(3,3)  DEST

Visited cells are temporarily set to 0 so the path can't cross itself,
then restored to 1 when the recursion unwinds.
```

### Recursion / backtracking sketch

```text
solve(0,0,"")            mark (0,0)
 ├─ D -> solve(1,0,"D")  mark (1,0)
 │       ├─ D -> solve(2,0,"DD") ...
 │       ├─ L -> out of bounds, return
 │       ├─ R -> (1,1) open, continue ...
 │       └─ U -> back to (0,0) which is now 0 (visited), return
 │       unmark (1,0)
 ├─ L -> out of bounds
 ├─ R -> (0,1) is 0 (wall), return
 └─ U -> out of bounds
 unmark (0,0)
```

## Code

```cpp
// BRUTE FORCE - Backtracking
void solve(int row, int col, string path, vector<string> &res, vector<vector<int>> &maze, int n, int m)
{
  if (row < 0 || col < 0 || row >= n || col >= m || maze[row][col] == 0)
  {
    return;
  }

  if (row == n - 1 && col == m - 1)
  {
    res.push_back(path);
    return;
  }

  maze[row][col] = 0;

  // bottom
  solve(row + 1, col, path + 'D', res, maze, n, m);

  // left
  solve(row, col - 1, path + 'L', res, maze, n, m);

  // right
  solve(row, col + 1, path + 'R', res, maze, n, m);

  // top
  solve(row - 1, col, path + 'U', res, maze, n, m);

  maze[row][col] = 1;
}

vector<string> ratInMaze(vector<vector<int>> &maze)
{
  int n = maze.size();
  int m = maze[0].size();

  vector<string> res;

  solve(0, 0, "", res, maze, n, m);

  return res;
}
```

## Complexity

### Time — `O(3^(n*m))`

Backtracking explores all paths. From each cell there are 4 directions, but the cell you just came from is marked visited, leaving at most 3 real choices → `O(3^(n*m))` states, each doing `O(1)` work.

### Space — `O(n*m)` auxiliary, `O(s * n*m)` result

The recursion stack can be as deep as the longest path, `O(n*m)`. Visited marking is done in place (no extra visited grid). Storing `s` paths of length up to `n*m - 1` gives `O(s * n*m)` result space.

---

## Edge Cases

- Start or destination is `0` → the guard returns immediately; no paths (result stays empty).
- No path exists → returns an empty list.
- `1 x 1` maze with `maze[0][0] == 1` → start is already the destination; path is `""`.
- Multiple valid paths → all are collected, in lexicographic order thanks to the D/L/R/U ordering.

## Key Takeaways

- **In-place visited marking** (`cell = 0` on entry, `= 1` on exit) avoids a separate `visited[][]` array — a clean space optimization when you can mutate the grid.
- Direction order controls output order: trying moves alphabetically (`D < L < R < U`) yields lexicographically sorted paths for free.
- The out-of-bounds / wall / visited checks are unified into one guard at the top of the recursion (`maze[row][col] == 0` covers both walls and visited cells).
- Building the `path` string by value (`path + 'D'`) gives each branch its own copy — no manual pop needed for the string, only for the grid marking.

## Common Mistakes

- Not restoring `maze[row][col] = 1` after recursing, which permanently blocks cells for other paths (turns "all paths" into "at most one").
- Checking bounds *after* indexing `maze[row][col]` instead of before — the combined guard must short-circuit on bounds first.
- Wrong direction order when the problem requires lexicographic output.
- Treating the "already visited" case separately from the wall case when a single `== 0` check handles both.

## Interview Questions

1. Why can the visited marker reuse the maze itself instead of a separate array?
2. Why does the branching factor become `3` rather than `4` in the complexity?
3. How does the D/L/R/U ordering guarantee lexicographically sorted output?
4. Why doesn't the `path` string need an explicit backtrack (pop), while the grid does?
5. How would you modify this to return only the **shortest** path (hint: BFS)?

## Quick Revision

| Approach | Time | Space (aux) | Core Idea |
| -------- | ---- | ----------- | --------- |
| Backtracking | `O(3^(n*m))` | `O(n*m)` | DFS all 4 directions; mark visited in place, unmark on backtrack |

### 30-Second Recall

```text
Problem:
All paths of a rat from (0,0) to (n-1,m-1) over 1-cells, moves in D/L/R/U, lexicographic.

Idea:
DFS each direction; guard = out-of-bounds OR cell==0 (wall/visited).
Mark cell 0 on entry, unmark to 1 on exit (backtrack). Dest -> record path.

Order:
Try D, L, R, U (alphabetical) => sorted output.

TC:
O(3^(n*m)) (branching 3 after excluding the parent cell).

SC:
O(n*m) recursion stack; in-place visited; O(s*n*m) result.

Key trap:
Restore the cell to 1 after recursing, or other paths get blocked.
```
