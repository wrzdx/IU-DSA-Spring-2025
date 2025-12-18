// Evgenii Khovalyg DSAI-02

#include <algorithm>
#include <climits>
#include <iostream>
#include <list>
#include <vector>


using namespace std;

template<typename T>
class Queue {
    list<T> q;

public:
    Queue() = default;
    bool isEmpty() { return q.empty(); }
    void push(T element) { q.push_back(move(element)); }
    void pop() {
        if (isEmpty()) {
            throw runtime_error("Queue is empty");
        }
        q.pop_front();
    }
    T &front() {
        if (isEmpty()) {
            throw runtime_error("Queue is empty");
        }
        return q.front();
    }
    ~Queue() = default;
};

template<typename V, typename E>
class DirectedGraph {
public:
    class Vertex;
    class Edge;

private:
    list<Edge *> edges;
    list<Vertex *> vertices;
    int time;

    bool hasCycleDFS_Visit(Vertex *u) {
        u->color = GRAY;

        for (Edge *edge: u->adjacencyList) {
            Vertex *v = edge->to;
            if (v->color == GRAY) {
                return true;
            }
            if (v->color == WHITE) {
                if (hasCycleDFS_Visit(v)) {
                    return true;
                }
            }
        }

        u->color = BLACK;
        return false;
    }

public:
    enum Color { WHITE, GRAY, BLACK };

    DirectedGraph() : time(0) {}

    class Vertex {
    public:
        V value;
        list<Edge *> adjacencyList;
        Color color;
        int d;
        int f;
        Vertex *pi;

        Vertex(V val) : value(val), color(WHITE), d(0), f(0), pi(nullptr) {}
    };

    class Edge {
    public:
        Vertex *from;
        Vertex *to;
        E weight;

        Edge(Vertex *from, Vertex *to, E weight) :
            from(from), to(to), weight(weight) {}
    };

    Vertex *addVertex(V value) {
        auto *vertex = new Vertex(value);
        vertices.push_back(vertex);
        return vertex;
    }

    Edge *addEdge(Vertex *from, Vertex *to, E weight) {
        Edge *edge = new Edge(from, to, weight);
        edges.push_back(edge);

        from->adjacencyList.push_back(edge);
        return edge;
    }

    void removeEdge(Edge *edge) {
        auto it = find(edges.begin(), edges.end(), edge);
        if (it != edges.end()) {
            edges.erase(it);

            auto fromIt = find(edge->from->adjacencyList.begin(),
                               edge->from->adjacencyList.end(), edge);
            if (fromIt != edge->from->adjacencyList.end()) {
                edge->from->adjacencyList.erase(fromIt);
            }

            delete edge;
        }
    }

    void removeVertex(Vertex *vertex, bool assignToNull = false) {
        list<Edge *> adjListCopy = vertex->adjacencyList;

        for (Edge *edge: adjListCopy) {
            auto edgeIt = find(edges.begin(), edges.end(), edge);
            if (edgeIt != edges.end()) {
                edges.erase(edgeIt);
                delete edge;
            }
        }

        for (Vertex *u: vertices) {
            if (u != vertex && u != nullptr) {
                auto it = u->adjacencyList.begin();
                while (it != u->adjacencyList.end()) {
                    if ((*it)->to == vertex) {
                        auto edge = *it;
                        it = u->adjacencyList.erase(it);
                        auto edgeIt = find(edges.begin(), edges.end(), edge);
                        if (edgeIt != edges.end()) {
                            edges.erase(edgeIt);
                            delete edge;
                        }
                    } else {
                        ++it;
                    }
                }
            }
        }

        auto vertexIt = find(vertices.begin(), vertices.end(), vertex);
        if (vertexIt != vertices.end()) {
            if (assignToNull) {
                *vertexIt = nullptr;
            } else {
                vertices.erase(vertexIt);
                delete vertex;
            }
        }
    }

    bool isAdjacent(Vertex *u, Vertex *v) {
        for (Edge *edge: u->adjacencyList) {
            if (edge->to == v) {
                return true;
            }
        }
        return false;
    }

