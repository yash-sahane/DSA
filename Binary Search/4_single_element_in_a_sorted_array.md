# Single Element in a Sorted Array

**Problem Link:** https://leetcode.com/problems/single-element-in-a-sorted-array/
**Platform:** LeetCode
**Difficulty:** Medium
**Topic:** Binary Search
**Pattern:** Binary search on a monotonic predicate (parity of pair-alignment)

---

## Problem

You are given a sorted array consisting of integers where **every element appears exactly twice**, except for **one element that appears exactly once**. Find and return that single element. The intended solution runs in `O(log n)` time and `O(1)` space.

### Example 1

**Input:**

```text
nums = [1,1,2,3,3,4,4,8,8]
```

**Output:**

```text
2
```

### Example 2

**Input:**

```text
nums = [3,3,7,7,10,11,11]
```

**Output:**

```text
10
```

### Constraints

```text
1 <= nums.length <= 10^5
0 <= nums[i] <= 10^5
```

---

## Core Observation

Because the array is sorted and every value except one appears exactly twice, **before** the singleton every duplicate pair starts at an **even** index — `(0,1), (2,3), (4,5), ...`. The singleton has no partner, so it shifts everything after it by one slot: pairs **after** the singleton start at **odd** indices instead. The singleton itself sits exactly at the index where this even/odd alignment breaks.

```text
nums:       1  1  2  3  3  4  4  8  8
index:      0  1  2  3  4  5  6  7  8

Even-start pair check (idx, idx+1):
  idx 0: (1,1) intact -> T
  idx 2: (2,3) broken -> F   <-- singleton found here
  idx 4: (3,4) broken -> F
  idx 6: (4,8) broken -> F

Pattern:  T  F  F  F ...
              ^
        first broken pair = singleton's index
```

This `T...TFF...F` shape is exactly the monotonic structure that makes the problem binary-searchable (Approach 3).

---

# Approach 1 — Brute Force (Pairing Check)

## Intuition

Directly test the even/odd-alignment observation above with a linear scan: walk the array, and at every even index `i`, check whether `nums[i] == nums[i+1]`. As long as the pair holds, keep going. The moment an even-indexed pair doesn't hold, `nums[i]` must be the singleton (it broke the alignment). If the scan finishes without ever breaking, the singleton has no partner at all — it must be the last element.

This meets `O(n)` time / `O(1)` space, but **not** the `O(log n)` time the problem asks for — it only establishes correctness of the core observation before applying binary search.

## Algorithm

1. For `i` from `0` to `n-2`: if `i` is even and `nums[i] != nums[i+1]`, return `nums[i]`.
2. If the loop completes without returning, return `nums[n-1]`.

## Code

```cpp
// brute force
class Solution
{
public:
  int singleNonDuplicate(vector<int> &nums)
  {
    int n = nums.size();

    for (int i = 0; i < n - 1; i++)
    {
      if (i % 2 == 0 && nums[i] != nums[i + 1])
        return nums[i];
    }

    return nums[n - 1];
  }
};
```

## Complexity

### Time — `O(n)`

Single left-to-right scan; each index checked once.

### Space — `O(1)`

Only the loop index and `n`.

---

# Approach 2 — Better (XOR)

## Intuition

XOR is its own inverse: `a ^ a = 0` and `a ^ 0 = a`. Since every value except the singleton appears **exactly twice**, XOR-ing the entire array cancels every duplicate pair to `0`, leaving only the singleton. This is elegant and — unlike Approaches 1 and 3 — **does not require the array to be sorted at all**.

It is still `O(n)` time, so it doesn't meet the problem's `O(log n)` target either, but it's a useful, more robust `O(n)` fallback (works even if the array weren't sorted).

## Algorithm

1. `res = 0`.
2. XOR every element into `res`.
3. Return `res`.

## Code

```cpp
// xor
int singleNonDuplicate(vector<int> &nums)
{
  int n = nums.size();

  int res = 0;
  for (int i = 0; i < n; i++)
  {
    res = res ^ nums[i];
  }

  return res;
}
```

## Complexity

### Time — `O(n)`

One pass, XOR-ing every element.

### Space — `O(1)`

Only the accumulator `res`.

---

# Approach 3 — Optimal (Binary Search on the Alignment Break)

## Intuition

Approach 1 already identifies *what* to look for — the point where even-indexed pairing breaks — but scans for it linearly. Since "pairing intact" is `T` for every even index before the singleton and `F` for every even index from the singleton onward (see Core Observation above), this is a classic monotonic-predicate binary search: shrink the range toward the first `F`.

To test a candidate `mid`, first **normalize it to an even index** (if `mid` is odd, decrement it) so `(mid, mid+1)` is always checked as a would-be pair. If `nums[mid] == nums[mid+1]`, the pairing is still intact at `mid`, so the singleton is strictly to the right (`s = mid + 2`, skipping the whole intact pair). Otherwise the alignment has already broken at or before `mid`, so the singleton is at or to the left (`e = mid`).

Boundary elements are handled separately up front (`n == 1`, singleton at index `0`, singleton at the last index) so the loop only searches the well-defined interior range `[1, n-2]`.

## Algorithm

1. If `n == 1`, return `nums[0]`.
2. If `nums[0] != nums[1]`, the singleton is at index `0` — return it.
3. If `nums[n-1] != nums[n-2]`, the singleton is at the last index — return it.
4. `s = 1`, `e = n - 2`. While `s < e`:
   - `mid = s + (e - s) / 2`; if `mid` is odd, `mid--` (normalize to even).
   - If `nums[mid] == nums[mid+1]`: pairing intact → `s = mid + 2`.
   - Else: pairing broken → `e = mid`.
