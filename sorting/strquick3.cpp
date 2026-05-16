#include <fstream>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

vector<string> V;

int charAt(const string &s, int d) {
  if (d >= (int)s.size())
    return -1;
  return s[d];
}

void printList(const string &label) {
  cout << label << ":";
  for (const string &x : V)
    cout << " " << x;
  cout << "\n";
}

void quick3(int s, int e, int d) {
  if (s >= e) {
    cout << "Immediate return from subarray " << s << "..." << e
         << " with depth " << d << "\n";
    return;
  }

  int mid = (s + e) / 2;
  swap(V[s], V[mid]);

  string pivot = V[s];
  int p = charAt(pivot, d);

  cout << "Sorting subarray " << s << "..." << e << " with depth " << d
       << " and pivot " << pivot << "\n";

  int lt = s, gt = e, i = s + 1;

  while (i <= gt) {
    int c = charAt(V[i], d);

    if (c < p) {
      swap(V[lt], V[i]);
      lt++;
      i++;
    } else if (c > p) {
      swap(V[i], V[gt]);
      gt--;
    } else {
      i++;
    }
  }

  quick3(s, lt - 1, d);

  if (p != -1) {
    quick3(lt, gt, d + 1);
  }

  quick3(gt + 1, e, d);
}

int main(int argc, char *argv[]) {
  ifstream fin(argv[1]);

  string word;
  while (getline(fin, word)) {
    if (!word.empty())
      V.push_back(word);
  }

  printList("Original");

  if (!V.empty()) {
    quick3(0, (int)V.size() - 1, 0);
  }

  printList("Sorted");

  return 0;
}