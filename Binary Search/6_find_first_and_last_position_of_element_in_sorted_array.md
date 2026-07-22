# Find First and Last Position of Element in Sorted Array

**Problem Link:** https://leetcode.com/problems/find-first-and-last-position-of-element-in-sorted-array/
**Platform:** LeetCode
**Difficulty:** Medium
**Topic:** Binary Search
**Pattern:** Two independent boundary searches (leftmost & rightmost occurrence)

---

## Problem

Given an array `nums` sorted in non-decreasing order and a `target`, return `[first, last]` — the indices of the first and last occurrence of `target`. If `target` is not present, return `[-1, -1]`.

Must run in `O(log n)` time.

### Example 1

**Input:**

```text
nums = [5,7,7,8,8,10], target = 8
```

**Output:**

```text
[3, 4]
```

### Example 2

**Input:**

```text
nums = [5,7,7,8,8,10], target = 6
```

**Output:**

```text
[-1, -1]
```

### Example 3

**Input:**

```text
nums = [], target = 0
```

**Output:**

```text
[-1, -1]
```

### Constraints

```text
0 <= nums.length <= 10^5
-10^9 <= nums[i] <= 10^9
nums is a non-decreasing array.
-10^9 <= target <= 10^9
```

---

## Core Observation

Because `nums` is sorted, every occurrence of `target` sits in one **contiguous block**. Finding that block only requires finding its **left edge** and **right edge** independently — two boundary searches instead of one full scan.

```text
nums:      5  7  7  8  8  10
index:     0  1  2  3  4  5

match? :   F  F  F  T  T  F
                     ^  ^
                first  last
```

---

# Approach 1 — Brute Force (Linear Scan)

## Intuition

Directly scan every element once. The first time `target` is seen, record it as `firstIdx`; every subsequent match updates `lastIdx`. If the scan finishes without any match, both stay `-1`.

## Algorithm

1. `firstIdx = -1`, `lastIdx = -1`.
2. For `i` from `0` to `n-1`: if `nums[i] == target`, set `firstIdx = i` (only if still `-1`) and `lastIdx = i`.
3. Return `{firstIdx, lastIdx}`.

## Code

```cpp
vector<int> searchRange(vector<int> &nums, int target)
{
  int n = nums.size();

  int firstIdx = -1, lastIdx = -1;

  for (int i = 0; i < n; i++)
  {
    if (nums[i] == target)
    {
      if (firstIdx == -1)
        firstIdx = i;
      lastIdx = i;
    }
  }

  return {firstIdx, lastIdx};
}
```

## Complexity

### Time — `O(n)`

Single left-to-right scan regardless of where `target` occurs.

### Space — `O(1)`

Only `firstIdx`, `lastIdx`, `n`, and the loop index.

---

# Approach 2 — Optimal (Two Binary Searches)

## Intuition

The brute force already relies on the sorted structure (matches form one block) but still pays `O(n)` to find its edges. Since the block's boundaries are each a **monotonic predicate** over the index, they can be found with two independent binary searches instead:

- `findFirst`: on a match, the leftmost occurrence might still be further left, so record `idx = mid` and **keep searching left** (`e = mid - 1`).
- `findLast`: on a match, the rightmost occurrence might still be further right, so record `idx = mid` and **keep searching right** (`s = mid + 1`).

Both searches otherwise behave like a normal binary search (`nums[mid] < target` → go right, `nums[mid] > target` → go left).

## Algorithm

**findFirst:**

1. `s = 0`, `e = n-1`, `idx = -1`.
2. While `s <= e`: `mid = s + (e-s)/2`.
   - `nums[mid] == target` → `idx = mid`, `e = mid - 1` (look for an earlier match).
   - `nums[mid] < target` → `s = mid + 1`.
   - else → `e = mid - 1`.
3. Return `idx`.

**findLast:** identical, except on a match set `s = mid + 1` (look for a later match) instead of `e = mid - 1`.

**searchRange:** return `{findFirst(...), findLast(...)}`.

## Visualization — two independent boundary searches

```text
nums = [5, 7, 7, 8, 8, 10], target = 8

findFirst:                         findLast:
s=0 e=5 mid=2 nums=7 <8 -> s=3      s=0 e=5 mid=2 nums=7 <8 -> s=3
s=3 e=5 mid=4 nums=8==8 -> idx=4,e=3  s=3 e=5 mid=4 nums=8==8 -> idx=4,s=5
s=3 e=3 mid=3 nums=8==8 -> idx=3,e=2  s=5 e=5 mid=5 nums=10>8 -> e=4
s=3 e=2 -> loop ends -> idx=3        s=5 e=4 -> loop ends -> idx=4

first = 3, last = 4
```

