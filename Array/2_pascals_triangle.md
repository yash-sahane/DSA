# Pascal's Triangle

**Problem Link:** https://leetcode.com/problems/pascals-triangle/
**Platform:** LeetCode
**Difficulty:** Easy
**Topic:** Array / Math
**Pattern:** Build-from-previous-row (each row derived from the one above)

---

## Problem

Given an integer `numRows`, return the first `numRows` of **Pascal's Triangle**. In Pascal's Triangle each number is the sum of the two numbers directly above it; the edges are always `1`.

### Example 1

**Input:**

```text
numRows = 5
```

**Output:**

```text
[[1],
 [1,1],
 [1,2,1],
 [1,3,3,1],
 [1,4,6,4,1]]
```

**Explanation:**

Every interior value `row[j]` equals `prevRow[j-1] + prevRow[j]`; the first and last entries of each row are `1`.

### Example 2

**Input:**

```text
numRows = 1
```

**Output:**

```text
[[1]]
```

### Constraints

```text
1 <= numRows <= 30
```

---

## Visualization

Each interior cell is the sum of the two cells above it:

```text
row0:        1
row1:       1 1
row2:      1 2 1        2 = 1 + 1
row3:     1 3 3 1       3 = 1 + 2 , 3 = 2 + 1
row4:    1 4 6 4 1      6 = 3 + 3
```

Both approaches below build each new row from the previous one: pad with leading/trailing `1`, and fill each interior position from adjacent pairs of the prior row.

---

# Approach 1 — Recursive (row by row)

## Intuition

If you already have row `i`, you can derive row `i+1` directly: it has one more element, its ends are `1`, and each interior element is the sum of two neighbors from row `i`. Recursion just repeats "build the next row from the current one" until the triangle has `numRows` rows.

## Algorithm

1. Base cases: `numRows == 1` returns `{{1}}`. Otherwise seed the result with rows `{1}` and `{1,1}`, and set `curr = {1,1}`.
2. `solve` stops once `curr.size() == numRows`.
3. Otherwise build `temp` of size `curr.size()+1`, all `1`s, then set `temp[i+1] = curr[i] + curr[i+1]` for interior positions.
4. Set `curr = temp`, push it, and recurse.

## Code

```cpp
// RECURSIVE - Build Pascal's Triangle Row by Row
void solve(int n, vector<int> curr, vector<vector<int>> &res)
{
  if (curr.size() == n)
  {
    return;
  }

  vector<int> temp(curr.size() + 1, 1);
  for (int i = 0; i < curr.size() - 1; i++)
  {
    temp[i + 1] = curr[i] + curr[i + 1];
  }

  curr = temp;
  res.push_back(curr);

  solve(n, curr, res);
}

vector<vector<int>> generate(int numRows)
{
  if (numRows == 1)
    return {{1}};

  vector<vector<int>> res({{1}, {1, 1}});
  vector<int> curr({1, 1});

  solve(numRows, curr, res);

  return res;
}
```

## Complexity

### Time — `O(n^2)`

Row `i` costs `O(i)` work; summed over `n` rows: `1 + 2 + ... + n = O(n^2)`.

### Space — `O(n)` auxiliary, `O(n^2)` result

Recursion depth is `O(n)` and each `curr`/`temp` holds a single row (`O(n)`). The returned triangle itself has `1 + 2 + ... + n = O(n^2)` elements.

---

# Approach 2 — Iterative (row by row)

## Intuition

Exactly the same row-building recurrence, but with a `for` loop instead of recursion — which removes the `O(n)` recursion stack. Preferred when you only want to avoid stack usage; the asymptotic time is identical.

## Algorithm

1. Same base/seed as the recursive version (`{{1},{1,1}}`, `curr = {1,1}`).
2. Loop `i` from `2` to `numRows-1`: build `temp` (all `1`s, size `curr.size()+1`), fill interior positions `temp[j+1] = curr[j] + curr[j+1]`, set `curr = temp`, push.

## Code

```cpp
// ITERATIVE - Build Pascal's Triangle Row by Row
vector<vector<int>> generate(int numRows)
{
  if (numRows == 1)
    return {{1}};

  vector<vector<int>> res({{1}, {1, 1}});
  vector<int> curr({1, 1});

  for (int i = 2; i < numRows; i++)
  {
    vector<int> temp(curr.size() + 1, 1);
    for (int j = 0; j < curr.size() - 1; j++)
    {
      temp[j + 1] = curr[j] + curr[j + 1];
    }

    curr = temp;
    res.push_back(curr);
  }

  return res;
}
```

## Complexity

### Time — `O(n^2)`

Outer loop over rows, inner loop over the current row: `1 + 2 + ... + n = O(n^2)`.

### Space — `O(n)` auxiliary, `O(n^2)` result

No recursion stack; `curr`/`temp` hold one row. Result triangle is `O(n^2)`.

---

## Edge Cases

- `numRows == 1` → `{{1}}` (handled explicitly; the seed assumes at least 2 rows otherwise).
- `numRows == 2` → the seed `{{1},{1,1}}` is already complete; the loop/recursion does nothing.
- Values grow large but stay within `int` for `numRows <= 30` (max value is `C(29,14) ≈ 6.7e7`).

## Key Takeaways

- "Result space" (the output you must return) is separate from "auxiliary space" (extra working memory). Interviewers usually mean auxiliary space — here `O(n)`.
- Any recursion that simply repeats "derive next state from current state" converts mechanically to a loop, trading the `O(depth)` stack for `O(1)` control overhead.
- Padding a row with leading/trailing `1`s and summing adjacent pairs is the clean way to express the binomial recurrence `C(i,j) = C(i-1,j-1) + C(i-1,j)`.

## Common Mistakes

- Off-by-one in the interior loop bound (`curr.size() - 1`) — writing past the ends or overwriting the boundary `1`s.
- Forgetting the `numRows == 1` special case, which makes the two-row seed produce a wrong-sized answer.
- Mutating `curr` in place while still reading from it (both versions correctly build a fresh `temp`).

## Interview Questions

1. What is the value at `row i`, `col j` in closed form? (`C(i, j)`.)
2. Can you return just the `k`-th row in `O(k)` space? (Yes — update a single row right-to-left in place.)
3. Why is the auxiliary space `O(n)` and not `O(n^2)`?
4. How would you avoid integer overflow for large `numRows`?

## Quick Revision

| Approach | Time | Space (aux) | Core Idea |
| --------- | ---- | ----------- | --------- |
| Recursive | `O(n^2)` | `O(n)` stack | Build next row from current via adjacent sums, recurse |
| Iterative | `O(n^2)` | `O(n)` | Same recurrence with a loop; no recursion stack |

### 30-Second Recall

```text
Problem:
Return first numRows of Pascal's Triangle.

Recurrence:
row[j] = prevRow[j-1] + prevRow[j], ends are 1.

Brute/Recursive:
Recurse "build next row from current" until size == numRows.

Optimal/Iterative:
Same but with a loop -> drops the O(n) recursion stack.

TC:
O(n^2) (1+2+...+n elements).

SC:
O(n) auxiliary, O(n^2) result.

Key trap:
Handle numRows == 1; keep interior loop bounds correct (don't touch the edge 1s).
```
