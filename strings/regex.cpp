// NFA construction from regular expressions
// This program reads a regular expression in a specific format from a file and
// constructs a non-deterministic finite automaton (NFA) for it. The regular
// expression is given as a list of expressions, where each line contains an
// expression ID, an operator, and the operands (if any).
// The program takes one command-line argument: the filename.
// Example usage:
//   ./regex regex.txt

#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

struct Trans {
  int to;
  string lab;
};

struct State {
  vector<Trans> trans;
};

struct Expr {
  string op, val;
  int a = -1, b = -1;
};

struct NFA {
  int start, end;
  vector<int> nodes;
};

vector<State> st;
map<int, Expr> exprs;

int newState() {
  st.push_back(State());
  return (int)st.size() - 1;
}

void addTrans(int u, int v, const string &lab) {
  st[u].trans.push_back({v, lab});
}

vector<int> mergeNodes(vector<int> a, vector<int> b, int skip = -1) {
  for (int x : b) {
    if (x != skip)
      a.push_back(x);
  }
  sort(a.begin(), a.end());
  a.erase(unique(a.begin(), a.end()), a.end());
  return a;
}

NFA build(int id) {
  Expr e = exprs[id];

  if (e.op == "str") {
    int start = newState();
    int cur = start;
    vector<int> nodes;
    nodes.push_back(start);

    if (e.val == "\\w") {
      int nxt = newState();
      addTrans(cur, nxt, "\\w");
      nodes.push_back(nxt);
      return {start, nxt, nodes};
    }

    string s = e.val;
    if (s == "\\.")
      s = ".";

    for (char c : s) {
      int nxt = newState();
      addTrans(cur, nxt, string(1, c));
      nodes.push_back(nxt);
      cur = nxt;
    }

    return {start, cur, nodes};
  }

  if (e.op == "concat") {
    NFA left = build(e.a);
    NFA right = build(e.b);

    for (auto tr : st[right.start].trans) {
      addTrans(left.end, tr.to, tr.lab);
    }

    for (int u : right.nodes) {
      if (u == right.start)
        continue;
      for (auto &tr : st[u].trans) {
        if (tr.to == right.start) {
          tr.to = left.end;
        }
      }
    }

    vector<int> nodes = mergeNodes(left.nodes, right.nodes, right.start);
    int newEnd = (right.end == right.start ? left.end : right.end);

    return {left.start, newEnd, nodes};
  }

  if (e.op == "|") {
    NFA left = build(e.a);
    NFA right = build(e.b);

    int start = newState();
    int end = newState();

    addTrans(start, left.start, "EPS");
    addTrans(start, right.start, "EPS");
    addTrans(left.end, end, "EPS");
    addTrans(right.end, end, "EPS");

    vector<int> nodes;
    nodes.push_back(start);
    nodes.push_back(end);
    nodes = mergeNodes(nodes, left.nodes);
    nodes = mergeNodes(nodes, right.nodes);

    return {start, end, nodes};
  }

  if (e.op == "*") {
    NFA child = build(e.a);

    int start = newState();
    int end = newState();

    addTrans(child.end, child.start, "EPS");
    addTrans(start, child.start, "EPS");
    addTrans(start, end, "EPS");
    addTrans(child.end, end, "EPS");

    vector<int> nodes;
    nodes.push_back(start);
    nodes.push_back(end);
    nodes = mergeNodes(nodes, child.nodes);

    return {start, end, nodes};
  }

  if (e.op == "+") {
    NFA child = build(e.a);

    addTrans(child.end, child.start, "EPS");

    return child;
  }

  return {-1, -1, {}};
}

vector<int> transitionOrder(int u) {
  vector<int> ord;

  for (int i = 0; i < (int)st[u].trans.size(); i++) {
    if (st[u].trans[i].lab != "EPS")
      ord.push_back(i);
  }

  for (int i = 0; i < (int)st[u].trans.size(); i++) {
    if (st[u].trans[i].lab == "EPS")
      ord.push_back(i);
  }

  return ord;
}

int main(int argc, char *argv[]) {
  ifstream fin(argv[1]);

  string line;
  while (getline(fin, line)) {
    if (line.empty())
      continue;

    stringstream ss(line);
    int num;
    string op;
    ss >> num >> op;

    Expr e;

    if (op == "concat") {
      e.op = "concat";
      ss >> e.a >> e.b;
    } else if (op == "|") {
      e.op = "|";
      ss >> e.a >> e.b;
    } else if (op == "*") {
      e.op = "*";
      ss >> e.a;
    } else if (op == "+") {
      e.op = "+";
      ss >> e.a;
    } else {
      e.op = "str";
      e.val = op;
    }

    exprs[num] = e;
  }

  NFA nfa = build(0);

  map<int, int> id;
  map<int, int> level;
  vector<int> oldByNew;

  queue<int> q;
  id[nfa.start] = 0;
  level[nfa.start] = 0;
  oldByNew.push_back(nfa.start);
  q.push(nfa.start);

  while (!q.empty()) {
    int u = q.front();
    q.pop();

    vector<int> ord = transitionOrder(u);

    for (int idx : ord) {
      int v = st[u].trans[idx].to;

      if (!id.count(v)) {
        id[v] = (int)id.size();
        level[v] = level[u] + 1;
        oldByNew.push_back(v);
        q.push(v);
      }
    }
  }

  map<int, vector<string>> out;

  for (int u : oldByNew) {
    vector<int> ord = transitionOrder(u);

    for (int idx : ord) {
      Trans tr = st[u].trans[idx];

      int h = level[u] + 1;

      string item = "[" + to_string(id[u]) + "-" + to_string(id[tr.to]) + ": " +
                    tr.lab + "]";

      out[h].push_back(item);
    }
  }

  cout << "NFA transitions:\n";

  for (auto &p : out) {
    cout << "Level " << p.first << ":";
    for (string x : p.second) {
      cout << " " << x;
    }
    cout << "\n";
  }

  return 0;
}