    void EvgeniiKhovalyg_bfs(Vertex *s) {
        for (Vertex *u: vertices) {
            if (u != s) {
                u->color = WHITE;
                u->d = INT_MAX;
                u->pi = nullptr;
            }
        }

        s->color = GRAY;
        s->d = 0;
        s->pi = nullptr;

        Queue<Vertex *> Q;
        Q.push(s);

        while (!Q.empty()) {
            Vertex *u = Q.front();
            Q.pop();

            for (Edge *edge: u->adjacencyList) {
                Vertex *v = edge->to;
                if (v->color == WHITE) {
                    v->color = GRAY;
                    v->d = u->d + 1;
                    v->pi = u;
                    Q.push(v);
                }
            }

            u->color = BLACK;
        }
    }

    bool hasCycle() {
        for (Vertex *u: vertices) {
            u->color = WHITE;
        }

        for (Vertex *u: vertices) {
            if (u->color == WHITE) {
                if (hasCycleDFS_Visit(u)) {
                    return true;
                }
            }
        }
        return false;
    }

    void EvgeniiKhovalyg_dfs_visit(Vertex *u,
                                   list<Vertex *> *topSortList = nullptr,
                                   const bool deleteVisited = false) {
        time = time + 1;
        u->d = time;
        u->color = GRAY;

        list<Edge *> adjListCopy = u->adjacencyList;

        for (Edge *edge: adjListCopy) {
            Vertex *v = edge->to;
            if (v->color == WHITE) {
                v->pi = u;
                EvgeniiKhovalyg_dfs_visit(v, topSortList, deleteVisited);
            }
        }


        u->color = BLACK;
        time = time + 1;
        u->f = time;

        if (topSortList != nullptr) {
            topSortList->push_front(u);
        }
        if (deleteVisited) {
            removeVertex(u, true);
        }
    }

    void EvgeniiKhovalyg_dfs(Vertex *s) {
        for (Vertex *u: vertices) {
            u->color = WHITE;
            u->pi = nullptr;
        }

        time = 0;

        for (Vertex *u: vertices) {
            if (u->color == WHITE) {
                EvgeniiKhovalyg_dfs_visit(u);
            }
        }
    }

    list<Vertex *> EvgeniiKhovalyg_topsort() {
        list<Vertex *> sortedList;

        if (hasCycle()) {
            return sortedList;
        }

        for (Vertex *u: vertices) {
            u->color = WHITE;
            u->pi = nullptr;
        }

        time = 0;

        for (Vertex *u: vertices) {
            if (u->color == WHITE) {
                EvgeniiKhovalyg_dfs_visit(u, &sortedList);
            }
        }

        return sortedList;
    }

    const list<Vertex *> &getVertices() const { return vertices; }

    ~DirectedGraph() {
        for (Edge *edge: edges) {
            delete edge;
        }
        for (Vertex *vertex: vertices) {
            if (vertex != nullptr) {
                delete vertex;
            }
        }
    }
};

int main() {
    DirectedGraph<int, int> graph;

    int n;
    cin >> n;
    int r;
    cin >> r;
    vector<DirectedGraph<int, int>::Vertex *> vertices(n + r, nullptr);
    vector<DirectedGraph<int, int>::Vertex *> roots(r, nullptr);
    for (int i = 0; i < n; i++) {
        int v;
        cin >> v;
        vertices[i] = graph.addVertex(v);
    }
    for (int i = 0; i < r; i++) {
        int v;
        cin >> v;
        roots[i] = graph.addVertex(v);
    }
    for (int i = 0; i < n; i++) {
        if (vertices[i]->value >= 0) {
            graph.addEdge(vertices[i], vertices[vertices[i]->value], 1);
        }
    }
    for (int i = 0; i < r; i++) {
        graph.addEdge(roots[i], vertices[roots[i]->value], 1);
    }
    for (int i = 0; i < r; i++) {
        graph.EvgeniiKhovalyg_dfs_visit(roots[i], nullptr, true);
    }
    copy(graph.getVertices().begin(), graph.getVertices().end(),
         vertices.begin());

    vector<pair<int, int>> result;
    int mark = 0;
    bool flag = true;
    pair<int, int> a{0, 0};
    for (int i = 0; i < n; i++) {
        if (vertices[i] != nullptr) {
            if (flag) {
                a.first = i;
                flag = false;
                mark = i;
            }
        } else {
            if (!flag) {
                a.second = i - mark;
                result.push_back(a);
                flag = true;
            }
        }
    }
    if (!flag) {
        a.second = n - mark;
        result.push_back(a);
    }

    cout << result.size() << endl;
    for (auto& [a,b] : result) {
        cout << a << " " << b << endl;
    }
    vertices.clear();
    roots.clear();
    return 0;
}
