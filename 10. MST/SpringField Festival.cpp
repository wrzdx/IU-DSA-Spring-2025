// Evgenii Khovalyg DSAI-02

#include <iostream>
#include <list>
#include <queue>
#include <vector>
#include <climits>

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

    void EvgeniiKhovalyg_mst_prim() {
        for (auto v : vertices) {
            v->key = INT_MAX;
            v->color = White;
            v->p = nullptr;
        }

        auto comp = [](Edge<V, E>* a, Edge<V, E>* b) {
            return a->weight > b->weight;
        };

        for (auto start : vertices) {
            if (start->color != Black) {
                priority_queue<Edge<V, E>*, vector<Edge<V, E>*>, decltype(comp)> q(comp);
                for (auto edge : start->adjacencyList) {
                    if (edge->to->color == White) {
                        q.push(edge);
                    }
                }
                start->color = Black;

                while (!q.empty()) {
                    Edge<V, E>* u = q.top();
                    q.pop();
                    if (u->to->color == Black) {
                        continue;
                    }

                    u->to->color = Black;
                    cout << u->from->value.name << ":" << u->to->value.name << " ";



                    for (auto edge : u->to->adjacencyList) {
                        if (edge->to->color == White) {
                            q.push(edge);
                        }
                    }
                }
            }
        }
        cout << endl;
    }

    ~DirectedGraph() {
        for (auto e : edges) delete e;
        for (auto v : vertices) delete v;
    }
};

struct Stall {
    string name;
    int tax;
};

double countWeight(Stall a, Stall b, int distance) {
    return static_cast<double>(distance) / (a.tax + b.tax);
}

Vertex<Stall, double>* findVertex(const string& name, const vector<Vertex<Stall, double>*>& vertices) {
    for (auto v : vertices) {
        if (v->value.name == name)
            return v;
    }
    return nullptr;
}

int main() {
    DirectedGraph<Stall, double> graph;
    vector<Vertex<Stall, double>*> vertices;
    int n;
    cin >> n;

    for (int i = 0; i < n; ++i) {
        string command;
        cin >> command;

        if (command == "ADD") {
            Stall s;
            cin >> s.name >> s.tax;
            vertices.push_back(graph.addVertex(s));
        }
        else if (command == "CONNECT") {
            string name1, name2;
            int dist;
            cin >> name1 >> name2 >> dist;
            auto v1 = findVertex(name1, vertices);
            auto v2 = findVertex(name2, vertices);
            double w = countWeight(v1->value, v2->value, dist);
            graph.addEdge(v1, v2, w);
            graph.addEdge(v2, v1, w);
        }
        else if (command == "PRINT_MIN") {
            graph.EvgeniiKhovalyg_mst_prim();
        }
    }

    return 0;
}
