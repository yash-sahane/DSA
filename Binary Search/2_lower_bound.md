# Lower Bound

**Problem Link:** https://www.geeksforgeeks.org/problems/implement-lower-bound/1
**Platform:** GeeksforGeeks
**Topic:** Binary Search
**Pattern:** Binary search for a boundary (first index satisfying a predicate)

---

## Problem

Given a **sorted** (ascending) array `arr` and a `target`, return the **lower bound**: the smallest index `i` such that `arr[i] >= target`. If no such element exists (all elements are smaller than `target`), return `n` (the array length — a valid insertion position).

### Example 1

**Input:**

```text
arr = [1, 2, 2, 3], target = 2
```

**Output:**

```text
1
```

**Explanation:**

The first index where `arr[i] >= 2` is index `1`.

### Example 2

**Input:**

```text
arr = [3, 5, 8, 15, 19], target = 9
```

**Output:**

```text
3
```

**Explanation:**

`arr[3] = 15` is the first element `>= 9`.

### Example 3

**Input:**

```text
arr = [1, 2, 3], target = 5
```

**Output:**

```text
3
```

**Explanation:**

No element is `>= 5`, so the answer is `n = 3`.

### Constraints

```text
1 <= n <= 10^5
1 <= arr[i], target <= 10^6
arr is sorted in non-decreasing order.
```

---

## Intuition

The condition `arr[i] >= target` is **monotonic** on a sorted array: once it becomes true, it stays true for all larger indices. So the array looks like `F F F ... F T T ... T`, and we want the **first `T`**. Binary search for that boundary: whenever `arr[mid] >= target`, `mid` is a *candidate* answer — record it and keep searching **left** for something even earlier. Otherwise move right.

## Algorithm

1. `idx = n` (default when no valid index exists).
2. `s = 0`, `e = n-1`.
3. While `s <= e`, `mid = s + (e - s) / 2`:
   - If `arr[mid] >= target`: store `idx = mid`, search left (`e = mid - 1`).
   - Else search right (`s = mid + 1`).
4. Return `idx`.

## Visualization — monotonic answer space

```text
arr = [3, 5, 8, 15, 19], target = 9

predicate arr[i] >= 9 :  F  F  F  T  T
                                  ^
                             first feasible (lower bound) = index 3

s=0 e=4 mid=2 arr=8  <9 -> s=3
s=3 e=4 mid=3 arr=15 >=9 -> idx=3, e=2
s=3 e=2 -> loop ends -> idx = 3
```

## Code

```cpp
// BINARY SEARCH - Find Lower Bound
int lowerBound(vector<int> &arr, int target)
{
  int n = arr.size();
  int idx = n;

  int s = 0, e = n - 1;
  while (s <= e)
  {
    int mid = s + (e - s) / 2;

    if (arr[mid] >= target)
    {
      idx = mid;
      e = mid - 1;
    }
    else if (arr[mid] < target)
    {
      s = mid + 1;
    }
  }

  return idx;
}
```

## Complexity

### Time — `O(log n)`

The search space halves each iteration.

### Space — `O(1)`

Only scalars `n`, `idx`, `s`, `e`, `mid`.

---

## Edge Cases

- `target` smaller than all elements → lower bound is `0`.
- `target` larger than all elements → returns `n` (the `idx = n` default).
- `target` equals an element → returns the **first** occurrence (because `>=` keeps moving left on equality).
- Duplicates of the target → returns the leftmost matching index.

## Key Takeaways

- **Lower bound = first index where `arr[i] >= target`.** Contrast upper bound = first index where `arr[i] > target`.
- The moment a search has a **monotonic predicate** (`F...FT...T`), binary search finds the boundary in `O(log n)` — you don't need an exact-match target.
- The "store candidate, then keep shrinking toward the boundary" pattern generalizes to countless "first/last index that satisfies P" problems (binary search on answer).
- Initializing `idx = n` elegantly encodes "no valid index / insertion at end".

## Common Mistakes

- Using `>` instead of `>=`, which computes upper bound instead.
- Returning `-1` instead of `n` for the not-found case (breaks the insertion-position semantics).
- On a match, returning immediately instead of continuing left — that yields *an* occurrence, not necessarily the *first*.
- Off-by-one in bound updates causing an infinite loop.

## Interview Questions

1. What is the precise difference between lower bound and upper bound?
2. Why do we keep searching left after finding `arr[mid] >= target`?
3. Why is the not-found answer `n` rather than `-1`?
4. How do lower/upper bound together give the count of occurrences of a value? (`upper - lower`.)
5. How would you implement this with `std::lower_bound`?

## Quick Revision

| Approach | Time | Space | Core Idea |
| -------- | ---- | ----- | --------- |
| Binary Search | `O(log n)` | `O(1)` | First index with `arr[i] >= target`; record candidate, shrink left |

### 30-Second Recall

```text
Problem:
Smallest index i with arr[i] >= target (else n).

Idea:
Predicate F...FT...T -> find first T.
arr[mid] >= target -> idx=mid, go left; else go right.

TC:
O(log n).

SC:
O(1).

Key trap:
Use >= (not >); default idx = n; keep going left on a match to get the FIRST index.
```
