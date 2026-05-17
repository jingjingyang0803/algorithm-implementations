// Suffix array construction and binary search
// This program reads a text and a pattern from a file, constructs the suffix
// array for the text, and then performs a binary search to find the pattern in
// the text.
// The program takes one command-line argument: the filename containing the text
// and pattern. The first line of the file is the text, and the second line is
// the pattern.
// Example usage:
//   ./suffarr input.txt

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

string T, P;

int main(int argc, char *argv[]) {
  ifstream fin(argv[1]);

  getline(fin, T);
  getline(fin, P);

  int n = T.size();
  int m = P.size();

  vector<int> SA(n);
  for (int i = 0; i < n; i++)
    SA[i] = i;

  sort(SA.begin(), SA.end(),
       [](int a, int b) { return T.substr(a) < T.substr(b); });

  cout << "SA:";
  for (int x : SA)
    cout << " " << x;
  cout << "\n";

  cout << "Binary search for " << P << "\n";

  int low = 0;
  int high = n;

  while (low < high) {
    int mid = (low + high) / 2;
    int pos = SA[mid];

    int k = 0;
    while (k < m && pos + k < n && P[k] == T[pos + k]) {
      k++;
    }

    int last = k;
    if (last == m)
      last = m - 1;

    string pPart = P.substr(0, last + 1);
    string tPart = T.substr(pos, last + 1);

    cout << "(" << low << ", " << mid << ", " << high << "): " << pPart
         << " <=> " << tPart << "\n";

    if (k == m) {
      cout << "Found the pattern at location " << pos << "\n";
      return 0;
    }

    if (pos + k >= n || P[k] > T[pos + k]) {
      low = mid + 1;
    } else {
      high = mid;
    }
  }

  return 0;
}