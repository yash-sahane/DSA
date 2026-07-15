// BRUTE FORCE - Mark Rows and Columns using Temporary Value (-1)
//
// TC - O((n * m) * (n + m))
//      - Traverse every cell of the matrix once.
//      - Whenever a 0 is found:
//          - Entire row is traversed in O(m).
//          - Entire column is traversed in O(n).
//      - In the worst case, every cell is 0.
//      - Therefore:
//          O(n * m) * O(n + m)
//          = O(n * m * (n + m))
//
//      - Final traversal converts all -1 values to 0 in O(n * m),
//        which is dominated by O(n * m * (n + m)).
//
// SC - O(1)
//      - No extra data structures are used.
//      - Cells are temporarily marked with -1 inside the given matrix.
//      - Therefore auxiliary space = O(1).
void setMatrixRow(int row, vector<vector<int>> &matrix, int m)
{
  for (int col = 0; col < m; col++)
  {
    if (matrix[row][col] != 0)
      matrix[row][col] = -1;
  }
}

void setMatrixCol(int col, vector<vector<int>> &matrix, int n)
{
  for (int row = 0; row < n; row++)
  {
    if (matrix[row][col] != 0)
      matrix[row][col] = -1;
  }
}

void setZeroes(vector<vector<int>> &matrix)
{
  int n = matrix.size();
  int m = matrix[0].size();

  vector<int> rows(n, 1);
  vector<int> cols(m, 1);

  for (int i = 0; i < n; i++)
  {
    for (int j = 0; j < m; j++)
    {
      if (matrix[i][j] == 0)
      {
        setMatrixRow(i, matrix, m);
        setMatrixCol(j, matrix, n);
      }
    }
  }

  for (int i = 0; i < n; i++)
  {
    for (int j = 0; j < m; j++)
    {
      if (matrix[i][j] == -1)
      {
        matrix[i][j] = 0;
      }
    }
  }
}

// BETTER - Row and Column Hashing
//
// TC - O(n * m)
//      - First traversal identifies all rows and columns containing 0
//        and marks them in the rows[] and cols[] arrays.
//      - Second traversal updates every cell based on these markers.
//      - Therefore:
//          O(n * m) + O(n * m)
//          = O(n * m)
//
// SC - O(n + m)
//      - rows[] array requires O(n) space.
//      - cols[] array requires O(m) space.
//      - Therefore auxiliary space = O(n + m).
void setZeroes(vector<vector<int>> &matrix)
{
  int n = matrix.size();
  int m = matrix[0].size();

  vector<int> rows(n, 1);
  vector<int> cols(m, 1);

  for (int i = 0; i < n; i++)
  {
    for (int j = 0; j < m; j++)
    {
      if (matrix[i][j] == 0)
      {
        rows[i] = 0;
        cols[j] = 0;
      }
    }
  }

  for (int i = 0; i < n; i++)
  {
    for (int j = 0; j < m; j++)
    {
      if (rows[i] == 0 || cols[j] == 0)
      {
        matrix[i][j] = 0;
      }
    }
  }
}

// OPTIMIZED - First Row and First Column as Markers
//
// TC - O(n * m)
//      - First traversal marks rows and columns using the first row
//        and first column of the matrix.
//      - Second traversal updates every cell using these markers.
//      - All marking and checking operations take O(1) time.
//      - Therefore:
//          O(n * m) + O(n * m)
//          = O(n * m)
//
// SC - O(1)
//      - No extra arrays are used.
//      - Only one additional variable (col0) is used to track whether
//        the first column should be set to 0.
//      - Therefore auxiliary space = O(1).
void setZeroes(vector<vector<int>> &matrix)
{
  int n = matrix.size();
  int m = matrix[0].size();

  int col0 = 1;

  for (int i = 0; i < n; i++)
  {
    if (matrix[i][0] == 0)
      col0 = 0;
    for (int j = 1; j < m; j++)
    {
      if (matrix[i][j] == 0)
      {
        matrix[i][0] = 0;
        matrix[0][j] = 0;
      }
    }
  }

  for (int i = n - 1; i >= 0; i--)
  {
    for (int j = m - 1; j >= 1; j--)
    {
      if (matrix[i][0] == 0 || matrix[0][j] == 0)
      {
        matrix[i][j] = 0;
      }
    }
    if (col0 == 0)
      matrix[i][0] = 0;
  }
}