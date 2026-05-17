// All alignments of the longest common subsequence of two strings.
// The program uses dynamic programming to compute the length of the longest
// common subsequence (LCS) of two strings, and then uses depth-first search to
// find and print all alignments of the LCS. Each alignment consists of two
// lines: the first line shows the characters from the first string (with spaces
// for gaps), and the second line shows the characters from the second string
// (with spaces for gaps).
// The program takes one command-line argument: the filename. The file should
// contain two lines, each with a string.
// Example usage:
//   ./align strings.txt

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

string A, B;
vector<vector<int>> L;

void dfs(int i, int j, string top, string bottom) {
  if (i == 0 && j == 0) {
    reverse(top.begin(), top.end());
    reverse(bottom.begin(), bottom.end());

    cout << "\n";
    cout << top << "\n";
    cout << bottom << "\n";
    return;
  }

  if (i > 0 && j > 0 && A[i - 1] == B[j - 1] &&
      L[i][j] == L[i - 1][j - 1] + 1) {
    dfs(i - 1, j - 1, top + A[i - 1], bottom + B[j - 1]);
  }

  if (j > 0 && L[i][j] == L[i][j - 1]) {
    dfs(i, j - 1, top + ' ', bottom + B[j - 1]);
  }

  if (i > 0 && L[i][j] == L[i - 1][j]) {
    dfs(i - 1, j, top + A[i - 1], bottom + ' ');
  }
}

int main(int argc, char *argv[]) {
  ifstream fin(argv[1]);

  getline(fin, A);
  getline(fin, B);

  int m = A.size();
  int n = B.size();

  L.assign(m + 1, vector<int>(n + 1, 0));

  for (int i = 1; i <= m; i++) {
    for (int j = 1; j <= n; j++) {
      if (A[i - 1] == B[j - 1]) {
        L[i][j] = L[i - 1][j - 1] + 1;
      } else {
        L[i][j] = max(L[i - 1][j], L[i][j - 1]);
      }
    }
  }

  cout << "LCS length: " << L[m][n] << "\n";

  dfs(m, n, "", "");

  return 0;
}