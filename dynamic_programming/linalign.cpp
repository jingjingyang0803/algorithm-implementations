// Linear space LCS path computation using divide-and-conquer approach.
// This program computes the longest common subsequence (LCS) of two strings A
// and B using a divide-and-conquer approach. It uses dynamic programming to
// compute the LCS lengths in both forward and backward directions, and then
// recursively finds the path of the LCS through the matrix. The results are
// printed in a specific format showing the indices of the path cells.
// The program takes one command-line argument: the filename. The file should
// contain two lines, each with a string.
// Example usage:
//   ./linalign strings.txt

#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>
using namespace std;

string A, B;
map<int, int> pathCell;

vector<int> forwardLCS(int a, int b, int c, int mid) {
  int rows = b - a + 1;
  int cols = mid - c + 1;

  vector<int> prev(rows + 1, 0), cur(rows + 1, 0);

  for (int j = 1; j <= cols; j++) {
    cur[0] = 0;
    for (int i = 1; i <= rows; i++) {
      if (A[a + i - 1] == B[c + j - 1]) {
        cur[i] = prev[i - 1] + 1;
      } else {
        cur[i] = max(prev[i], cur[i - 1]);
      }
    }
    prev = cur;
  }

  return prev;
}

vector<int> backwardLCS(int a, int b, int midPlusOne, int d) {
  int rows = b - a + 1;
  int cols = d - midPlusOne + 1;

  vector<int> prev(rows + 1, 0), cur(rows + 1, 0);

  for (int j = 1; j <= cols; j++) {
    cur[rows] = 0;
    for (int i = rows - 1; i >= 0; i--) {
      if (A[a + i] == B[d - j + 1]) {
        cur[i] = prev[i + 1] + 1;
      } else {
        cur[i] = max(prev[i], cur[i + 1]);
      }
    }
    prev = cur;
  }

  return prev;
}

void recursivePath(int a, int b, int c, int d) {
  cout << "[";
  if (a <= b)
    cout << A.substr(a, b - a + 1);
  cout << " " << a << " " << b << " ";

  if (c <= d)
    cout << B.substr(c, d - c + 1);
  cout << " " << c << " " << d << "]\n";

  int mid = (c + d) / 2;

  vector<int> left = forwardLCS(a, b, c, mid);
  vector<int> right = backwardLCS(a, b, mid + 1, d);

  int rows = b - a + 1;

  int bestI = -1;
  int bestVal = -1;

  for (int r = 0; r <= rows; r++) {
    int val = left[r] + right[r];
    if (val > bestVal) {
      bestVal = val;
      bestI = r;
    }
  }

  int i = a + bestI - 1;
  int row = i + 1;
  int col = mid + 1;

  pathCell[col] = row;

  if (c < mid) {
    recursivePath(a, i, c, mid);
  }

  if (mid + 1 < d) {
    recursivePath(i + 1, b, mid + 1, d);
  }
}

int main(int argc, char *argv[]) {
  ifstream fin(argv[1]);

  getline(fin, A);
  getline(fin, B);

  int m = A.size();
  int n = B.size();

  if (n >= 2) {
    recursivePath(0, m - 1, 0, n - 1);
  }

  cout << " ";
  for (int j = 1; j <= n - 1; j++) {
    if (j > 1)
      cout << " ";
    cout << "(" << pathCell[j] << ", " << j << ")";
  }
  cout << "\n";

  return 0;
}