// Problem - https://www.geeksforgeeks.org/problems/rat-in-a-maze-problem/1
//
// BRUTE FORCE - Backtracking
//
// TC - O(3^(n * m))
//      - Let n × m be the size of the maze.
//      - Backtracking explores all possible paths.
//      - From each cell, there are at most 4 directions,
//        but one direction (the parent cell) is already marked
//        as visited, leaving at most 3 valid recursive choices.
//      - Each recursive call performs only O(1) work
//        (boundary check, destination check, mark/unmark visited).
//      - Therefore: O(3^(n * m)) states × O(1) work
//                   = O(3^(n * m))
//
// SC - O(n * m) Auxiliary Space
//      - Recursion stack can grow up to n × m levels
//        in the longest possible path.
//      - Maze is modified in-place for visited marking,
//        so no extra visited array is required.
//      - Therefore auxiliary space = O(n * m).
//
//      O(s * n * m) Result Space
//      - Let s be the number of valid paths.
//      - Each path can have a maximum length of (n × m - 1).
//      - Therefore result space = O(s * n * m).
void solve(int row, int col, string path, vector<string> &res, vector<vector<int>> &maze, int n, int m)
{
  if (row < 0 || col < 0 || row >= n || col >= m || maze[row][col] == 0)
  {
    return;
  }

  if (row == n - 1 && col == m - 1)
  {
    res.push_back(path);
    return;
  }

  maze[row][col] = 0;

  // bottom
  solve(row + 1, col, path + 'D', res, maze, n, m);

  // left
  solve(row, col - 1, path + 'L', res, maze, n, m);

  // right
  solve(row, col + 1, path + 'R', res, maze, n, m);

  // top
  solve(row - 1, col, path + 'U', res, maze, n, m);

  maze[row][col] = 1;
}

vector<string> ratInMaze(vector<vector<int>> &maze)
{
  int n = maze.size();
  int m = maze[0].size();

  vector<string> res;

  solve(0, 0, "", res, maze, n, m);

  return res;
}