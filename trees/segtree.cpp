// Segment tree implementation for range sum queries and point updates.
// The program reads an array of values from a file and a list of commands from
// another file. The commands can be either "query L R" to query the sum of the
// elements from index L to R (inclusive) or "set I V" to set the value at index
// I to V.
// The program takes two command-line arguments: the filename of the array and
// the filename of the commands. Example usage:
//   ./segtree array.txt commands.txt

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

vector<long long> S;
int baseSize;

void printTree() {
  cout << "Segment tree levels:\n";

  for (int levelStart = 1; levelStart < 2 * baseSize; levelStart *= 2) {
    int levelEnd = levelStart * 2 - 1;

    cout << " ";
    for (int i = levelStart; i <= levelEnd; i++) {
      if (i > levelStart)
        cout << " ";
      cout << S[i];
    }
    cout << "\n";
  }

  cout << "\n";
}

void setValue(int index, long long value) {
  cout << "\n";
  cout << "Updating V[" << index << "] = " << value << "\n";

  int pos = baseSize + index;
  S[pos] = value;

  pos /= 2;
  while (pos >= 1) {
    S[pos] = S[2 * pos] + S[2 * pos + 1];
    pos /= 2;
  }
}

void query(int left, int right) {
  cout << "\n";
  cout << "Querying interval " << left << "..." << right << "\n";

  int L = baseSize + left;
  int R = baseSize + right;
  long long result = 0;

  while (L <= R) {
    cout << "  left and right positions: " << L << " " << R << "\n";

    if (L % 2 == 1) {
      long long old = result;
      result += S[L];
      cout << "    updated result from " << old << " to " << result
           << " using S[" << L << "]=" << S[L] << "\n";
      L++;
    }

    if (R % 2 == 0) {
      long long old = result;
      result += S[R];
      cout << "    updated result from " << old << " to " << result
           << " using S[" << R << "]=" << S[R] << "\n";
      R--;
    }

    L /= 2;
    R /= 2;
  }

  cout << "\n";
  cout << "Sum(" << left << "..." << right << ") = " << result << "\n";
}

int main(int argc, char *argv[]) {
  ifstream arrayFile(argv[1]);
  ifstream commandFile(argv[2]);

  vector<long long> values;
  long long x;

  while (arrayFile >> x) {
    values.push_back(x);
  }

  int n = values.size();
  baseSize = 1;
  while (baseSize < n)
    baseSize *= 2;

  S.assign(2 * baseSize, 0);

  for (int i = 0; i < n; i++) {
    S[baseSize + i] = values[i];
  }

  for (int i = baseSize - 1; i >= 1; i--) {
    S[i] = S[2 * i] + S[2 * i + 1];
  }

  printTree();

  string cmd;
  int par1;
  long long par2;

  while (commandFile >> cmd >> par1 >> par2) {
    if (cmd == "query") {
      query(par1, (int)par2);
    } else if (cmd == "set") {
      setValue(par1, par2);
    }
  }

  return 0;
}