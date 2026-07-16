// Problem - https://leetcode.com/problems/n-queens/

// BRUTE FORCE - Backtracking
//
// TC - O(n * n!)
//      - Backtracking explores at most O(n!) valid queen arrangements,
//        since each row places one queen and already-used columns cannot be reused.
//      - For each column, isSafe() takes O(n) to check the column and diagonals.
//      - Therefore: O(n!) states * O(n) safety check
//                   = O(n * n!)
//
// SC - O(n^2) Auxiliary Space
//      - Board requires O(n^2) space.
//      - Recursion stack requires O(n) space.
//      - Therefore auxiliary space = O(n^2).
//
//      O(s * n^2) Result Space
//      - There are s valid solutions.
//      - Each solution stores an n * n board.
//      - Therefore result space = O(s * n^2).

bool isSafe(int row, int col, vector<string> &board, int n)
{
    // same column
    for (int i = row; i >= 0; i--)
    {
        if (board[i][col] == 'Q')
            return false;
    }

    // same diagonal - top left
    for (int i = row - 1, j = col - 1; i >= 0 && j >= 0; i--, j--)
    {
        if (board[i][j] == 'Q')
            return false;
    }

    // same diagonal - top right
    for (int i = row, j = col; i >= 0 && j < n; i--, j++)
    {
        if (board[i][j] == 'Q')
            return false;
    }
    return true;
}

void solve(int row, vector<string> &board, vector<vector<string>> &res, int n)
{
    if (row == n)
    {
        res.push_back(board);
        return;
    }

    for (int col = 0; col < n; col++)
    {
        if (isSafe(row, col, board, n))
        {
            board[row][col] = 'Q';
            solve(row + 1, board, res, n);
            board[row][col] = '.';
        }
    }
}

vector<vector<string>> solveNQueens(int n)
{
    vector<vector<string>> res;
    vector<string> board(n, string(n, '.'));

    solve(0, board, res, n);

    return res;
}

// OPTIMIZED - Backtracking with Column and Diagonal Hashing
//
// TC - O(n!)
//      - Backtracking explores at most O(n!) valid queen arrangements,
//        since each row places one queen and already-used columns cannot be reused.
//      - Checking whether a queen can be placed takes O(1) time using
//        cols, leftDia, and rightDia arrays.
//      - Placing and removing a queen also takes O(1) time.
//      - Therefore: O(n!) states * O(1) safety check
//                   = O(n!)
//
// SC - O(n^2) Auxiliary Space
//      - Board requires O(n^2) space.
//      - cols array requires O(n) space.
//      - leftDia and rightDia arrays require O(n) space each.
//      - Recursion stack requires O(n) space.
//      - Therefore auxiliary space = O(n^2).
//
//      O(s * n^2) Result Space
//      - There are s valid solutions.
//      - Each solution stores an n * n board.
//      - Therefore result space = O(s * n^2).

void solve(int row, vector<string> &board, vector<int> &cols, vector<int> &leftDia, vector<int> &rightDia, vector<vector<string>> &res, int n)
{
    if (row == n)
    {
        res.push_back(board);
        return;
    }

    for (int col = 0; col < n; col++)
    {
        if (cols[col] || leftDia[row - col + n - 1] || rightDia[row + col])
        {
            continue;
        }

        cols[col] = 1;
        leftDia[row - col + n - 1] = 1;
        rightDia[row + col] = 1;

        board[row][col] = 'Q';
        solve(row + 1, board, cols, leftDia, rightDia, res, n);
        board[row][col] = '.';

        cols[col] = 0;
        leftDia[row - col + n - 1] = 0;
        rightDia[row + col] = 0;
    }
}

vector<vector<string>> solveNQueens(int n)
{
    vector<vector<string>> res;
    vector<string> board(n, string(n, '.'));

    vector<int> col(n, 0);
    vector<int> leftDia(n * 2, 0);
    vector<int> rightDia(n * 2, 0);

    solve(0, board, col, leftDia, rightDia, res, n);

    return res;
}