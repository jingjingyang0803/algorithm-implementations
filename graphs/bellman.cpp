// Shortest paths vs negative cycles implementation using Bellman-Ford
// algorithm.
// The program reads a directed graph from a file and outputs the shortest paths
// from a given source node to all other nodes, or detects if there is a
// negative cycle reachable from the source.
// The graph is given as a list of edges, where each line contains three
// integers: the source node, the destination node, and the weight of the edge.
// The program takes two command-line arguments: the filename and the source
// node.
// Example usage:
//   ./bellman graph.txt 0

#include <algorithm>
#include <climits>
#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <vector>
using namespace std;

struct Edge {
  int from;
  int to;
  long long w;
};

int main(int argc, char *argv[]) {
  string filename = argv[1];
  int s = stoi(argv[2]);

  ifstream fin(filename);

  set<int> nodes;
  map<int, vector<Edge>> adj;
  map<int, map<int, long long>> weight;

  int u, v;
  long long w;

  while (fin >> u >> v >> w) {
    nodes.insert(u);
    nodes.insert(v);

    adj[u].push_back({u, v, w});
    weight[u][v] = w;
  }

  nodes.insert(s);

  for (int x : nodes) {
    sort(adj[x].begin(), adj[x].end(), [](const Edge &a, const Edge &b) {
      if (a.to != b.to)
        return a.to < b.to;
      return a.w < b.w;
    });
  }

  const long long INF = LLONG_MAX / 4;

  map<int, long long> dist;
  map<int, int> parent;

  for (int x : nodes)
    dist[x] = INF;
  dist[s] = 0;

  set<int> improved;
  improved.insert(s);

  int n = nodes.size();

  for (int iter = 1; iter <= n; iter++) {
    set<int> nextImproved;

    for (int x : improved) {
      if (dist[x] == INF)
        continue;

      for (const Edge &e : adj[x]) {
        if (dist[x] + e.w < dist[e.to]) {
          dist[e.to] = dist[x] + e.w;
          parent[e.to] = x;
          nextImproved.insert(e.to);
        }
      }
    }

    if (nextImproved.empty()) {
      cout << "No improvements in iteration " << iter << "\n";

      cout << "Distances from " << s << ": ";
      bool first = true;
      for (int x : nodes) {
        if (!first)
          cout << ", ";
        first = false;

        cout << "d(" << x << ") = ";
        if (dist[x] == INF)
          cout << "INF";
        else
          cout << dist[x];
      }
      cout << "\n";
      return 0;
    }

    cout << "Improvements in iteration " << iter << ": ";
    bool first = true;
    for (int x : nextImproved) {
      if (!first)
        cout << ", ";
      first = false;
      cout << "d(" << x << ") = " << dist[x];
    }
    cout << "\n";

    if (iter == n) {
      int cur = *nextImproved.begin();

      map<int, int> pos;
      vector<int> path;

      while (!pos.count(cur)) {
        pos[cur] = path.size();
        path.push_back(cur);
        cur = parent[cur];
      }

      int start = pos[cur];

      vector<int> cycle;
      cycle.push_back(cur);

      for (int i = (int)path.size() - 1; i > start; i--) {
        cycle.push_back(path[i]);
      }

      cycle.push_back(cur);

      long long cost = 0;
      for (int i = 0; i + 1 < (int)cycle.size(); i++) {
        cost += weight[cycle[i]][cycle[i + 1]];
      }

      cout << "A negative cycle with cost " << cost << " detected:";
      for (int x : cycle)
        cout << " " << x;
      cout << "\n";
      return 0;
    }

    improved = nextImproved;
  }

  return 0;
}