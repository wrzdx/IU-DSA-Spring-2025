// Evgenii Khovalyg DSAI-02 

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
struct Vertex;

template<typename V, typename E>
struct Edge {
    Vertex<V, E> *from;
    Vertex<V, E> *to;
    E weight;

    Edge(Vertex<V, E> *from, Vertex<V, E> *to, E weight) :
        from(from), to(to), weight(weight) {}
};

template<typename V, typename E>
struct Vertex {
    V value;
    list<Edge<V, E> *> adjacencyList;
    list<Vertex<V, E> *> disjointSet;
    int rank = 0;
    Vertex<V, E> *p = this;

    Vertex(V val) : value(val) {}
};

template<typename V, typename E>
class DirectedGraph {
    list<Edge<V, E> *> edges;
    list<Vertex<V, E> *> vertices;


public:
    DirectedGraph() {}


    Vertex<V, E> *addVertex(V value) {
        auto *vertex = new Vertex<V, E>(value);
        vertices.push_back(vertex);
        return vertex;
    }

    Edge<V, E> *addEdge(Vertex<V, E> *from, Vertex<V, E> *to, E weight) {
        auto *edge = new Edge<V, E>(from, to, weight);
        edges.push_back(edge);

        from->adjacencyList.push_back(edge);
        return edge;
    }

    void removeEdge(Edge<V, E> *edge) {
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

    void removeVertex(Vertex<V, E> *vertex, bool assignToNull = false) {
        list<Edge<V, E> *> adjListCopy = vertex->adjacencyList;

        for (Edge<V, E> *edge: adjListCopy) {
            auto edgeIt = find(edges.begin(), edges.end(), edge);
            if (edgeIt != edges.end()) {
                edges.erase(edgeIt);
                delete edge;
            }
        }

        for (Vertex<V, E> *u: vertices) {
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

    bool isAdjacent(Vertex<V, E> *u, Vertex<V, E> *v) {
        for (Edge<V, E> *edge: u->adjacencyList) {
            if (edge->to == v) {
                return true;
            }
        }
        return false;
    }


    const list<Vertex<V, E> *> &getVertices() const { return vertices; }

    Vertex<V, E> *findSet(Vertex<V, E> *vertex) {
        if (vertex != vertex->p) {
            vertex->p = findSet(vertex->p);
        }
        return vertex->p;
    }

    static void link(Vertex<V, E> *u, Vertex<V, E> *v) {
        if (u->rank > v->rank) {
            v->p = u;
        } else {
            u->p = v;
            if (u->rank == v->rank) {
                ++v->rank;
            }
        }
    }

    void unionSets(Vertex<V, E> *u, Vertex<V, E> *v) {
        link(findSet(u), findSet(v));
    }

    static void makeSet(Vertex<V,E> *v) {
        v->disjointSet.push_back(v);
    }

    bool sameComponent(Vertex<V, E> *u, Vertex<V, E> *v) {
        return findSet(u) == findSet(v);
    }

    void connectedComponents() {
        for (auto v: vertices) {
            makeSet(v);
        }
        for (auto e: edges) {
            if (!sameComponent(e->from, e->to)) {
                unionSets(e->from, e->to);
            }
        }
    }

    static bool isDisjointSet(Vertex<V, E> *v) {
        return v == v->p;
    }

    ~DirectedGraph() {
        for (Edge<V, E> *edge: edges) {
            delete edge;
        }
        for (Vertex<V, E> *vertex: vertices) {
            if (vertex != nullptr) {
                delete vertex;
            }
        }
    }
};





int main() {
    // My disjoint set union is integrated directly into the graph,
    // where each vertex can either be a regular element or a disjoint set.
    DirectedGraph<int, int> graph;
    int n; cin >> n;
    int m; cin >> m;

    vector<Vertex<int, int> *> vertices;
    for (int i = 0; i < n; i++) {
        vertices.push_back(graph.addVertex(1));
    }

    for (int i = 0; i < m; i++) {
        int u, v; cin >> u >> v;
        --u; --v;
        graph.addEdge(vertices[u], vertices[v], 1);
        graph.addEdge(vertices[v], vertices[u], 1);
    }
    graph.connectedComponents();
    int count = 0;
    for (const auto v: vertices) {
        count += graph.isDisjointSet(v);
    }
    cout << count << endl;
}
