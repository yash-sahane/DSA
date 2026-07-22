# N-th Root of M

**Problem Link:** https://www.geeksforgeeks.org/problems/find-nth-root-of-m5843/1
**Platform:** GeeksforGeeks

**Topic:** Binary Search
**Pattern:** Binary search on answer (monotonic function of the candidate root)

---

## Problem

Given two positive integers `n` and `m`, find the integer **n-th root** of `m` — an integer `x` such that `x^n = m`. If no such integer exists, return `-1`.

### Example 1

**Input:**

```text
n = 3, m = 27
```

**Output:**

```text
3
```

**Explanation:**

`3^3 = 27`.

### Example 2

**Input:**

```text
n = 4, m = 69
```

**Output:**

```text
-1
```

**Explanation:**

`2^4 = 16` and `3^4 = 81`; no integer's 4th power equals `69`.

### Constraints

The GeeksforGeeks page renders its constraints client-side, so the exact bounds couldn't be confirmed here. Reliably: `n` and `m` are positive integers (`n, m >= 1`).

---

# Approach 1 — Brute Force (Incremental Search)

## Intuition

`x^n` is strictly increasing in `x` for `x >= 1` (and `0^n = 0`). So candidates can be tried in increasing order `x = 0, 1, 2, ...`: the moment `x^n` reaches or passes `m`, the answer is decided — equal means found, greater means no integer root exists (everything larger only grows further away, so it's safe to stop). To avoid wastefully computing a huge power, the inner computation of `x^n` bails out the moment its running product exceeds `m`.

## Algorithm

1. For `i` from `0` to `m`:
   - Build `res = i^n` one multiplication at a time; break out of this inner computation early once `res` exceeds `m`.
   - If `res == m`, return `i`.
   - If `res > m`, no larger `i` can work either (monotonic) — stop the outer loop.
2. Return `-1` if the outer loop exhausts without a match.

## Code

```cpp
int nthRoot(int n, int m)
{
  for (int i = 0; i <= m; i++)
  {
    int res = 1;

    for (int j = 0; j < n; j++)
    {
      res *= i;

      if (res > m)
        break;
    }

    if (res == m)
      return i;

    if (res > m)
      break;
  }

  return -1;
}
```

## Complexity

### Time — `O(n · m^(1/n))`

Let `k = floor(m^(1/n))` be the true root (or the point of overshoot). The outer loop only runs roughly `k` times before detecting `i^n > m` and stopping — it does **not** run the full `m` iterations. Each of those iterations does at most `n` multiplications. This degrades to `O(m)` when `n = 1` (an identity power has no shortcut) and shrinks quickly as `n` grows.

### Space — `O(1)`

Only the loop scalars `i`, `res`, `j`.

---

# Approach 2 — Optimal (Binary Search on Answer)

## Intuition

Approach 1 already exploits monotonicity to stop early, but still walks candidates one at a time from `0`. Since `x^n` is monotonically increasing, comparing `x^n` to `m` is a monotonic predicate over the **entire** range `[0, m]` — the candidates don't need to be visited in order at all. Binary search directly for the `x` whose `n`-th power equals `m`, halving the candidate range each step instead of incrementing by one.

## Algorithm

1. `s = 0`, `e = m`.
2. While `s <= e`: `val = s + (e - s) / 2`; compute `res = val^n` one multiplication at a time, breaking out early the moment `res` exceeds `m` (guards against overflow — see note below).
   - `res == m` → return `val`.
   - `res > m` → search left (`e = val - 1`).
   - else → search right (`s = val + 1`).
3. Return `-1` if the loop ends without a match.

## Visualization — monotonic answer space

```text
n = 3, m = 27

val:       0   1   2   3    4
val^3:     0   1   8   27   64
compare:   <   <   <   ==   >
                       ^
               exact match at val = 3
```

### Dry Run — no integer root exists

```text
n = 4, m = 69

s=0 e=69 val=34  34^4=1,336,336 > 69 -> e=33
s=0 e=33 val=16  16^4=65,536    > 69 -> e=15
s=0 e=15 val=7   7^4=2,401      > 69 -> e=6
s=0 e=6  val=3   3^4=81         > 69 -> e=2
s=0 e=2  val=1   1^4=1          < 69 -> s=2
s=2 e=2  val=2   2^4=16         < 69 -> s=3
s=3 e=2  -> loop ends -> return -1   ✓
```

## Code

```cpp
int nthRoot(int n, int m)
{
  int s = 0, e = m;
  while (s <= e)
  {
    int val = s + (e - s) / 2;

    long long res = 1;
    for (int j = 0; j < n; j++)
    {
      res *= val;

      if(res > m) break;
    }

    if (res == m)
    {
      return val;
    }
    else if (res > m)
    {
      e = val - 1;
    }
    else
    {
      s = val + 1;
    }
  }

  return -1;
}
```

> 💡 Why the early-exit guard matters

Without the `if (res > m) break;` inside the power loop, this computation would overflow **even `long long`**. Early in the search `e` starts at `m`, so the very first `val` tested is around `m/2` — and for `n >= 3` that alone is enough to blow past `long long`'s range.

Concretely, for a large `m` (say on the order of `10^8`–`10^9`, typical for this kind of problem) with `n >= 3`: take `val ≈ 5×10^8`. `val^2 = 2.5×10^17` fits comfortably in a `long long`, but the _next_ multiplication, `val^3 = 1.25×10^26`, blows past `long long`'s ~`9.2×10^18` ceiling. Approach 1 never risks this, because its candidates climb one at a time starting from `0`, so `i` is always close to the true root by the time `n` multiplications have been applied — it never tests a base as disproportionately large as binary search's very first midpoint can be.

A `long long` overflow is undefined behavior in C++: without the guard, `res == m` / `res > m` would become unreliable, silently steering the search in the wrong direction instead of visibly crashing. Breaking the instant `res` exceeds `m` is what keeps every intermediate value bounded — the guard is load-bearing, not just a speed tweak.

## Complexity

### Time — `O(n log m)`

`O(log m)` iterations (search space `[0, m]` halves each time), each doing up to `n` multiplications to build `val^n`. The early-exit guard only trims wasted multiplications once an iteration's outcome is already decided — it doesn't change this worst-case bound.

### Space — `O(1)`

Only `s`, `e`, `val`, `res`.

---

## Edge Cases

- `m == 1` → answer is `1` for any `n` (`1^n = 1`).
- `m == 0` (if permitted) → answer is `0` (`0^n = 0`) for `n >= 1`.
- No integer root exists (Example 2) → both approaches correctly fall through to `-1`.
- `n == 1` → answer is `m` itself; Approach 1 degrades to `O(m)` here since an identity power has no shortcut.
- Large `m` combined with `n >= 3` → exactly why the power loop's early-exit guard is required in Approach 2; a wider type alone (`long long`) would not be enough (see note above).

## Key Takeaways

- Whenever a candidate function is **strictly monotonic** in the variable you're searching over (here `x^n` in `x`), you can binary search directly on that variable instead of testing candidates in order — the general "binary search on answer" pattern.
- An early-exit check during a power/product computation isn't just a speed optimization — it's also what keeps intermediate values bounded and overflow-safe. Dropping it would reintroduce a correctness risk even with a wider integer type — the guard is not optional here.
- A wider type (`long long`) only shifts the overflow threshold; it doesn't remove the risk once the exponent is large enough relative to the base being raised.

## Common Mistakes

- Computing `val^n` in full without an early-exit check — for large `val` with `n >= 3` this risks silent overflow/UB even in a `long long`, exactly what the `break` above guards against.
- Assuming `long long` alone is "safe enough" without checking the actual magnitude the computation can reach.
- Off-by-one errors in `e = val - 1` / `s = val + 1` that skip over an exact match.
- Forgetting the `-1` return when no integer root exists.

## Interview Questions

1. Why is `x^n` a valid function to binary search over?
2. Why would Approach 2 overflow without its early-exit break, even though Approach 1 uses a smaller `int` accumulator?
3. What exactly does `if (res > m) break;` prevent, and why is it necessary even with `long long`?
4. What is Approach 1's time complexity in terms of the true answer, not just `m`?
5. What happens when `n == 1`? Does each approach still behave correctly, and at what cost?

## Quick Revision

| Approach                          | Time             | Space  | Core Idea                                                                                            |
| --------------------------------- | ---------------- | ------ | ---------------------------------------------------------------------------------------------------- |
| Brute (Incremental Search)        | `O(n · m^(1/n))` | `O(1)` | Try `x = 0,1,2,...`; stop once `x^n >= m`                                                            |
| Optimal (Binary Search on Answer) | `O(n log m)`     | `O(1)` | Binary search `x` by comparing `x^n` to `m`; power loop breaks early once it exceeds `m` to stay overflow-safe |

### 30-Second Recall

```text
Problem:
Integer x with x^n = m, else -1.

Brute:
Try x=0,1,2,... ; stop once x^n >= m (equal -> found, exceeds -> -1).

Optimal:
Binary search x over [0,m]; x^n == m -> found, > m -> go left, < m -> go right.

TC:
Brute O(n * m^(1/n)), worst O(m) when n=1. Optimal O(n log m).

SC:
O(1) both.

Key trap:
The power computation must bail out early once it exceeds m - otherwise even
long long can overflow for a large val (~m/2) once n >= 3 (already guarded
in the code above).
```
