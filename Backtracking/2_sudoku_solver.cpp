// Problem - https://leetcode.com/problems/sudoku-solver/

// BRUTE FORCE - Backtracking
//
// TC - O(n^2 * 9^E)
//      - Let E be the number of empty cells.
//      - Backtracking explores at most O(9^E) states,
//        since each empty cell can try up to 9 possible digits.
//      - In every recursive call, the board is scanned to find the
//        next empty cell, which takes O(n^2).
//      - isPossible() checks the current row, column, and 3x3 box.
//        This takes O(n) time (or O(1) for a fixed 9x9 Sudoku),
//        which is dominated by the O(n^2) board scan.
//      - Therefore: O(9^E) states * O(n^2) work per state
//                   = O(n^2 * 9^E)
//
//      - For LeetCode's fixed 9x9 Sudoku,
//        n is constant, so the complexity simplifies to O(9^E).
//
// SC - O(E) Auxiliary Space
//      - The recursion stack can grow up to E levels,
//        where E is the number of empty cells.
//      - No additional data structures are used.
//      - The board is modified in-place during backtracking.
//      - Therefore auxiliary space = O(E).
//
//      - For a fixed 9x9 Sudoku, E <= 81,
//        so the auxiliary space can also be considered O(1).
bool isPossible(vector<vector<char>> &board, char val, int row, int col, int n)
{
  // same row
  for (int c = 0; c < n; c++)
  {
    if (board[row][c] == val)
      return false;
  }

  // same col
  for (int r = 0; r < n; r++)
  {
    if (board[r][col] == val)
      return false;
  }

  // same grid
  int sRow = (row / 3) * 3;
  int sCol = (col / 3) * 3;
  for (int i = sRow; i < sRow + 3; i++)
  {
    for (int j = sCol; j < sCol + 3; j++)
    {
      if (board[i][j] == val)
      {
        return false;
      }
    }
  }

  return true;
}

bool solve(vector<vector<char>> &board, int n)
{
  for (int i = 0; i < n; i++)
  {
    for (int j = 0; j < n; j++)
    {
      if (board[i][j] == '.')
      {
        for (char val = '1'; val <= '9'; val++)
        {
          if (isPossible(board, val, i, j, n))
          {
            board[i][j] = val;
            if (solve(board, n))
              return true;
            board[i][j] = '.';
          }
        }

        return false;
      }
    }
  }

  return true;
}

void solveSudoku(vector<vector<char>> &board)
{
  int n = board.size();

  solve(board, n);
}