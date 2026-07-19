# Maximum Subarray

**Problem Link:** https://leetcode.com/problems/maximum-subarray/
**Platform:** LeetCode
**Difficulty:** Medium
**Topic:** Array / Dynamic Programming
**Pattern:** Kadane's Algorithm (running subarray sum with reset)

---

## Problem

Given an integer array `nums`, find the **contiguous subarray** (containing at least one element) with the **largest sum** and return that sum.

### Example 1

**Input:**

```text
nums = [-2,1,-3,4,-1,2,1,-5,4]
```

**Output:**

```text
6
```

**Explanation:**

The subarray `[4,-1,2,1]` has the largest sum `6`.

### Example 2

**Input:**

```text
nums = [1]
```

**Output:**

```text
1
```

### Example 3

**Input:**

```text
nums = [5,4,-1,7,8]
```

**Output:**

```text
23
```

### Constraints

```text
1 <= n <= 10^5
-10^4 <= nums[i] <= 10^4
```

---

# Approach 1 — Brute Force (Try every subarray)

## Intuition

Directly examine every contiguous subarray. Fix a start index, extend the end one step at a time while maintaining a running sum (so each extension is `O(1)`), and track the maximum sum seen.

## Algorithm

1. Initialize `maxSum = nums[0]`.
2. For each start index `i`, reset `sum = 0` and for each end `j >= i`, add `nums[j]` and update `maxSum`.

## Code

```cpp
// BRUTE FORCE - Generate All Subarrays
int maxSubArray(vector<int> &nums)
{
  int n = nums.size();

  int maxSum = nums[0];
  for (int i = 0; i < n; i++)
  {
    int sum = 0;
    for (int j = i; j < n; j++)
    {
      sum += nums[j];

      if (sum > maxSum)
        maxSum = sum;
    }
  }

  return maxSum;
}
```

## Complexity

### Time — `O(n^2)`

Outer loop picks the start, inner loop extends the end; the running sum keeps each step `O(1)`. `O(n) * O(n) = O(n^2)`.

### Space — `O(1)`

Only `sum` and `maxSum` are used.

---

# Approach 2 — Optimal (Kadane's Algorithm)

## Intuition

Walk left to right keeping `currSum` = best subarray sum **ending exactly at the current index**. At each element you make a binary choice: either **extend** the previous subarray (`currSum + nums[i]`) or **start fresh** at the current element (`nums[i]`). You start fresh precisely when the running sum has gone negative and would only drag the current element down. `maxSum` records the best `currSum` ever seen.

The DP insight: `best_ending_here[i] = max(nums[i], best_ending_here[i-1] + nums[i])`, and the answer is the max over all `i`.

## Algorithm

1. `maxSum = currSum = nums[0]`.
2. For `i` from `1` to `n-1`:
   - `currSum = max(nums[i], currSum + nums[i])`.
   - `maxSum = max(maxSum, currSum)`.

## Visualization

```text
nums = [-2, 1, -3, 4, -1, 2, 1, -5, 4]

i:        -2   1   -3   4   -1   2   1   -5   4
currSum:  -2   1   -2   4    3   5   6    1   5
maxSum:   -2   1    1   4    4   5   6    6   6
                                    ^
                        best subarray [4,-1,2,1] = 6

Reset points: whenever currSum + nums[i] < nums[i], we "start fresh".
At i=1: -2+1 = -1 < 1  -> start fresh, currSum = 1.
At i=3:  -2+4 = 2  < 4  -> start fresh, currSum = 4.
```

## Code

```cpp
// OPTIMIZED - Kadane's Algorithm
int maxSubArray(vector<int> &nums)
{
  int n = nums.size();

  int maxSum = nums[0];
  int currSum = nums[0];

  for (int i = 1; i < n; i++)
  {
    currSum = max(nums[i], currSum + nums[i]);
    maxSum = max(currSum, maxSum);
  }

  return maxSum;
}
```

## Complexity

### Time — `O(n)`

Single left-to-right pass, `O(1)` work per element.

### Space — `O(1)`

Only `currSum` and `maxSum`.

---

## Edge Cases

- **All negative** values, e.g. `[-3,-1,-2]` → answer is the largest single element `-1`. Kadane handles this because it initializes with `nums[0]` and always allows "start fresh" at each element (never forcing an empty subarray).
- Single element → returns that element.
- All positive → the whole array is the answer.
- A single large positive surrounded by negatives.

## Key Takeaways

- Kadane = DP where the state is "best subarray **ending here**"; reset to the current element whenever the carried sum turns into a liability.
- Initializing `maxSum` with `nums[0]` (not `0`) is essential so the all-negative case returns the least-negative element rather than `0`.
- The reset condition `currSum + nums[i] < nums[i]` is equivalent to `currSum < 0` — a negative prefix can never help a later subarray.
- A "try all subarrays" brute force must let the start index range over **every** index (including `0`), or it silently misses answers — see the bug above.

## Common Mistakes

- Starting the brute-force start-index loop at `1` instead of `0` (the exact bug in Approach 1).
- Initializing `maxSum = 0`, which breaks all-negative inputs.
- Resetting `currSum` to `0` unconditionally instead of `max(nums[i], currSum + nums[i])`, which mishandles arrays with no positive numbers.
- Confusing "maximum subarray sum" (contiguous) with "maximum subsequence sum".

## Interview Questions

1. Why must `maxSum` be initialized to `nums[0]` rather than `0`?
2. What is the DP recurrence Kadane implements, and what is its state?
3. When exactly do you "start a new subarray", and why is that optimal?
4. How would you also return the **indices** of the maximum subarray?
5. Can Kadane be extended to a 2D matrix (maximum submatrix sum)?

## Quick Revision

| Approach         | Time     | Space  | Core Idea                                               |
| ---------------- | -------- | ------ | ------------------------------------------------------- |
| Brute            | `O(n^2)` | `O(1)` | Try every subarray with a running sum                   |
| Optimal (Kadane) | `O(n)`   | `O(1)` | Track best sum ending here; reset when it goes negative |

### 30-Second Recall

```text
Problem:
Largest sum over all contiguous subarrays (at least one element).

Brute:
For every start i, extend end j, keep a running sum, track max. (start i from 0!)

Optimal (Kadane):
currSum = max(nums[i], currSum + nums[i]); maxSum = max(maxSum, currSum).

TC:
Brute O(n^2), Kadane O(n).

SC:
O(1) both.

Key trap:
Init maxSum = nums[0] (not 0) for all-negative arrays; brute must include start index 0.
```
