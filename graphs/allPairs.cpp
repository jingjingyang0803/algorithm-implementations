#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>
using namespace std;

const long long INF = 4000000000000000000LL;

struct Edge {
  int from, to;
  bool exists;
};

void printTables(int k, const vector<vector<long long>> &D,
                 const vector<vector<Edge>> &spEdge) {
  int n = D.size();

  cout << "Iteration " << k << "\n";

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (D[i][j] >= INF / 2)
        cout << setw(5) << "inf";
      else
        cout << setw(5) << D[i][j];
    }
    cout << "\n";
  }
  cout << "\n";

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (!spEdge[i][j].exists) {
        cout << setw(7) << "null";
      } else {
        string s =
            to_string(spEdge[i][j].from) + "-" + to_string(spEdge[i][j].to);
        cout << setw(7) << s;
      }
    }
    cout << "\n";
  }
  cout << "\n";
}

vector<int> getCycle(int start, const vector<vector<Edge>> &spEdge) {
  int n = spEdge.size();
  vector<int> pos(n + 1, -1);
  vector<int> path;

  int cur = start;

  while (true) {
    if (pos[cur] != -1) {
      vector<int> cycle;
      for (int i = pos[cur]; i < (int)path.size(); i++) {
        cycle.push_back(path[i]);
      }
      cycle.push_back(cur);
      return cycle;
    }

    pos[cur] = path.size();
    path.push_back(cur);

    Edge e = spEdge[cur - 1][start - 1];
    cur = e.to;
  }
}

bool checkNegativeCycle(const vector<vector<long long>> &D,
                        const vector<vector<Edge>> &spEdge) {
  int n = D.size();

  for (int i = 0; i < n; i++) {
    if (D[i][i] < 0) {
      vector<int> cycle = getCycle(i + 1, spEdge);

      cout << "A negative cycle detected:";
      for (int x : cycle)
        cout << " " << x;
      cout << "\n";

      return true;
    }
  }

  return false;
}

int main(int argc, char *argv[]) {
  ifstream fin(argv[1]);

  vector<tuple<int, int, long long>> edges;
  int u, v;
  long long w;
  int n = 0;

  while (fin >> u >> v >> w) {
    edges.push_back(make_tuple(u, v, w));
    n = max(n, max(u, v));
  }

  vector<vector<long long>> D(n, vector<long long>(n, INF));
  vector<vector<Edge>> spEdge(n, vector<Edge>(n, {0, 0, false}));

  for (int i = 0; i < n; i++) {
    D[i][i] = 0;
  }

  for (auto e : edges) {
    int a = get<0>(e);
    int b = get<1>(e);
    long long c = get<2>(e);

    if (c < D[a - 1][b - 1]) {
      D[a - 1][b - 1] = c;
      spEdge[a - 1][b - 1] = {a, b, true};
    }
  }

  printTables(0, D, spEdge);
  if (checkNegativeCycle(D, spEdge))
    return 0;

  for (int k = 1; k <= n; k++) {
    int kk = k - 1;

    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        if (D[i][kk] >= INF / 2 || D[kk][j] >= INF / 2)
          continue;

        long long cand = D[i][kk] + D[kk][j];

        if (cand < D[i][j]) {
          D[i][j] = cand;
          spEdge[i][j] = spEdge[i][kk];
        }
      }
    }

    printTables(k, D, spEdge);

    if (checkNegativeCycle(D, spEdge))
      return 0;
  }

  return 0;
}