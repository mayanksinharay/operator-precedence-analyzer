# Operator Precedence Analyzer

**Graph-Based Precedence Function Construction in C++**

A C++ tool that converts an operator-precedence relation table into a directed graph, merges equality constraints using Disjoint Set Union (DSU), and computes precedence functions using topological sorting and longest-path dynamic programming.

The project demonstrates how graph algorithms can be applied to a fundamental problem in compiler design: constructing compact precedence functions from pairwise operator relationships.

## Overview

Operator-precedence parsing is a bottom-up parsing technique that uses precedence relationships between terminals to determine their relative binding behavior.

An operator-relation table describes the relationship between pairs of terminals using three symbols:

* `<` — yields precedence
* `=` — equal precedence
* `>` — takes precedence

This project automates the construction of precedence functions \(f\) and \(g\) from a user-provided relation table.

The program builds a graph of precedence constraints, merges nodes that must have equal values, detects directed cycles, computes longest-path-based function values, and verifies whether the resulting functions satisfy the original relations.

## Features

* **Interactive input:** Accepts a terminal count, terminal symbols, and an operator-relation table.
* **DSU-based equality handling:** Merges \(f(a)\) and \(g(b)\) whenever the corresponding relation is `=`.
* **Graph construction:** Represents precedence constraints using an adjacency list implemented with `vector<set<int>>`.
* **Readable graph output:** Displays graph nodes as groups of \(f\) and \(g\) symbols, along with their outgoing edges.
* **Cycle detection:** Uses Kahn's topological sorting algorithm to detect cycles in the constructed graph.
* **Longest-path computation:** Calculates precedence-function values using dynamic programming over the topological ordering.
* **Precedence-function table:** Displays the computed \(f\) and \(g\) values for every terminal.
* **Constraint validation:** Checks the computed values against every specified relation in the original input table.

## How It Works

The program follows a sequence of graph-processing steps.

### 1. Read the relation table

The user provides \(n\) terminal symbols and an \(n \times n\) matrix of precedence relations.

For every terminal \(a\), the program creates two logical nodes:

* \(f(a)\)
* \(g(a)\)

The initial graph therefore contains \(2n\) nodes.

### 2. Merge equality constraints using DSU

The program uses a Disjoint Set Union data structure to merge nodes that must have equal precedence-function values.

For every relation \(a = b\), it merges:

$$
f(a) \equiv g(b)
$$

The DSU implementation uses path compression in its `find()` operation to accelerate representative lookup.

After processing all equality constraints, the program assigns a compact group index to each distinct representative.

### 3. Construct the precedence-function graph

The graph is stored using an adjacency list, with each vertex representing a group of equivalent \(f\) and \(g\) nodes.

The strict relations are converted into directed edges:

| Input relation | Constraint      | Edge direction                 |
| -------------- | --------------- | ------------------------------ |
| \(a < b\)      | \(f(a) < g(b)\) | \(g(b) \rightarrow f(a)\)      |
| \(a > b\)      | \(f(a) > g(b)\) | \(f(a) \rightarrow g(b)\)      |
| \(a = b\)      | \(f(a) = g(b)\) | Merge the corresponding groups |

The graph's edge direction is chosen so that the source node has a greater function value than the destination node.

For example, an edge \(u \rightarrow v\) imposes the strict ordering:

$$
value(u) > value(v)
$$

Edges connecting nodes in the same merged group are omitted.

### 4. Display the adjacency list

The program prints each graph vertex as a group of equivalent nodes and lists its outgoing neighbors.

For example, the output structure resembles:

```text
{ f(id), g(+) }  -->  { f(*) }, { g($) }
{ f(+), g(*) }   -->  None
```

These lines are illustrative only; the actual graph depends on the input relation table.

### 5. Detect cycles using topological sorting

Kahn's algorithm is used to produce a topological ordering of the graph.

The procedure is:

1. Calculate the indegree of each vertex.
2. Add all zero-indegree vertices to a queue.
3. Remove vertices from the queue and process their outgoing edges.
4. Decrease the indegree of each neighbor.
5. Add a neighbor to the queue when its indegree becomes zero.
6. Compare the number of processed vertices with the total vertex count.

If every vertex is processed, the graph is acyclic. Otherwise, the program reports that the graph contains a cycle and stops before computing the precedence-function table.

### 6. Compute precedence functions

The program traverses the topological ordering in reverse and calculates the longest-path-based value for each vertex.

For a vertex \(u\), the recurrence is:

$$
dp[u] = \max_{u \rightarrow v}\big(dp[v]+1\big)
$$

If \(u\) has no outgoing edges, its value remains zero.

The resulting values are mapped back to the original terminal symbols:

$$
f(a)=dp[group(f_a)]
$$

$$
g(a)=dp[group(g_a)]
$$

This assigns equal values to nodes merged by DSU and strictly ordered values to nodes connected by graph edges.

### 7. Validate the computed values

Finally, the program checks the original relation table against the calculated functions:

* `<` requires \(f(a)<g(b)\).
* `>` requires \(f(a)>g(b)\).
* `=` requires \(f(a)=g(b)\).

The program prints a success message if every specified relation is satisfied; otherwise, it displays a warning.

This validation step is important because an acyclic graph alone does not guarantee that every original constraint has been preserved correctly.

## Algorithm Summary