5. Return `nums[s]` (loop ends when `s == e`, the singleton's index).

## Visualization — monotonic answer space

```text
Even-indexed "pair intact" predicate:  T T ... T  F F ... F
                                                   ^
                                          first F = singleton index
```

### Dry Run

```text
nums = [1, 1, 2, 3, 3, 4, 4, 8, 8]   (n = 9)

nums[0]=1 == nums[1]=1        -> not the edge case
nums[8]=8 == nums[7]=8        -> not the edge case
s=1, e=7

s=1 e=7 mid=4 (even)          nums[4]=3, nums[5]=4  -> broken -> e=4
s=1 e=4 mid=2 (even)          nums[2]=2, nums[3]=3  -> broken -> e=2
s=1 e=2 mid=1 -> odd -> mid=0 nums[0]=1, nums[1]=1  -> intact -> s=mid+2=2
s=2 e=2 -> loop ends -> return nums[2] = 2   ✓
```

## Code

```cpp
// binary search
int singleNonDuplicate(vector<int> &nums)
{
  int n = nums.size();

  if (n == 1)
    return nums[0];
  if (nums[0] != nums[1])
    return nums[0];
  if (nums[n - 1] != nums[n - 2])
    return nums[n - 1];

  int s = 1, e = n - 2;
  while (s < e)
  {
    int mid = s + (e - s) / 2;

    if (mid % 2 == 1)
      mid--;

    if (nums[mid] == nums[mid + 1])
    {
      s = mid + 2;
    }
    else
    {
      e = mid;
    }
  }

  return nums[s];
}
```

## Complexity

### Time — `O(log n)`

Each iteration discards roughly half the remaining candidate range (`s = mid + 2` or `e = mid`), giving logarithmic iterations.

### Space — `O(1)`

Only `s`, `e`, `mid`, `n`.

---

## Edge Cases

- `n == 1` → the single element is the only element; handled explicitly before the loop.
- Singleton at index `0` (`nums[0] != nums[1]`) or at the last index (`nums[n-1] != nums[n-2]`) → handled explicitly so the binary-search interior range `[1, n-2]` is always well-formed.
- Singleton adjacent to the boundary (e.g. index `1` or `n-2`) → covered correctly since the loop range starts/ends exactly there.
- All duplicates except one at the exact middle.

## Key Takeaways

- **Parity-based invariants**: when duplicates are guaranteed to pair up, a break in an expected even/odd alignment is a powerful, cheap-to-check monotonic signal for binary search.
- **XOR cancellation** (`a^a=0`, `a^0=a`) is the standard trick for "every element appears twice except one" — and notably works **without sorted order**, unlike the binary search approach.
- Binary search doesn't require searching over "values" — here it searches over **index positions** using a boolean predicate ("is this even-start pair still intact?"), the same "binary search on answer" idea used elsewhere (e.g. lower/upper bound).
- Always normalize `mid` to the correct parity before evaluating a parity-dependent predicate, or the check is meaningless.
- Only the binary search approach actually satisfies the problem's stated `O(log n)` time / `O(1)` space requirement — brute force and XOR are both correct but `O(n)`.

## Common Mistakes

- Comparing `nums[mid]` and `nums[mid+1]` without first normalizing `mid` to an even index — this checks the wrong, misaligned pair and breaks the invariant.
- Using `e = mid - 1` instead of `e = mid` when the pairing is broken — since `mid` itself could be the singleton's index, decrementing past it can skip the answer.
- Skipping the `n == 1` / first-pair / last-pair edge cases and indexing out of bounds (`nums[n]` or `nums[-1]`) inside the main loop.
- Assuming XOR needs a sorted array (it doesn't) or that binary search works on an unsorted array (it does not — it depends on sortedness for the parity invariant).

## Interview Questions

1. Why does XOR-ing the whole array isolate the single element?
2. Why do duplicate pairs start at even indices before the singleton and odd indices after it?
3. Why must `mid` be normalized to an even index before comparing `nums[mid]` and `nums[mid+1]`?
4. Why are the `nums[0] vs nums[1]` and `nums[n-1] vs nums[n-2]` checks needed before the binary search loop even starts?
5. Of the three approaches, which one actually satisfies the problem's `O(log n)`/`O(1)` requirement, and why do the other two fall short?

## Quick Revision

| Approach | Time | Space | Core Idea |
| -------- | ---- | ----- | --------- |
| Brute (Pairing Check) | `O(n)` | `O(1)` | Scan even indices; first broken pair (or the last element) is the singleton |
| Better (XOR) | `O(n)` | `O(1)` | XOR cancels every duplicate pair to 0, leaving the singleton; works unsorted too |
| Optimal (Binary Search) | `O(log n)` | `O(1)` | Binary search the index where even-start pairing breaks; normalize `mid` to even |

### 30-Second Recall

```text
Problem:
Sorted array, every value twice except one — find the singleton. Target O(log n)/O(1).

Brute:
Scan even indices; nums[i] != nums[i+1] -> that's it; else it's the last element.

Better (XOR):
XOR everything; duplicates cancel (a^a=0), singleton survives. Works even unsorted.

Optimal:
Binary search on "pair-intact" predicate (T...TFF...F).
Normalize mid to even; intact -> s=mid+2; broken -> e=mid.
Handle n==1, first-pair, last-pair before the loop.

TC:
Brute/XOR O(n), Binary Search O(log n).

SC:
O(1) all three.

Key trap:
Normalize mid's parity before comparing nums[mid] & nums[mid+1]; use e=mid (not mid-1) on break.
```
