# Binary Search

**Problem Link:** https://leetcode.com/problems/binary-search/
**Platform:** LeetCode
**Difficulty:** Easy
**Topic:** Binary Search
**Pattern:** Classic binary search on a sorted array (halving search space)

---

## Problem

Given a **sorted** (ascending) array `nums` and a `target`, return the index of `target` if it exists, otherwise `-1`. The algorithm must run in `O(log n)`.

### Example 1

**Input:**

```text
nums = [-1,0,3,5,9,12], target = 9
```

**Output:**

```text
4
```

### Example 2

**Input:**

```text
nums = [-1,0,3,5,9,12], target = 2
```

**Output:**

```text
-1
```

**Explanation:**

`2` is not present, so `-1` is returned.

### Constraints

```text
1 <= n <= 10^4
-10^4 <= nums[i], target <= 10^4
All values are distinct, sorted in ascending order.
```

---

## Intuition

Because the array is sorted, comparing the target with the **middle** element tells you which half can possibly contain it — the other half is discarded entirely. Each comparison halves the search space, giving `O(log n)`.

## Algorithm

1. `s = 0`, `e = n-1`.
2. While `s <= e`:
   - `mid = s + (e - s) / 2`.
   - If `nums[mid] == target`, return `mid`.
   - If `nums[mid] > target`, search left → `e = mid - 1`.
   - Else search right → `s = mid + 1`.
3. Return `-1` if not found.

## Visualization

```text
nums = [-1, 0, 3, 5, 9, 12], target = 9

s=0, e=5, mid=2 -> nums[2]=3  < 9  -> go right, s=3
s=3, e=5, mid=4 -> nums[4]=9 == 9  -> return 4

Search window shrinks:
[-1  0  3  5  9  12]
          ^mid          -> right half
         [ 5  9  12]
             ^mid       -> found
```

### Dry Run — target absent

```text
nums = [-1, 0, 3, 5, 9, 12], target = 2
s=0 e=5 mid=2 nums=3 >2 -> e=1
s=0 e=1 mid=0 nums=-1<2 -> s=1
s=1 e=1 mid=1 nums=0 <2 -> s=2
s=2 e=1 -> s>e, loop ends -> return -1
```

## Code

```cpp
// BINARY SEARCH - Search Target in Sorted Array
int search(vector<int> &nums, int target)
{
  int n = nums.size();

  int s = 0, e = n - 1;
  while (s <= e)
  {
    int mid = s + (e - s) / 2;

    if (nums[mid] == target)
    {
      return mid;
    }
    else if (nums[mid] > target)
    {
      e = mid - 1;
    }
    else
    {
      s = mid + 1;
    }
  }

  return -1;
}
```

## Complexity

### Time — `O(log n)`

The search space halves each iteration: `n → n/2 → n/4 → ... → 1`, i.e. `O(log n)` iterations.

### Space — `O(1)`

Only the scalars `s`, `e`, `mid`, `n`.

---

## Edge Cases

- Single-element array — one iteration.
- Target smaller than all / larger than all elements → `-1`.
- Target at the first or last index.
- Empty array (`n == 0`) → loop never runs, returns `-1`.

## Key Takeaways

- `mid = s + (e - s) / 2` avoids the integer overflow that `(s + e) / 2` can cause when `s + e` exceeds `INT_MAX`.
- The invariant "the target, if present, lies within `[s, e]`" is what justifies discarding a half each step.
- `while (s <= e)` with `e = mid - 1` / `s = mid + 1` is the inclusive-bounds template; the loop ends when `s > e`.
- Any time the search space provably **halves** per step, the cost is `O(log n)` — a signal to reach for binary search.

## Common Mistakes

- Writing `mid = (s + e) / 2`, risking overflow for large indices.
- Using `s < e` instead of `s <= e`, missing the single-element window.
- Updating bounds to `mid` instead of `mid ± 1`, causing an infinite loop.
- Applying binary search to an unsorted array.

## Interview Questions

1. Why is `s + (e - s) / 2` preferred over `(s + e) / 2`?
2. Why is the loop condition `s <= e` and not `s < e`?
3. How do you adapt this to find the **first** or **last** occurrence when duplicates exist?
4. How would you binary-search a rotated sorted array?

## Quick Revision

| Approach | Time | Space | Core Idea |
| -------- | ---- | ----- | --------- |
| Binary Search | `O(log n)` | `O(1)` | Compare with mid, discard the impossible half |

### 30-Second Recall

```text
Problem:
Return index of target in a sorted array, else -1, in O(log n).

Idea:
mid = s + (e-s)/2; == target -> return; > target -> go left; < -> go right.

Loop:
while (s <= e), move e=mid-1 or s=mid+1.

TC:
O(log n).

SC:
O(1).

Key trap:
Use s + (e-s)/2 (overflow-safe) and s <= e; update bounds to mid +/- 1.
```
