// Dinitz' algorithm for maximum flow
// Time complexity: O(V^2 * E) in general, O(sqrt(V) * E) for unit networks.
// The graph is given as a list of edges, where the first line contains two
// integers representing the source and sink, and each subsequent line contains
// three integers representing the source, destination, and capacity of the
// edge.
// The program takes one command-line argument: the filename.
// Example usage:
//   ./dinitz graph.txt

#include <algorithm>
#include <climits>
#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <vector>
using namespace std;

struct Move {
  int next;
  int a, b;
  bool rev;
};

int s, t;
set<int> nodes;
map<int, map<int, int>> cap, flow;
map<int, int> level;
map<int, vector<Move>> moves;
map<int, int> ptr;
set<int> blocked;
int maxFlow = 0;

int residual(const Move &m) {
  if (!m.rev)
    return cap[m.a][m.b] - flow[m.a][m.b];
  return flow[m.a][m.b];
}

bool bfs() {
  level.clear();
  queue<int> q;

  level[s] = 0;
  q.push(s);

  while (!q.empty()) {
    int u = q.front();
    q.pop();

    for (int v : nodes) {
      if (level.count(v))
        continue;

      if (cap[u][v] > 0 && cap[u][v] - flow[u][v] > 0) {
        level[v] = level[u] + 1;
        q.push(v);
      } else if (cap[v][u] > 0 && flow[v][u] > 0) {
        level[v] = level[u] + 1;
        q.push(v);
      }
    }
  }

  return level.count(t);
}

void printLevelGraph(int iter) {
  cout << "Edges of the level graph for iteration " << iter << ":\n";

  int maxLev = level[t];

  for (int k = 1; k <= maxLev; k++) {
    vector<pair<int, int>> edges;

    for (int u : nodes) {
      for (int v : nodes) {
        if (!level.count(u) || !level.count(v))
          continue;

        if (cap[u][v] > 0 && cap[u][v] - flow[u][v] > 0 && level[u] == k - 1 &&
            level[v] == k) {
          edges.push_back({u, v});
        }

        if (cap[u][v] > 0 && flow[u][v] > 0 && level[v] == k - 1 &&
            level[u] == k) {
          edges.push_back({u, v});
        }
      }
    }

    sort(edges.begin(), edges.end());

    cout << " level " << k << ":";
    for (auto e : edges)
      cout << " " << e.first << "-" << e.second;
    cout << "\n";
  }
}

void buildMoves() {
  moves.clear();
  ptr.clear();
  blocked.clear();

  int maxLev = level[t];

  for (int u : nodes) {
    ptr[u] = 0;

    for (int v : nodes) {
      if (!level.count(u) || !level.count(v))
        continue;
      if (level[u] >= maxLev)
        continue;
      if (level[v] != level[u] + 1)
        continue;
      if (level[v] > maxLev)
        continue;

      if (cap[u][v] > 0 && cap[u][v] - flow[u][v] > 0) {
        moves[u].push_back({v, u, v, false});
      }

      if (cap[v][u] > 0 && flow[v][u] > 0) {
        moves[u].push_back({v, v, u, true});
      }
    }

    sort(moves[u].begin(), moves[u].end(),
         [](const Move &x, const Move &y) { return x.next < y.next; });
  }
}

bool dfs(int u, int bottleneck, vector<int> &path, vector<Move> &used) {
  if (u == t) {
    cout << "Found a path with capacity " << bottleneck << ":";
    for (int x : path)
      cout << " " << x;
    cout << "\n";

    for (Move m : used) {
      if (!m.rev)
        flow[m.a][m.b] += bottleneck;
      else
        flow[m.a][m.b] -= bottleneck;
    }

    maxFlow += bottleneck;
    return true;
  }

  while (ptr[u] < (int)moves[u].size()) {
    Move m = moves[u][ptr[u]];
    int r = residual(m);

    if (blocked.count(m.next) || r <= 0) {
      ptr[u]++;
      continue;
    }

    cout << "DFS steps from " << u << " to " << m.next;
    if (m.rev)
      cout << " with available reverse capacity " << r << "\n";
    else
      cout << " with available capacity " << r << "\n";

    path.push_back(m.next);
    used.push_back(m);

    if (dfs(m.next, min(bottleneck, r), path, used)) {
      if (residual(m) == 0)
        ptr[u]++;
      return true;
    }

    used.pop_back();
    path.pop_back();

    cout << "DFS backtracks from " << m.next << " to " << u << "\n";
    ptr[u]++;
  }

  if (u != s)
    blocked.insert(u);
  if (u == s)
    cout << "\n";

  return false;
}

int main(int argc, char *argv[]) {
  ifstream fin(argv[1]);

  fin >> s >> t;
  nodes.insert(s);
  nodes.insert(t);

  int u, v, c;
  while (fin >> u >> v >> c) {
    nodes.insert(u);
    nodes.insert(v);
    cap[u][v] += c;
    flow[u][v] = 0;
  }

  int iter = 1;

  while (bfs()) {
    printLevelGraph(iter);
    buildMoves();

    while (true) {
      vector<int> path;
      vector<Move> used;
      path.push_back(s);

      if (!dfs(s, INT_MAX, path, used))
        break;
    }

    iter++;
  }

  cout << "Maximum flow: " << maxFlow << "\n";
  return 0;
}