```text
INPUT:
    Number of terminals n
    Terminal symbols
    n × n operator-relation table

INITIALIZATION:
    Create 2n nodes: f(a) and g(a)
    Initialize DSU

EQUALITY PROCESSING:
    Merge f(a) and g(b) for each '=' relation

GRAPH CONSTRUCTION:
    Assign group IDs to DSU representatives
    Add directed edges for '<' and '>' relations
    Store edges in adjacency lists

CYCLE DETECTION:
    Run Kahn's topological sorting algorithm
    If a cycle exists, report an error and terminate

PRECEDENCE FUNCTIONS:
    Compute longest-path values in reverse topological order
    Map graph values to f(a) and g(a)

VALIDATION:
    Check every specified relation against computed values

OUTPUT:
    Precedence-function graph
    Adjacency list
    Precedence-function table
    Constraint-validation result
```

## Technology Stack

* **C++** — implementation language.
* **STL vectors** — terminal storage, relation matrix, and graph representation.
* **STL sets** — adjacency lists with duplicate-edge suppression.
* **STL maps** — mapping DSU representatives to graph group IDs.
* **Disjoint Set Union** — equality-constraint merging.
* **Graph algorithms** — topological sorting, cycle detection, and longest-path dynamic programming.

## Project Structure

```text
operator-precedence-analyzer/
├── main.cpp       # Program implementation
├── README.md      # Project documentation
├── Makefile       # Optional build automation
└── tests/         # Optional sample inputs and expected outputs
```

The filenames above are suggested. Adapt them to match the actual repository contents.

## Prerequisites

* A C++ compiler supporting C++11 or later.
* A terminal or command-line environment.

Check your compiler installation:

```bash
g++ --version
```

## Build and Run

Clone the repository:

```bash
git clone https://github.com/mayanksinharay/operator-precedence-analyzer.git
cd operator-precedence-analyzer
```

Compile the program:

```bash
g++ -std=c++11 -O2 -Wall -Wextra main.cpp -o precedence_analyzer
```

Run the executable:

```bash
./precedence_analyzer
```

On Windows, run:

```powershell
.\precedence_analyzer.exe
```

If your source file has a different name, replace `main.cpp` in the compilation command accordingly.

## Input Format

The program expects:

1. An integer \(n\), representing the number of terminals.
2. Exactly \(n\) terminal symbols.
3. Exactly \(n^2\) relation characters, arranged as an \(n \times n\) matrix.

The relation matrix is read as individual non-whitespace characters. Spaces and newlines may separate the entries.

Supported relation characters are:

* `<` — less-than precedence relation.
* `>` — greater-than precedence relation.
* `=` — equality relation.

Other characters are effectively ignored by the graph-construction and validation logic. Use only the supported relation symbols for meaningful results.

## Example Input

The following small example illustrates the input format for two terminals:

```text
2
id +
=
<
>
=
```

This corresponds to the matrix:

|      | `id` | `+` |
| ---- | ---- | --- |
| `id` | `=`  | `<` |
| `+`  | `>`  | `=` |

The example is intended to demonstrate input formatting. The resulting graph and precedence-function values are determined by the implementation.

## Complexity Analysis

Let:

* \(n\) be the number of terminals.
* \(V\) be the number of merged graph vertices.
* \(E\) be the number of unique directed graph edges.

### Time complexity

| Operation                                | Complexity                                                                       |
| ---------------------------------------- | -------------------------------------------------------------------------------- |
| Reading the relation matrix              | \(O(n^2)\)                                                                       |
| Processing equality constraints with DSU | \(O(n^2\alpha(n))\) amortized                                                    |
| Mapping DSU representatives to groups    | \(O(n\alpha(n))\) expected under standard map assumptions, plus map lookup costs |
| Graph construction                       | \(O(n^2\log V)\) with `set`-based adjacency lists                                |
| Topological sorting and cycle detection  | \(O(V+E)\)                                                                       |
| Longest-path dynamic programming         | \(O(V+E)\)                                                                       |
| Final relation validation                | \(O(n^2)\)                                                                       |

Here, \(\alpha(n)\) is the inverse Ackermann function, which grows extremely slowly.

For a dense relation table, the overall running time is dominated by processing the \(O(n^2)\) input relations and inserting graph edges.

### Space complexity

* Relation matrix: \(O(n^2)\).
* DSU and group mappings: \(O(n)\).
* Graph and longest-path data: \(O(V+E)\).

Overall space complexity is:

$$
O(n^2+V+E)
$$

## Limitations

* The program constructs precedence functions from a supplied relation matrix; it does not derive operator relations from a grammar.
* It does not implement a complete operator-precedence parser or parse source programs.
* The input is assumed to contain the expected number of terminals and relation entries.
* Unsupported relation characters are not explicitly rejected.
* A cycle is detected in the constructed graph, but a strict constraint that collapses into a single DSU group is skipped as an edge. Such a contradiction can instead be detected by the final validation step.
* The implementation uses longest-path values with zero-valued sinks. The resulting values are valid only when all original constraints are satisfied.

## Future Improvements

* Reject malformed input and unsupported relation symbols.
* Report contradictory equality and strict-order constraints more explicitly.
* Add automated test cases for valid, cyclic, and inconsistent relation tables.
* Export the graph in Graphviz DOT format for visualization.
* Support reading relation tables from files.
* Add a separate operator-precedence parsing engine that uses the generated functions.

## Learning Outcomes

This project explores practical applications of:

* Operator-precedence parsing concepts.
* Disjoint Set Union and path compression.
* Directed graph construction and adjacency lists.
* Kahn's topological sorting algorithm.
* Cycle detection in directed graphs.
* Longest-path dynamic programming on DAGs.
* Constraint validation and algorithmic testing.

## License

Add a `LICENSE` file to specify the terms under which the project may be used, modified, and distributed.

---

**A graph-based compiler-design utility for constructing and validating operator precedence functions using DSU and DAG algorithms.**
