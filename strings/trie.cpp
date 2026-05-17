#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <vector>
using namespace std;

struct Edge {
  char ch;
  int to;
};

struct Node {
  int id;
  vector<Edge> edges;
  vector<int> prefixes;
};

vector<Node> trie;

char keyChar(char c) { return (char)tolower((unsigned char)c); }

int findEdgeIndex(int node, char ch) {
  char target = keyChar(ch);
  vector<Edge> &edges = trie[node].edges;

  int lo = 0;
  int hi = edges.size();

  while (lo < hi) {
    int mid = (lo + hi) / 2;
    char midChar = keyChar(edges[mid].ch);

    if (midChar == target)
      return mid;
    if (midChar < target)
      lo = mid + 1;
    else
      hi = mid;
  }

  return -1;
}

void insertString(const string &s, int stringIndex) {
  int cur = 0;

  for (char c : s) {
    int idx = findEdgeIndex(cur, c);

    if (idx == -1) {
      int newId = trie.size();
      trie.push_back({newId, {}, {}});

      Edge e{c, newId};

      vector<Edge> &edges = trie[cur].edges;
      edges.push_back(e);

      sort(edges.begin(), edges.end(), [](const Edge &a, const Edge &b) {
        return keyChar(a.ch) < keyChar(b.ch);
      });

      idx = findEdgeIndex(cur, c);
    }

    cur = trie[cur].edges[idx].to;
    trie[cur].prefixes.push_back(stringIndex);
  }
}

void printTrieLevels() {
  cout << "Trie levels:\n";

  queue<pair<int, int>> q;
  q.push({0, 0});

  vector<vector<pair<pair<int, int>, char>>> byLevel;

  while (!q.empty()) {
    int u = q.front().first;
    int depth = q.front().second;
    q.pop();

    for (const Edge &e : trie[u].edges) {
      int v = e.to;

      if ((int)byLevel.size() <= depth + 1) {
        byLevel.resize(depth + 2);
      }

      byLevel[depth + 1].push_back({{u, v}, e.ch});
      q.push({v, depth + 1});
    }
  }

  for (int level = 1; level < (int)byLevel.size(); level++) {
    sort(byLevel[level].begin(), byLevel[level].end(),
         [](const pair<pair<int, int>, char> &a,
            const pair<pair<int, int>, char> &b) {
           if (a.first.first != b.first.first)
             return a.first.first < b.first.first;
           return keyChar(a.second) < keyChar(b.second);
         });

    cout << " level " << level << ":";
    for (auto e : byLevel[level]) {
      cout << " (" << e.first.first << "-" << e.first.second << ", " << e.second
           << ")";
    }
    cout << "\n";
  }
}

int binarySearchPrint(int node, char ch, const string &P, int i) {
  char target = keyChar(ch);
  vector<Edge> &edges = trie[node].edges;

  int lo = 0;
  int hi = edges.size();

  cout << "  searching " << P[i] << ":";

  while (lo < hi) {
    int mid = (lo + hi) / 2;
    char midChar = keyChar(edges[mid].ch);

    cout << " [" << lo << ", " << hi << ", " << midChar << "]";

    if (midChar == target) {
      cout << "\n";
      return mid;
    }

    if (midChar < target)
      lo = mid + 1;
    else
      hi = mid;
  }

  cout << "\n";
  return -1;
}

void matchPattern(const string &P) {
  cout << "\n";
  cout << "Matching P = " << P << "\n";

  int cur = 0;

  for (int i = 0; i < (int)P.size(); i++) {
    int idx = binarySearchPrint(cur, P[i], P, i);

    if (idx == -1) {
      cout << "  matching failed at " << cur << " with character P[" << i
           << "] = " << P[i] << "\n";
      return;
    }

    int nxt = trie[cur].edges[idx].to;

    cout << "  move from " << cur << " to " << nxt << " with character P[" << i
         << "] = " << P[i] << "\n";

    cur = nxt;

    if (i == (int)P.size() - 1) {
      vector<int> ans = trie[cur].prefixes;
      sort(ans.begin(), ans.end());
      ans.erase(unique(ans.begin(), ans.end()), ans.end());

      cout << "  P matches with (prefixes of):";
      for (int x : ans)
        cout << " S" << x;
      cout << "\n";
    }
  }
}

int main(int argc, char *argv[]) {
  ifstream stringFile(argv[1]);
  ifstream patternFile(argv[2]);

  trie.push_back({0, {}, {}});

  string s;
  int idx = 1;

  while (getline(stringFile, s)) {
    if (!s.empty()) {
      insertString(s, idx);
      idx++;
    }
  }

  printTrieLevels();

  string p;
  while (getline(patternFile, p)) {
    if (!p.empty()) {
      matchPattern(p);
    }
  }

  return 0;
}