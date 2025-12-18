// Evgenii Khovalyg DSAI-02

#include <climits>
#include <iostream>
#include <list>
#include <queue>
#include <vector>

using namespace std;

enum Color { White, Gray, Black };

template<typename V, typename E>
struct Vertex;

template<typename V, typename E>
struct Edge {
    Vertex<V, E> *from;
    Vertex<V, E> *to;
    E weight;

    Edge(Vertex<V, E> *f, Vertex<V, E> *t, E w) : from(f), to(t), weight(w) {}
};

template<typename V, typename E>
struct Vertex {
    V value;
    list<Edge<V, E> *> adjacencyList;
    Vertex<V, E> *p = nullptr;
    long long key = LLONG_MAX;   
    long long bandwidth = 0;     
    Color color = White;
    int rank = 0;

    Vertex(V val) : value(val) {}
};

template<typename V, typename E>
class Graph {
    list<Vertex<V, E> *> vertices;
    list<Edge<V, E> *> edges;

public:
    Vertex<V, E> *addVertex(V value) {
        Vertex<V, E> *v = new Vertex<V, E>(value);
        vertices.push_back(v);
        return v;
    }

    Edge<V, E> *addEdge(Vertex<V, E> *from, Vertex<V, E> *to, E weight) {
        Edge<V, E> *e = new Edge<V, E>(from, to, weight);
        from->adjacencyList.push_back(e);
        edges.push_back(e);
        return e;
    }

    const list<Vertex<V, E> *> &getVertices() const { return vertices; }

    void EvgeniiKhovalyg_dijkstra(Vertex<V, E> *start) {
        for (auto v : vertices) {
            v->key = LLONG_MAX;
            v->bandwidth = 0;
            v->p = nullptr;
        }

        start->key = 0;
        start->bandwidth = LLONG_MAX;

        auto comp = [](Vertex<V, E> *a, Vertex<V, E> *b) {
            if (a->key != b->key) return a->key > b->key;
            return a->bandwidth < b->bandwidth;
        };

        priority_queue<Vertex<V, E> *, vector<Vertex<V, E> *>, decltype(comp)> q(comp);
        q.push(start);

        while (!q.empty()) {
            Vertex<V, E> *u = q.top();
            q.pop();

            for (auto edge : u->adjacencyList) {
                Vertex<V, E> *to = edge->to;
                long long new_dist = u->key + edge->weight.distance;
                long long new_bw = min(u->bandwidth, static_cast<long long>(edge->weight.bandwidth));

                if (new_dist < to->key || (new_dist == to->key && new_bw > to->bandwidth)) {
                    to->key = new_dist;
                    to->bandwidth = new_bw;
                    to->p = u;
                    q.push(to);
                }
            }
        }
    }

    ~Graph() {
        for (auto e : edges)
            delete e;
        for (auto v : vertices)
            delete v;
    }
};

struct Tube {
    int distance;
    int bandwidth;
    Tube(int distance, int bandwidth) :
        distance(distance), bandwidth(bandwidth) {}
};

int main() {
    Graph<int, Tube> graph;
    int n, m;
    cin >> n >> m;
    vector<Vertex<int, Tube> *> vertices;
    for (int i = 0; i <= n; i++) {
        vertices.push_back(graph.addVertex(i));
    }
    for (int i = 0; i < m; i++) {
        int u, v, d, b;
        cin >> u >> v >> d >> b;
        graph.addEdge(vertices[u], vertices[v], Tube(d, b));
        graph.addEdge(vertices[v], vertices[u], Tube(d, b)); 
    }
    int s, t;
    cin >> s >> t;
    graph.EvgeniiKhovalyg_dijkstra(vertices[s]);
    cout << vertices[t]->key << " " << vertices[t]->bandwidth << endl;
    return 0;
}