// Evgenii Khovalyg DSAI-02

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

    void EvgeniiKhovalyg_dfs_visit(Vertex *u,
                                   list<Vertex *> *topSortList = nullptr) {
        time = time + 1;
        u->d = time;
        u->color = GRAY;

        for (Edge *edge: u->adjacencyList) {
            Vertex *v = edge->to;
            if (v->color == WHITE) {
                v->pi = u;
                EvgeniiKhovalyg_dfs_visit(v, topSortList);
            }
        }

        u->color = BLACK;
        time = time + 1;
        u->f = time;
        if (topSortList != nullptr) {
            topSortList->push_front(u);
        }
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

    void removeVertex(Vertex *vertex) {
        list<Edge *> adjListCopy = vertex->adjacencyList;

        for (Edge *edge: adjListCopy) {
            auto edgeIt = find(edges.begin(), edges.end(), edge);
            if (edgeIt != edges.end()) {
                edges.erase(edgeIt);
                delete edge;
            }
        }

        for (Vertex *u: vertices) {
            if (u != vertex) {
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
            vertices.erase(vertexIt);
            delete vertex;
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

    ~DirectedGraph() {
        for (Vertex *vertex: vertices) {
            vertex->adjacencyList.clear();
        }

        for (Edge *edge: edges) {
            delete edge;
        }

        for (Vertex *vertex: vertices) {
            delete vertex;
        }
    }
};

int main() {
    DirectedGraph<char, int> graph;
    int n;
    cin >> n;


    vector<string> words(n, string(100, ' '));
    vector<DirectedGraph<char, int>::Vertex *> vertices(26, nullptr);

    for (int i = 0; i < n; i++) {
        string word;
        cin >> word;
        for (int j = 0; j < word.length(); j++) {
            words[i][j] = word[j];
        }
        for (char c: word) {
            if (vertices[c - 'a'] == nullptr) {
                vertices[c - 'a'] = graph.addVertex(c);
            }
        }
    }

    bool has_contradiction = false;
    for (int i = 1; i < n; i++) {
        for (int j = 0; j < 100; j++) {
            if (words[i - 1][j] == ' ' && words[i][j] != ' ') {
                break;
            }
            if (words[i][j] == ' ' && words[i - 1][j] != ' ') {
                has_contradiction = true;
                break;
            }
            if (words[i - 1][j] != words[i][j]) {
                if (words[i - 1][j] != ' ') {
                    auto from = vertices[words[i - 1][j] - 'a'];
                    auto to = vertices[words[i][j] - 'a'];
                    graph.addEdge(from, to, 1);
                }
                break;
            }
        }
        if (has_contradiction)
            break;
    }

    if (has_contradiction) {
        cout << "Doh" << endl;
    } else {
        auto topSortList = graph.EvgeniiKhovalyg_topsort();
        if (topSortList.empty()) {
            cout << "Doh" << endl;
        } else {
            for (auto vertex: topSortList) {
                cout << vertex->value;
            }
            cout << endl;
        }
    }
    return 0;
}

