#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

using namespace std;

map<string, vector<string>> graph;
map<string, int> disc, low;
set<string> articulationNodes;
set<pair<string, string>> bridges;

int timer = 0;

void dfs(const string &u, const string &parent) {
  disc[u] = low[u] = ++timer;
  int children = 0;

  for (const string &v : graph[u]) {
    if (v == parent)
      continue;

    if (disc[v] == 0) {
      children++;
      dfs(v, u);

      low[u] = min(low[u], low[v]);

      if (parent != "" && low[v] >= disc[u]) {
        articulationNodes.insert(u);
      }

      if (low[v] > disc[u]) {
        string a = u;
        string b = v;
        if (b < a)
          swap(a, b);
        bridges.insert({a, b});
      }
    } else {
      low[u] = min(low[u], disc[v]);
    }
  }

  if (parent == "" && children > 1) {
    articulationNodes.insert(u);
  }
}

int main(int argc, char *argv[]) {
  if (argc != 2)
    return 1;

  ifstream input(argv[1]);
  if (!input)
    return 1;

  string u, v;

  while (input >> u >> v) {
    graph[u].push_back(v);
    graph[v].push_back(u);

    disc[u] = 0;
    disc[v] = 0;
    low[u] = 0;
    low[v] = 0;
  }

  string start = graph.begin()->first;
  dfs(start, "");

  cout << "Found " << articulationNodes.size() << " articulation nodes:";

  for (const string &node : articulationNodes) {
    cout << " " << node;
  }

  cout << endl;

  cout << "Found " << bridges.size() << " bridges:";

  for (const auto &bridge : bridges) {
    cout << " " << bridge.first << "-" << bridge.second;
  }

  cout << endl;

  return 0;
}