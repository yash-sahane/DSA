# Next Permutation

**Problem Link:** https://leetcode.com/problems/next-permutation/
**Platform:** LeetCode
**Difficulty:** Medium
**Topic:** Array
**Pattern:** Pivot → successor → reverse suffix (lexicographic ordering)

---

## Problem

Rearrange the numbers into the **lexicographically next greater permutation**. If no greater permutation exists (the array is in descending order), rearrange it into the **lowest** order (ascending). The transformation must be done **in place**.

### Example 1

**Input:**

```text
nums = [1,2,3]
```

**Output:**

```text
[1,3,2]
```

### Example 2

**Input:**

```text
nums = [3,2,1]
```

**Output:**

```text
[1,2,3]
```

**Explanation:**

`[3,2,1]` is the highest permutation, so it wraps around to the lowest.

### Example 3

**Input:**

```text
nums = [1,1,5]
```

**Output:**

```text
[1,5,1]
```

### Constraints

```text
1 <= n <= 100
0 <= nums[i] <= 100
```

---

# Approach 1 — Brute Force (Generate all permutations, then pick the next)

## Intuition

The definition of "next permutation" is literally about ordering all permutations. So the most direct approach is to actually **generate every unique permutation**, sort them lexicographically, find where the current arrangement sits, and return the one after it (wrapping to the first if it was the last).

Correct, but hopelessly expensive: there are up to `n!` permutations. This only clarifies _what_ the answer means — it is not usable for `n` beyond ~10.

## Algorithm

1. `solve` generates all unique permutations via backtracking, using an `unordered_set` per recursion level to skip duplicate values at the same position.
2. Sort all permutations lexicographically.
3. Linear-scan for the current arrangement; return the next one, wrapping to index `0` if it was last.

## Visualization

```text
nums = [1,3,2]

All unique perms, sorted:
  [1,2,3]
  [1,3,2]   <- current
  [2,1,3]
  [2,3,1]
  [3,1,2]
  [3,2,1]

Answer = the entry right after current = [2,1,3]
```

## Code

```cpp
// BRUTE FORCE - Generate All Unique Permutations
void solve(int idx, vector<int> nums, vector<vector<int>> &perms, int n)
{
  if (idx == n)
  {
    perms.push_back(nums);
    return;
  }

  unordered_set<int> used;

  for (int i = idx; i < n; i++)
  {

    if (used.count(nums[i]))
      continue;

    used.insert(nums[i]);

    swap(nums[idx], nums[i]);
    solve(idx + 1, nums, perms, n);
    swap(nums[idx], nums[i]);
  }
}

// Uses solve() above to generate all permutations, then returns the next one.
void nextPermutation(vector<int> &nums)
{
  int n = nums.size();
  vector<vector<int>> perms;
  solve(0, nums, perms, n);

  sort(perms.begin(), perms.end());

  for (int i = 0; i < perms.size(); i++)
  {
    if (perms[i] == nums)
    {
      if (i == perms.size() - 1)
      {
        nums = perms[0];
      }
      else
      {
        nums = perms[i + 1];
      }

      return;
    }
  }
}
```

## Complexity

### Time — `O(n * n!)`

Backtracking produces up to `n!` permutations, each copied in `O(n)`. Sorting is `O(n! log n!)` (dominated), and the linear search is `O(n! * n)`. Overall `O(n * n!)`.

### Space — `O(n)` auxiliary, `O(n * n!)` result

Recursion depth `O(n)` plus the per-level `unordered_set` (`O(n)`). Storing every permutation costs `O(n * n!)`.

---

# Approach 2 — Optimal (Pivot + Swap + Reverse Suffix)

## Intuition

We want the _smallest_ increase over the current arrangement. Scanning from the right, a suffix that is entirely **descending** is already the largest arrangement of those elements — it cannot be increased without touching an earlier index. So find the first index (from the right) that **breaks** the descending run: `nums[idx] < nums[idx+1]`. That `idx` is the **pivot** — the leftmost position we can afford to bump up.

To make the increase minimal, swap the pivot with the **smallest element to its right that is still larger than the pivot**. The suffix after the pivot is now descending; reverse it to make it ascending (the smallest possible tail). If no pivot exists, the whole array was descending → reverse everything to get the lowest permutation.

## Algorithm

1. From `i = n-2` down to `0`, find the first `idx` with `nums[idx] < nums[idx+1]`.
2. If `idx == -1`, reverse the entire array and return.
3. Otherwise, from the right, find the first `nums[i] > nums[idx]` and swap them.
4. Reverse the suffix `nums[idx+1 .. n-1]`.

