// Bipartite matching using the Hungarian algorithm (Kuhn's algorithm)
// Time complexity: O(E * V) where E is the number of edges and V is the number
// of vertices in the left partition.
// The graph is given as a list of edges, where each line contains two strings
// representing the source and destination of the edge.
// The program takes one command-line argument: the filename.
// Example usage:
//   ./bipartite graph.txt

#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>
using namespace std;

map<string, vector<string>> adj;
map<string, string> matchR;
set<string> visited;

bool augment(const string &u) {
  for (const string &v : adj[u]) {
    if (visited.count(v))
      continue;
    visited.insert(v);

    if (!matchR.count(v) || augment(matchR[v])) {
      matchR[v] = u;
      return true;
    }
  }
  return false;
}

int main(int argc, char *argv[]) {
  ifstream fin(argv[1]);

  string u, v;
  set<string> leftNodes;

  while (fin >> u >> v) {
    leftNodes.insert(u);
    adj[u].push_back(v);
  }

  for (auto &p : adj) {
    sort(p.second.begin(), p.second.end());
  }

  int matchingSize = 0;

  for (const string &uNode : leftNodes) {
    visited.clear();
    if (augment(uNode)) {
      matchingSize++;
    }
  }

  cout << "A maximum bipartite matching with " << matchingSize << " pairs:\n";

  vector<pair<string, string>> result;
  for (auto &p : matchR) {
    result.push_back({p.second, p.first});
  }

  sort(result.begin(), result.end());

  for (auto &p : result) {
    cout << p.first << " " << p.second << "\n";
  }

  return 0;
}