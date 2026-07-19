# Upper Bound

**Problem Link:** Not available in original file
**Topic:** Binary Search
**Pattern:** Binary search for a boundary (first index strictly greater than target)

---

## Problem

Given a **sorted** (ascending) array `arr` and a `target`, return the **upper bound**: the smallest index `i` such that `arr[i] > target` (strictly greater). If no such element exists (all elements are `<= target`), return `n` (the array length — a valid insertion position just past the end).

### Example 1

**Input:**

```text
arr = [1, 2, 2, 3], target = 2
```

**Output:**

```text
3
```

**Explanation:**

The first index where `arr[i] > 2` is index `3` (`arr[3] = 3`).

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

`arr[3] = 15` is the first element strictly greater than `9`.

### Example 3

**Input:**

```text
arr = [1, 2, 3], target = 3
```

**Output:**

```text
3
```

**Explanation:**

No element is `> 3`, so the answer is `n = 3`.

---

## Intuition

The predicate `arr[i] > target` is **monotonic** on a sorted array: `F F F ... F T T ... T`. Upper bound is the **first `T`**. It differs from lower bound only in the comparison: upper bound uses **strict** `>`, so it skips _past_ all elements equal to the target, whereas lower bound (`>=`) stops at the first equal element.

## Algorithm

1. `idx = n` (default when no valid index exists).
2. `s = 0`, `e = n-1`.
3. While `s <= e`, `mid = s + (e - s) / 2`:
   - If `arr[mid] <= target`: cannot be the upper bound, search right (`s = mid + 1`).
   - Else (`arr[mid] > target`): store `idx = mid`, search left (`e = mid - 1`).
4. Return `idx`.

## Visualization — monotonic answer space

```text
arr = [1, 2, 2, 3], target = 2

predicate arr[i] > 2 :  F  F  F  T
                                 ^
                          first feasible (upper bound) = index 3

s=0 e=3 mid=1 arr=2 <=2 -> s=2
s=2 e=3 mid=2 arr=2 <=2 -> s=3
s=3 e=3 mid=3 arr=3  >2 -> idx=3, e=2
s=3 e=2 -> loop ends -> idx = 3
```

Compare with lower bound on the same input (`arr[i] >= 2` → `F T T T`, answer `1`). The band of equal values `[1, 2]` sits exactly between `lowerBound = 1` and `upperBound = 3`, so it contains `3 - 1 = 2` twos.

## Code

```cpp
// BINARY SEARCH - Find Upper Bound
int upperBound(vector<int> &arr, int target)
{
  int n = arr.size();
  int idx = n;

  int s = 0, e = n - 1;
  while (s <= e)
  {
    int mid = s + (e - s) / 2;

    if (arr[mid] <= target)
    {
      s = mid + 1;
    }
    else if (arr[mid] > target)
    {
      idx = mid;
      e = mid - 1;
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

- `target` smaller than all elements → upper bound is `0`.
- `target` greater than or equal to all elements → returns `n` (the `idx = n` default).
- `target` equals elements → returns the index **just after** the last equal element.
- Array of all-identical values equal to target → returns `n`.

## Key Takeaways

- **Upper bound = first index where `arr[i] > target`** (strict). **Lower bound = first index where `arr[i] >= target`.** The _only_ code difference is `>` vs `>=`.
- `upperBound(target) - lowerBound(target)` = **count of occurrences** of `target` in the sorted array — a hugely reusable identity (frequency counts, range queries).
- `lowerBound(target)` also gives the correct **insertion index** to keep the array sorted; `upperBound` gives the insertion index that places the new element _after_ existing duplicates.
- Same boundary-search skeleton as lower bound: record candidate on `T`, shrink toward the earliest `T`.

## Common Mistakes

- Using `>=` (that computes lower bound) instead of strict `>`.
- Returning `-1` instead of `n` for the not-found case, breaking insertion/count semantics.
- Returning immediately on the first `arr[mid] > target` instead of continuing left to find the _first_ such index.
- Confusing which of lower/upper bound to subtract when counting occurrences (`upper - lower`, not the reverse).

## Interview Questions

1. What single character differs between lower bound and upper bound, and what does it change?
2. How do you count occurrences of a value using both bounds?
3. What does upper bound return when the target is absent, and how does that relate to lower bound?
4. Why is the not-found answer `n` and not `-1`?
5. How does `std::upper_bound` relate to this implementation?

## Quick Revision

| Approach      | Time       | Space  | Core Idea                                                         |
| ------------- | ---------- | ------ | ----------------------------------------------------------------- |
| Binary Search | `O(log n)` | `O(1)` | First index with `arr[i] > target`; record candidate, shrink left |

### 30-Second Recall

```text
Problem:
Smallest index i with arr[i] > target (strict), else n.

Idea:
Predicate F...FT...T -> find first T.
arr[mid] <= target -> go right; arr[mid] > target -> idx=mid, go left.

Identity:
upperBound - lowerBound = count of target's occurrences.

TC:
O(log n).

SC:
O(1).

Key trap:
Use strict > (not >=); default idx = n; keep going left after the first T.
```
