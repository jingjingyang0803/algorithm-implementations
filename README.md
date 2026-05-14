# Data Structures and Algorithms

Implementation repository for the course exercises.

## Language

All tasks are implemented in:

- C++17

## Topics

### Graph Algorithms

| Task | Algorithm |
|---|---|
| Minimum spanning tree | Prim's algorithm |
| Articulation nodes and bridges | DFS low-link |
| Topological order or a cycle | DFS / Kahn |
| Shortest paths vs negative cycles | Bellman-Ford |
| All-pairs shortest paths | Floyd-Warshall |
| Bipartite matching | Kuhn algorithm |
| Dinitz' algorithm | Maximum flow |

---

### Trees and Range Queries

| Task | Algorithm |
|---|---|
| Segment tree | Range query tree |

---

### Sorting

| Task | Algorithm |
|---|---|
| 3-way quicksort for strings | String quicksort |

---

### String Algorithms

| Task | Algorithm |
|---|---|
| MP string matching | Morris-Pratt |
| Trie | Prefix tree |
| Aho-Corasick | Multiple pattern matching |
| Regular expressions | NFA simulation |
| Suffix array | Indexed string matching |

---

### Dynamic Programming

| Task | Algorithm |
|---|---|
| All alignments | Sequence alignment |
| Linear space LCS path | Hirschberg algorithm |

---

## Course Schedule

### Session 1
- Graph basics
- Minimum spanning trees
- Articulation nodes and bridges

### Session 2
- Topological sorting
- Shortest paths
- Negative cycles

### Session 3
- Flow networks
- Maximum flow
- Bipartite matching

### Session 4
- Segment trees
- Sorting algorithms
- String quicksort

### Session 5
- String matching
- Morris-Pratt
- Trie

### Session 6
- Aho-Corasick
- Automata
- Regular expressions

### Session 7
- Suffix structures
- Suffix array

### Session 8
- Longest common subsequences
- Alignment
- Edit distance

---

## Build

Example:

```bash
g++ -std=c++17 mst.cpp -o mst
./mst graph.txt 0
