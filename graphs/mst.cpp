#include <algorithm>
#include <fstream>
#include <iostream>
#include <queue>
#include <set>
#include <vector>
using namespace std;

struct Edge {
  int from, to, w;
};

struct CompareEdge {
  bool operator()(const Edge &a, const Edge &b) const {
    if (a.w != b.w)
      return a.w > b.w;
    if (a.from != b.from)
      return a.from > b.from;
    return a.to > b.to;
  }
};

int main(int argc, char *argv[]) {
  if (argc != 3)
    return 1;

  string filename = argv[1];
  int start = stoi(argv[2]);

  ifstream input(filename);
  if (!input)
    return 1;

  vector<Edge> edges;
  int maxNode = 0;

  int u, v, w;
  while (input >> u >> v >> w) {
    edges.push_back({u, v, w});
    maxNode = max(maxNode, max(u, v));
  }

  vector<vector<Edge>> adj(maxNode + 1);

  for (const auto &e : edges) {
    adj[e.from].push_back(e);
    adj[e.to].push_back(e);
  }

  vector<bool> inMST(maxNode + 1, false);
  priority_queue<Edge, vector<Edge>, CompareEdge> pq;

  inMST[start] = true;

  for (const auto &e : adj[start]) {
    pq.push(e);
  }

  vector<Edge> mstEdges;
  int totalCost = 0;
  int iteration = 1;

  while (!pq.empty()) {
    cout << "** Iteration " << iteration << " **" << endl;
    iteration++;

    Edge e = pq.top();
    pq.pop();

    bool fromIn = inMST[e.from];
    bool toIn = inMST[e.to];

    if (!fromIn || !toIn) {
      int newNode;

      if (!fromIn)
        newNode = e.from;
      else
        newNode = e.to;

      cout << "Adding the edge (" << e.from << ", " << e.to << ", " << e.w
           << ") with the new node " << newNode << endl;

      inMST[newNode] = true;
      mstEdges.push_back(e);
      totalCost += e.w;

      for (const auto &f : adj[newNode]) {
        if (!inMST[f.from] || !inMST[f.to]) {
          pq.push(f);
        }
      }
    }
  }

  sort(mstEdges.begin(), mstEdges.end(), [](const Edge &a, const Edge &b) {
    if (a.from != b.from)
      return a.from < b.from;
    return a.to < b.to;
  });

  cout << "MST(" << totalCost << "):";

  for (const auto &e : mstEdges) {
    cout << " " << e.from << "-" << e.to;
  }

  cout << endl;

  return 0;
}