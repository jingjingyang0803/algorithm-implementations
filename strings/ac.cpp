// Aho-Corasick algorithm implementation for multiple pattern matching
// This program reads a list of patterns from a file and builds a trie data
// structure with a suffix function for efficient pattern matching.
// It then processes a given text and prints the matching process step by step.
// The program takes one command-line argument: the filename containing the text
// and patterns. The first line of the file is the text, and the subsequent
// lines are the patterns.
// Example usage:
//   ./ac patterns.txt

#include <algorithm>
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
  vector<Edge> edges;
  int fac;
  int depth;
};

vector<Node> trie;

int findEdge(int u, char c) {
  const vector<Edge> &e = trie[u].edges;
  int lo = 0, hi = (int)e.size();

  while (lo < hi) {
    int mid = (lo + hi) / 2;
    if (e[mid].ch == c)
      return mid;
    if (e[mid].ch < c)
      lo = mid + 1;
    else
      hi = mid;
  }

  return -1;
}

void insertPattern(const string &s) {
  int cur = 0;

  for (char c : s) {
    int idx = findEdge(cur, c);

    if (idx == -1) {
      int id = trie.size();
      trie.push_back({{}, -1, trie[cur].depth + 1});

      trie[cur].edges.push_back({c, id});
      sort(trie[cur].edges.begin(), trie[cur].edges.end(),
           [](const Edge &a, const Edge &b) { return a.ch < b.ch; });

      idx = findEdge(cur, c);
    }

    cur = trie[cur].edges[idx].to;
  }
}

void printTrieLevels() {
  cout << "Trie levels:\n";

  int maxDepth = 0;
  for (const Node &node : trie) {
    maxDepth = max(maxDepth, node.depth);
  }

  for (int h = 1; h <= maxDepth; h++) {
    vector<pair<pair<int, int>, char>> edges;

    for (int u = 0; u < (int)trie.size(); u++) {
      if (trie[u].depth != h - 1)
        continue;

      for (const Edge &e : trie[u].edges) {
        if (trie[e.to].depth == h) {
          edges.push_back({{u, e.to}, e.ch});
        }
      }
    }

    sort(edges.begin(), edges.end(),
         [](const pair<pair<int, int>, char> &a,
            const pair<pair<int, int>, char> &b) {
           if (a.first.first != b.first.first)
             return a.first.first < b.first.first;
           return a.second < b.second;
         });

    cout << " level " << h << ":";
    for (auto x : edges) {
      cout << " (" << x.first.first << "-" << x.first.second << ", " << x.second
           << ")";
    }
    cout << "\n";
  }
}

void buildSuffixFunction() {
  trie[0].fac = -1;

  queue<int> q;

  for (const Edge &e : trie[0].edges) {
    trie[e.to].fac = 0;
    q.push(e.to);
  }

  while (!q.empty()) {
    int u = q.front();
    q.pop();

    for (const Edge &e : trie[u].edges) {
      char c = e.ch;
      int v = e.to;

      int f = trie[u].fac;

      while (f != -1 && findEdge(f, c) == -1) {
        f = trie[f].fac;
      }

      if (f == -1) {
        trie[v].fac = 0;
      } else {
        trie[v].fac = trie[f].edges[findEdge(f, c)].to;
      }

      q.push(v);
    }
  }
}

void printSuffixFunction() {
  cout << "\n";
  cout << "Suffix function:\n";

  int maxDepth = 0;
  for (const Node &node : trie) {
    maxDepth = max(maxDepth, node.depth);
  }

  for (int h = 0; h <= maxDepth; h++) {
    vector<int> nodes;

    for (int i = 0; i < (int)trie.size(); i++) {
      if (trie[i].depth == h) {
        nodes.push_back(i);
      }
    }

    sort(nodes.begin(), nodes.end());

    cout << " level " << h << ":";
    for (int v : nodes) {
      cout << " <" << v << ": ";
      if (trie[v].fac == -1)
        cout << "null";
      else
        cout << trie[v].fac;
      cout << ">";
    }
    cout << "\n";
  }
}

void processText(const string &text) {
  cout << "\n";
  cout << "Processing the text '" << text << "':\n";

  int cur = 0;

  for (int i = 0; i < (int)text.size(); i++) {
    char c = text[i];

    cout << " " << c << ":";

    cout << " " << cur;

    while (cur != 0 && findEdge(cur, c) == -1) {
      cur = trie[cur].fac;
      cout << " " << cur;
    }

    int idx = findEdge(cur, c);

    if (idx != -1) {
      cur = trie[cur].edges[idx].to;
      cout << " " << cur;
    } else {
      if (cur != 0) {
        cur = 0;
        cout << " " << cur;
      }
    }

    cout << "\n";
  }
}

int main(int argc, char *argv[]) {
  ifstream fin(argv[1]);

  string text;
  getline(fin, text);

  trie.push_back({{}, -1, 0});

  string pattern;
  while (getline(fin, pattern)) {
    insertPattern(pattern);
  }

  printTrieLevels();
  buildSuffixFunction();
  printSuffixFunction();
  processText(text);

  return 0;
}