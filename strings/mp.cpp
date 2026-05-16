// MP string matching using KMP algorithm
// This program reads a pattern and a text from a file and finds all occurrences
// of the pattern in the text using the Knuth-Morris-Pratt (KMP) algorithm.
// The program takes one command-line argument: the filename.
// Example usage:
//   ./mp pattern.txt

#include <cctype>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

string lowerString(string s) {
  for (char &c : s)
    c = (char)tolower((unsigned char)c);
  return s;
}

vector<int> buildFm(const string &P) {
  int m = P.size();
  vector<int> fm(m, 0);

  int j = 0;
  for (int i = 1; i < m; i++) {
    while (j > 0 && P[i] != P[j]) {
      j = fm[j - 1];
    }

    if (P[i] == P[j]) {
      j++;
    }

    fm[i] = j;
  }

  return fm;
}

int main(int argc, char *argv[]) {
  ifstream fin(argv[1]);

  string Porig, Torig;
  getline(fin, Porig);
  getline(fin, Torig);

  string P = lowerString(Porig);
  string T = lowerString(Torig);

  int m = P.size();
  int n = T.size();

  vector<int> fm = buildFm(P);

  cout << "P: " << Porig << "\n";

  cout << "Function fm:";
  for (int x : fm)
    cout << " " << x;
  cout << "\n";

  int i = 0;
  int j = 0;

  while (i < n && i - j <= n - m) {
    int align = i - j;

    cout << "P at pos " << align << " with i = " << i << " and j = " << j
         << "\n";

    int matchedStartI = i;
    int matchedStartJ = j;

    while (i < n && j < m && T[i] == P[j]) {
      i++;
      j++;
    }

    if (i > matchedStartI) {
      cout << "  matched T[" << matchedStartI << ".." << i - 1
           << "] = " << Torig.substr(matchedStartI, i - matchedStartI)
           << " = P[" << matchedStartJ << ".." << j - 1
           << "] = " << Porig.substr(matchedStartJ, j - matchedStartJ) << "\n";
    }

    if (j == m) {
      cout << "  found an occurrence of P\n";

      int oldJ = j;
      j = fm[j - 1];

      cout << "  updated j from " << oldJ << " to fm[" << oldJ - 1
           << "] = " << j << "\n";
    } else if (i < n) {
      cout << "  mismatch T[" << i << "] = " << Torig[i] << " != P[" << j
           << "] = " << Porig[j] << "\n";

      if (j > 0) {
        int oldJ = j;
        j = fm[j - 1];

        cout << "  updated j from " << oldJ << " to fm[" << oldJ - 1
             << "] = " << j << "\n";
      } else {
        int oldI = i;
        i++;

        cout << "  incremented i from " << oldI << " to " << i << "\n";
      }
    }
  }

  return 0;
}