## Code

```cpp
int findFirst(vector<int> &nums, int n, int target)
{
  int s = 0, e = n - 1;
  int idx = -1;

  while (s <= e)
  {
    int mid = s + (e - s) / 2;

    if (nums[mid] == target)
    {
      idx = mid;
      e = mid - 1;
    }
    else if (nums[mid] < target)
    {
      s = mid + 1;
    }
    else
    {
      e = mid - 1;
    }
  }

  return idx;
}

int findLast(vector<int> &nums, int n, int target)
{
  int s = 0, e = n - 1;
  int idx = -1;

  while (s <= e)
  {
    int mid = s + (e - s) / 2;

    if (nums[mid] == target)
    {
      idx = mid;
      s = mid + 1;
    }
    else if (nums[mid] < target)
    {
      s = mid + 1;
    }
    else
    {
      e = mid - 1;
    }
  }

  return idx;
}

vector<int> searchRange(vector<int> &nums, int target)
{
  int n = nums.size();

  int firstIdx = findFirst(nums, n, target);
  int lastIdx = findLast(nums, n, target);

  return {firstIdx, lastIdx};
}
```

## Complexity

### Time — `O(log n)`

Two independent binary searches, each halving the search space: `O(log n) + O(log n) = O(log n)`.

### Space — `O(1)`

Only scalars per call (`s`, `e`, `idx`, `mid`).

---

## Edge Cases

- Empty array (`n == 0`) → `e = -1` makes the loop condition `s <= e` false immediately in both searches → `{-1, -1}`.
- `target` absent → both searches exhaust without ever hitting the `==` branch → `{-1, -1}`.
- Only one occurrence → `findFirst` and `findLast` converge to the same index.
- All elements equal `target` → `first = 0`, `last = n-1`.
- `target` at the very first or very last index.

## Key Takeaways

- When duplicates form a **contiguous block** in a sorted array, split "find all occurrences" into two independent **boundary searches** rather than one scan or one search that must be re-run.
- `findFirst` mirrors [lower bound](2_lower_bound.md) with an equality short-circuit: on a match, keep shrinking **left**.
- `findLast` mirrors [upper bound](3_upper_bound.md) minus one: on a match, keep shrinking **right**.
- Equivalent one-liner using existing bound utilities: `first = lowerBound(target)` if `nums[first] == target` else `-1`; `last = upperBound(target) - 1` if that index's value equals `target` else `-1`. Same result, expressed through reusable primitives.

## Common Mistakes

- Returning immediately on the first match instead of continuing to shrink toward the true boundary — that gives *an* occurrence, not necessarily the first/last.
- Mixing up the shrink direction (`e = mid - 1` vs `s = mid + 1`) between `findFirst` and `findLast`.
- Forgetting the `-1` default, breaking the not-found case.
- Re-scanning the whole array for the second boundary instead of running two independent, cheap binary searches.

## Interview Questions

1. Why can this be solved with two separate binary searches instead of a single pass?
2. What is the one-line difference between `findFirst` and `findLast`, and why does it flip which boundary is found?
3. How would you express `findFirst`/`findLast` using `lowerBound`/`upperBound`?
4. Why does running two `O(log n)` searches not change the overall complexity class?
5. What happens on an empty array or when `target` is absent?

## Quick Revision

| Approach | Time | Space | Core Idea |
| -------- | ---- | ----- | --------- |
| Brute (Linear Scan) | `O(n)` | `O(1)` | Track first/last index seen while scanning once |
| Optimal (Two Binary Searches) | `O(log n)` | `O(1)` | `findFirst`: match → shrink left; `findLast`: match → shrink right |

### 30-Second Recall

```text
Problem:
First and last index of target in a sorted array, else [-1,-1]. O(log n).

Brute:
Scan once; first match -> firstIdx (if unset); every match -> lastIdx.

Optimal:
Two binary searches. On nums[mid]==target:
  findFirst -> record idx, e = mid-1 (go left for an earlier one).
  findLast  -> record idx, s = mid+1 (go right for a later one).

TC:
O(log n) (two independent O(log n) searches).

SC:
O(1).

Key trap:
Don't return immediately on a match — keep shrinking toward the true boundary.
```
