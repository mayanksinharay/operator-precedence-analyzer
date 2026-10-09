#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <map>
#include <iomanip>
#include <algorithm>

using namespace std;

class DSU {
    vector<int> parent;

public:
    DSU(int n) {
        parent.resize(n);
        for (int i = 0; i < n; i++)
            parent[i] = i;
    }

    int find(int x) {
        if (parent[x] == x)
            return x;
        return parent[x] = find(parent[x]);
    }

    void unite(int a, int b) {
        a = find(a);
        b = find(b);

        if (a != b)
            parent[b] = a;
    }
};

int main() {

    int n;

    cout << "Enter the number of operators: ";
    cin >> n;

    vector<string> op(n);

    cout << "Enter the operators: ";
    for (int i = 0; i < n; i++)
        cin >> op[i];

    vector<vector<char>> relation(n, vector<char>(n));

    cout << "\nEnter the operator precedence relation table:\n";
    cout << "Use '>' for greater, '<' for lesser, '=' for equal.\n\n";

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> relation[i][j];
        }
    }

    int totalNodes = 2 * n;

    DSU dsu(totalNodes);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (relation[i][j] == '=') {
                dsu.unite(i, n + j);
            }
        }
    }

    map<int, int> groupNumber;
    int groupCount = 0;

    for (int i = 0; i < totalNodes; i++) {
        int root = dsu.find(i);

        if (groupNumber.find(root) == groupNumber.end()) {
            groupNumber[root] = groupCount++;
        }
    }

    vector<int> group(totalNodes);

    for (int i = 0; i < totalNodes; i++) {
        group[i] = groupNumber[dsu.find(i)];
    }

    vector<set<int>> graph(groupCount);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {

            if (relation[i][j] == '<') {
                int from = group[n + j];
                int to = group[i];

                if (from != to)
                    graph[from].insert(to);
            }

            else if (relation[i][j] == '>') {
                int from = group[i];
                int to = group[n + j];

                if (from != to)
                    graph[from].insert(to);
            }
        }
    }

    cout << "\n\n";
    cout << "============================================================\n";
    cout << "             OPERATOR PRECEDENCE GRAPH\n";
    cout << "============================================================\n\n";

    vector<vector<string>> groupMembers(groupCount);

    for (int i = 0; i < n; i++) {
        groupMembers[group[i]].push_back("f(" + op[i] + ")");
        groupMembers[group[n + i]].push_back("g(" + op[i] + ")");
    }

    for (int i = 0; i < groupCount; i++) {

        cout << "{ ";

        for (int j = 0; j < (int)groupMembers[i].size(); j++) {
            cout << groupMembers[i][j];

            if (j + 1 != (int)groupMembers[i].size())
                cout << ", ";
        }

        cout << " }";

        cout << "  -->  ";

        if (graph[i].empty()) {
            cout << "None";
        }
        else {
            bool first = true;

            for (int next : graph[i]) {

                if (!first)
                    cout << ", ";

                first = false;

                cout << "{ ";

                for (int k = 0; k < (int)groupMembers[next].size(); k++) {
                    cout << groupMembers[next][k];

                    if (k + 1 != (int)groupMembers[next].size())
                        cout << ", ";
                }

                cout << " }";
            }
        }

        cout << "\n";
    }

    vector<int> indegree(groupCount, 0);

    for (int u = 0; u < groupCount; u++) {
        for (int v : graph[u]) {
            indegree[v]++;
        }
    }

    vector<int> topo;
    vector<int> queue;

    for (int i = 0; i < groupCount; i++) {
        if (indegree[i] == 0)
            queue.push_back(i);
    }

    int front = 0;

    while (front < (int)queue.size()) {

        int u = queue[front++];

        topo.push_back(u);

        for (int v : graph[u]) {

            indegree[v]--;

            if (indegree[v] == 0)
                queue.push_back(v);
        }
    }

    if ((int)topo.size() != groupCount) {

        cout << "\n============================================================\n";
        cout << "ERROR: PRECEDENCE GRAPH CONTAINS A CYCLE\n";
        cout << "Precedence functions cannot be constructed.\n";
        cout << "============================================================\n";

        return 0;
    }

    vector<int> dp(groupCount, 0);

    for (int i = groupCount - 1; i >= 0; i--) {

        int u = topo[i];

        for (int v : graph[u]) {
            dp[u] = max(dp[u], dp[v] + 1);
        }
    }

    vector<int> f(n);
    vector<int> g(n);

    for (int i = 0; i < n; i++) {
        f[i] = dp[group[i]];
        g[i] = dp[group[n + i]];
    }

    cout << "\n\n";
    cout << "------------------------------------------------\n";
    cout << "          PRECEDENCE FUNCTION TABLE\n";
    cout << "------------------------------------------------\n";

    cout << left
         << setw(15) << "Operator"
         << setw(12) << "f()"
         << setw(12) << "g()"
         << "\n";

    cout << "------------------------------------------------\n";

    for (int i = 0; i < n; i++) {
        cout << left
             << setw(15) << op[i]
             << setw(12) << f[i]
             << setw(12) << g[i]
             << "\n";
    }

    cout << "------------------------------------------------\n";

    bool valid = true;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {

            bool ok = true;

            if (relation[i][j] == '<')
                ok = f[i] < g[j];

            else if (relation[i][j] == '>')
                ok = f[i] > g[j];

            else if (relation[i][j] == '=')
                ok = f[i] == g[j];

            if (!ok)
                valid = false;
        }
    }

    cout << "\n";

    if (valid)
        cout << "All precedence relations are satisfied.\n";
    else
        cout << "Warning: Some precedence relations are not satisfied.\n";

    cout << "\n";

    return 0;
}
