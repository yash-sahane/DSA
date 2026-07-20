# Search Insert Position

**Problem Link:** https://leetcode.com/problems/search-insert-position/
**Platform:** LeetCode
**Difficulty:** Easy
**Topic:** Binary Search
**Pattern:** Binary search for a boundary (lower bound), with an early exact-match return

---

## Problem

Given a **sorted** array of **distinct** integers `nums` and a `target`, return the index of `target` if it exists. If not, return the index where it would be inserted to keep `nums` sorted.

Must run in `O(log n)` time.

### Example 1

**Input:**

```text
nums = [1,3,5,6], target = 5
```

**Output:**

```text
2
```

**Explanation:**

`5` is found at index `2`.

### Example 2

**Input:**

```text
nums = [1,3,5,6], target = 2
```

**Output:**

```text
1
```

**Explanation:**

`2` is not present; inserting it at index `1` keeps `[1,2,3,5,6]` sorted.

### Example 3

**Input:**

```text
nums = [1,3,5,6], target = 7
```

**Output:**

```text
4
```

**Explanation:**

`7` is greater than every element, so it belongs at the end (index `4`).

### Constraints

```text
1 <= nums.length <= 10^4
-10^4 <= nums[i] <= 10^4
nums contains distinct values sorted in ascending order.
-10^4 <= target <= 10^4
```

---

## Intuition

If `target` exists, its index is the answer. If it doesn't exist, the correct insertion point is exactly the **lower bound** of `target` — the first index `i` such that `nums[i] >= target` (everything before stays smaller, `target` slots in right there, everything from `i` onward shifts right). So this is a lower-bound binary search with one shortcut: if the search ever lands exactly on `target`, return immediately instead of continuing to shrink toward the leftmost `>=` position.

## Algorithm

1. `idx = n` (default insertion point — `target` greater than everything).
2. `s = 0`, `e = n - 1`.
3. While `s <= e`, `mid = s + (e - s) / 2`:
   - If `nums[mid] == target`: found — return `mid` directly.
   - Else if `nums[mid] < target`: search right (`s = mid + 1`).
   - Else (`nums[mid] > target`): record `idx = mid`, search left (`e = mid - 1`).
4. Return `idx`.

## Visualization — monotonic answer space

```text
nums = [1, 3, 5, 6], target = 2

predicate nums[i] >= 2 :  F  T  T  T
                              ^
                       first feasible = index 1 (insertion point)

s=0 e=3 mid=1 nums=3 >2 -> idx=1, e=0
s=0 e=0 mid=0 nums=1 <2 -> s=1
s=1 e=0 -> loop ends -> idx = 1
```

Because `nums` has distinct values, this is identical in shape to [lower bound](2_lower_bound.md) — the only difference is the `==` short-circuit for an early exact return.

## Code

```cpp
int searchInsert(vector<int> &nums, int target)
{
  int n = nums.size();

  int idx = n;
  int s = 0, e = n - 1;
  while (s <= e)
  {
    int mid = s + (e - s) / 2;

    if (nums[mid] == target)
    {
      return mid;
    }
    else if (nums[mid] < target)
    {
      s = mid + 1;
    }
    else
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

Standard binary search; the search space halves each iteration.

### Space — `O(1)`

Only scalars `n`, `idx`, `s`, `e`, `mid`.

---

## Edge Cases

- `target` smaller than all elements → insertion index `0`.
- `target` greater than all elements → returns `n` (the `idx = n` default, never overwritten).
- `target` present in the array → returned immediately via the `==` branch, no reliance on `idx`.
- `n == 1` → single comparison decides between `0` and `1`.

## Key Takeaways

- **Insertion point for a value not present = its lower bound** (`nums[i] >= target`). This problem is lower bound with a same-value short-circuit.
- Default `idx = n` handles "goes at the very end" without any special-case code.
- Recording `idx = mid` on every `>` step and continuing left is the same "record candidate, shrink toward the earliest match" skeleton used in [lower bound](2_lower_bound.md) / [upper bound](3_upper_bound.md).

## Common Mistakes

- Forgetting the default `idx = n`, which breaks the case where `target` exceeds every element.
- Using `e = mid - 1` correctly but forgetting to save `idx = mid` first — losing the current best candidate insertion point.
- Treating this as needing a separate "not found" pass after the loop instead of tracking `idx` during the search.

## Interview Questions

1. Why is the answer for an absent `target` the same as its lower bound?
2. What does `idx = n` represent, and why is it needed as the initial value?
3. Could this be solved with `upper_bound` instead of `lower_bound` semantics? Why not, given distinct values?
4. How would the algorithm change if `nums` allowed duplicates?

## Quick Revision

| Approach      | Time       | Space  | Core Idea                                                        |
| ------------- | ---------- | ------ | ----------------------------------------------------------------- |
| Binary Search | `O(log n)` | `O(1)` | Lower bound of `target`, with an early return on exact match      |

### 30-Second Recall

```text
Problem:
Return index of target in sorted distinct array, else its insertion index.

Idea:
Lower bound search (nums[i] >= target); return mid immediately on equality.

TC:
O(log n).

SC:
O(1).

Key trap:
Default idx = n for "insert at the end"; save idx = mid before shrinking left on nums[mid] > target.
```
