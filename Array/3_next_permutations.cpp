// BRUTE FORCE - Generate All Unique Permutations
//
// TC - O(n * n!)
//      - Backtracking generates all unique permutations.
//      - There are at most n! permutations.
//      - Copying each permutation into the result takes O(n) time.
//      - Sorting all permutations takes O(n! * log(n!)),
//        which is dominated by O(n * n!).
//      - Searching for the current permutation takes
//        O(n! * n) in the worst case.
//      - Therefore overall time complexity:
//          O(n * n!)
//
// SC - O(n) Auxiliary Space
//      - Recursion stack requires O(n) space.
//      - The unordered_set at each recursion level stores
//        at most O(n) elements.
//      - Therefore auxiliary space = O(n).
//
//      O(n * n!) Result Space
//      - There are at most n! unique permutations.
//      - Each permutation stores n elements.
//      - Therefore result space = O(n * n!).
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

// OPTIMIZED - Pivot + Swap + Reverse Suffix
//
// TC - O(n)
//      - Traverse from right to find the pivot in O(n).
//      - Traverse from right to find the element just
//        greater than the pivot in O(n).
//      - Reversing the suffix takes O(n).
//      - Therefore:
//          O(n) + O(n) + O(n)
//          = O(n)
//
// SC - O(1)
//      - No extra data structures are used.
//      - Reversal is performed in-place.
//      - Therefore auxiliary space = O(1).
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

//
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