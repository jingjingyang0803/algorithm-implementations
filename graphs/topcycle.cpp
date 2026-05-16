#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <vector>
using namespace std;

int main(int argc, char *argv[]) {
  ifstream fin(argv[1]);

  set<int> nodes;
  map<int, vector<int>> outEdges, inEdges;
  map<int, int> indeg;

  int u, v;
  while (fin >> u >> v) {
    nodes.insert(u);
    nodes.insert(v);

    outEdges[u].push_back(v);
    inEdges[v].push_back(u);

    indeg[v]++;
    if (!indeg.count(u))
      indeg[u] = 0;
  }

  for (int x : nodes) {
    sort(outEdges[x].begin(), outEdges[x].end());
    sort(inEdges[x].begin(), inEdges[x].end());
    if (!indeg.count(x))
      indeg[x] = 0;
  }

  priority_queue<int, vector<int>, greater<int>> sources;

  for (int x : nodes) {
    if (indeg[x] == 0)
      sources.push(x);
  }

  set<int> removed;
  vector<int> topo;

  while (!sources.empty()) {
    int x = sources.top();
    sources.pop();

    if (removed.count(x))
      continue;

    removed.insert(x);
    topo.push_back(x);

    for (int y : outEdges[x]) {
      indeg[y]--;
      if (indeg[y] == 0) {
        sources.push(y);
      }
    }
  }

  if ((int)topo.size() == (int)nodes.size()) {
    cout << "Found a topological order:";
    for (int x : topo)
      cout << " " << x;
    cout << "\n";
    return 0;
  }

  set<int> remaining;
  for (int x : nodes) {
    if (!removed.count(x))
      remaining.insert(x);
  }

  int cur = *remaining.begin();

  map<int, int> pos;
  vector<int> path;

  while (!pos.count(cur)) {
    pos[cur] = path.size();
    path.push_back(cur);

    int next = -1;
    for (int p : inEdges[cur]) {
      if (remaining.count(p)) {
        next = p;
        break;
      }
    }

    cur = next;
  }

  int start = pos[cur];
  vector<int> cycle;

  cycle.push_back(cur);
  for (int i = (int)path.size() - 1; i > start; i--) {
    cycle.push_back(path[i]);
  }
  cycle.push_back(cur);

  cout << "The graph is not acyclic, found a cycle:";
  for (int x : cycle)
    cout << " " << x;
  cout << "\n";

  return 0;
}