## Visualization

```text
nums = [1, 3, 5, 4, 2]

Step 1 - find pivot (first ascending pair from the right):
        3 < 5 ? yes  -> pivot idx = 1 (value 3)
        [1, 3, 5, 4, 2]
             ^pivot   (suffix 5,4,2 is descending)

Step 2 - from the right, first value > pivot(3) is 4 -> swap:
        [1, 4, 5, 3, 2]
             ^

Step 3 - reverse suffix after pivot (5,3,2 -> 2,3,5):
        [1, 4, 2, 3, 5]   <- next permutation
```

### Dry Run — descending (wrap-around) case

```text
nums = [3, 2, 1]
Step 1: no i with nums[i] < nums[i+1]  -> idx = -1
Step 2: reverse whole array -> [1, 2, 3]  (lowest permutation)
```

## Code

```cpp
// OPTIMIZED - Pivot + Swap + Reverse Suffix
void nextPermutation(vector<int> &nums)
{
  int idx = -1;
  int n = nums.size();

  for (int i = n - 2; i >= 0; i--)
  {
    if (nums[i] < nums[i + 1])
    {
      idx = i;
      break;
    }
  }

  if (idx == -1)
  {
    reverse(nums.begin(), nums.end());
  }
  else
  {
    for (int i = n - 1; i > idx; i--)
    {
      if (nums[i] > nums[idx])
      {
        swap(nums[i], nums[idx]);
        break;
      }
    }

    reverse(nums.begin() + idx + 1, nums.end());
  }
}
```

## Complexity

### Time — `O(n)`

Three linear scans: find pivot, find successor, reverse suffix. `O(n) + O(n) + O(n) = O(n)`.

### Space — `O(1)`

All operations are in place; no auxiliary structures.

---

## Edge Cases

- Fully descending array `[3,2,1]` → wraps to ascending `[1,2,3]` (`idx == -1`).
- Single element `[1]` → unchanged.
- Duplicates `[1,1,5]` → pivot/successor logic still works with `<` and `>` (strict) comparisons.
- Already the lowest permutation `[1,2,3]` → becomes `[1,3,2]`.

## Key Takeaways

- A **descending suffix** is the maximal arrangement of those elements — that observation is what makes the pivot the right place to change.
- To get the _next_ (minimal increase), swap the pivot with its **just-greater** successor from the right, then **reverse** (not sort — the suffix is already descending) to get the smallest tail.
- Reversing a known-descending run is `O(n)`; recognizing you don't need a full sort is the key efficiency win.
- `std::next_permutation` implements exactly this algorithm.

## Common Mistakes

- Sorting the suffix instead of reversing it (works but is `O(n log n)` and unnecessary).
- Using `<=`/`>=` where strict `<`/`>` is required, mishandling duplicates.
- Forgetting the wrap-around case when no pivot exists.
- Searching for the successor from the left instead of the right (you need the _smallest_ value greater than the pivot, which the descending suffix gives from the right).

## Interview Questions

1. Why is the suffix after the pivot always descending, and why does that mean you reverse instead of sort?
2. How do you find the element to swap with the pivot, and why from the right?
3. How does the algorithm handle duplicate values?
4. What does `idx == -1` signify and how is it handled?
5. Can you produce the _previous_ permutation with a symmetric algorithm?

## Quick Revision

| Approach | Time        | Space      | Core Idea                                                    |
| -------- | ----------- | ---------- | ------------------------------------------------------------ |
| Brute    | `O(n * n!)` | `O(n)` aux | Generate all perms, sort, return the one after current       |
| Optimal  | `O(n)`      | `O(1)`     | Find pivot, swap with just-greater successor, reverse suffix |

### 30-Second Recall

```text
Problem:
Rearrange to the next lexicographically greater permutation, in place (wrap if largest).

Brute:
Generate all unique perms, sort, pick the next (wrap to first).

Optimal:
1) From right, find pivot: first nums[i] < nums[i+1].
2) If none -> reverse all (lowest perm).
3) Else swap pivot with rightmost value > pivot, then reverse the suffix.

TC:
Brute O(n*n!), Optimal O(n).

SC:
Brute O(n) aux, Optimal O(1).

Key trap:
Reverse the suffix (already descending) instead of sorting; handle the all-descending wrap-around.
```
