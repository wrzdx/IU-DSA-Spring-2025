// Evgenii Khovalyg DSAI-02

#include <climits>
#include <iostream>
#include <list>
#include <vector>
#include <algorithm>

using namespace std;

enum Color { White, Gray, Black };

template<typename V, typename E>
struct Vertex;

template<typename V, typename E>
struct Edge {
    Vertex<V, E>* from;
    Vertex<V, E>* to;
    E weight;

    Edge(Vertex<V, E>* f, Vertex<V, E>* t, E w) : from(f), to(t), weight(w) {}
};

template<typename V, typename E>
struct Vertex {
    V value;
    list<Edge<V, E>*> adjacencyList;
    Vertex<V, E>* p = nullptr;
    double key = 0;
    Color color = White;
    int rank = 0;

    Vertex(V val) : value(val) {}
};

template<typename V, typename E>
class DirectedGraph {
    list<Vertex<V, E>*> vertices;
    list<Edge<V, E>*> edges;

public:
    Vertex<V, E>* addVertex(V value) {
        Vertex<V, E>* v = new Vertex<V, E>(value);
        vertices.push_back(v);
        return v;
    }

    Edge<V, E>* addEdge(Vertex<V, E>* from, Vertex<V, E>* to, E weight) {
        Edge<V, E>* e = new Edge<V, E>(from, to, weight);
        from->adjacencyList.push_back(e);
        edges.push_back(e);
        return e;
    }

    const list<Vertex<V, E>*>& getVertices() const {
        return vertices;
    }

    void relax(Vertex<V, E>* u, Vertex<V, E>* v, E weight) {
        if (v->key > u->key + weight) {
            v->key = u->key + weight;
            v->p = u;
        }
    }

    vector<Vertex<V, E> *> EvgeniiKhovalyg_bellman_ford(Vertex<V, E>* u) {
        vector<Vertex<V, E> *> cycle;
        for (auto v : vertices) {
            v->key = INT_MAX;
            v->p = nullptr;
        }
        u->key = 0;

        int n = vertices.size();
        for (int i = 1; i < n; i++) {
            for (auto edge : edges) {
                auto from = edge->from;
                auto to = edge->to;
                if (from->key != INT_MAX && to->key > from->key + edge->weight) {
                    to->key = from->key + edge->weight;
                    to->p = from;
                }
            }
        }

        Vertex<V, E>* cycle_start = nullptr;

        for (auto edge : edges) {
            auto from = edge->from;
            auto to = edge->to;
            if (from->key != INT_MAX && to->key > from->key + edge->weight) {
                to->p = from;
                cycle_start = to;
                break;
            }
        }

        if (!cycle_start)
            return cycle;

        for (int i = 0; i < n; ++i)
            cycle_start = cycle_start->p;

        Vertex<V, E>* v = cycle_start;
        do {
            cycle.push_back(v);
            v = v->p;
        } while (v != cycle_start);
        cycle.push_back(cycle_start);

        reverse(cycle.begin(), cycle.end());
        return cycle;
    }

    ~DirectedGraph() {
        for (auto e : edges) delete e;
        for (auto v : vertices) delete v;
    }
};



int main() {
    DirectedGraph<int, int> graph;
    int n; cin >> n;

    vector<Vertex<int,int>*> v;

    for (int i = 0; i < n; i++) {
        v.push_back(graph.addVertex(i+1));
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int weight; cin >> weight;
            if (weight == 0) { continue;}
            graph.addEdge(v[i], v[j], weight);
        }
    }
    auto cycle = graph.EvgeniiKhovalyg_bellman_ford(v[0]);
    if (cycle.empty()) {
        cout << "NO" << endl;
    } else {
        cout << "YES" << endl;
        cout << cycle.size()-1 << endl;
        auto it = cycle.begin();
        cout << (*it)->value;
        ++it;
        for (; it!= prev(cycle.end()); ++it) {
            cout << " " << (*it)->value;
        }
        cout << endl;
    }
    return 0; 
}
