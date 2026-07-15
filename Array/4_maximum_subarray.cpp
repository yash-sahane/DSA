// BRUTE FORCE - Generate All Subarrays
//
// TC - O(n^2)
//      - Outer loop selects the starting index of the subarray.
//      - Inner loop extends the subarray from the starting index
//        to every possible ending index.
//      - Running sum is maintained incrementally, so each extension
//        takes O(1) time.
//      - Therefore:
//          O(n) * O(n)
//          = O(n^2)
//
// SC - O(1)
//      - Only a few variables (sum and maxSum) are used.
//      - No extra data structures are required.
//      - Therefore auxiliary space = O(1).
int maxSubArray(vector<int> &nums)
{
  int n = nums.size();

  int maxSum = nums[0];
  for (int i = 1; i < n; i++)
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

// OPTIMIZED - Kadane's Algorithm
//
// TC - O(n)
//      - Traverse the array once from left to right.
//      - At each index:
//          - Decide whether to start a new subarray or extend
//            the previous one in O(1) time.
//          - Update the maximum subarray sum in O(1) time.
//      - Therefore:
//          O(n)
//
// SC - O(1)
//      - Only two variables (currSum and maxSum) are maintained.
//      - No extra data structures are used.
//      - Therefore auxiliary space = O(1).
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