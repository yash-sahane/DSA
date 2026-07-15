// RECURSIVE - Build Pascal's Triangle Row by Row
//
// TC - O(n^2)
//      - There are n rows in Pascal's Triangle.
//      - To generate the i-th row, O(i) elements are computed.
//      - Total work:
//          1 + 2 + 3 + ... + n
//          = O(n^2)
//
// SC - O(n) Auxiliary Space
//      - Recursion stack grows to O(n).
//      - curr and temp vectors store only one row at a time,
//        requiring at most O(n) space.
//      - Therefore auxiliary space = O(n).
//
//      O(n^2) Result Space
//      - Pascal's Triangle contains:
//          1 + 2 + ... + n
//          = O(n^2) elements.
//      - Therefore result space = O(n^2).
void solve(int n, vector<int> curr, vector<vector<int>> &res)
{
  if (curr.size() == n)
  {
    return;
  }

  vector<int> temp(curr.size() + 1, 1);
  for (int i = 0; i < curr.size() - 1; i++)
  {
    temp[i + 1] = curr[i] + curr[i + 1];
  }

  curr = temp;
  res.push_back(curr);

  solve(n, curr, res);
}

vector<vector<int>> generate(int numRows)
{
  if (numRows == 1)
    return {{1}};

  vector<vector<int>> res({{1}, {1, 1}});
  vector<int> curr({1, 1});

  solve(numRows, curr, res);

  return res;
}

// ITERATIVE - Build Pascal's Triangle Row by Row
//
// TC - O(n^2)
//      - Outer loop runs for n rows.
//      - Inner loop computes all elements of the current row.
//      - Total work:
//          1 + 2 + 3 + ... + n
//          = O(n^2)
//
// SC - O(n) Auxiliary Space
//      - curr and temp vectors store only one row at a time,
//        requiring at most O(n) space.
//      - No recursion is used.
//      - Therefore auxiliary space = O(n).
//
//      O(n^2) Result Space
//      - Pascal's Triangle contains:
//          1 + 2 + ... + n
//          = O(n^2) elements.
//      - Therefore result space = O(n^2).
vector<vector<int>> generate(int numRows)
{
  if (numRows == 1)
    return {{1}};

  vector<vector<int>> res({{1}, {1, 1}});
  vector<int> curr({1, 1});

  for (int i = 2; i < numRows; i++)
  {
    vector<int> temp(curr.size() + 1, 1);
    for (int j = 0; j < curr.size() - 1; j++)
    {
      temp[j + 1] = curr[j] + curr[j + 1];
    }

    curr = temp;
    res.push_back(curr);
  }

  return res;
}