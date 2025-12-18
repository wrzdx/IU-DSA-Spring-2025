// Evgenii Khovalyg DSAI-02

#include <iostream>
#include <string>

enum Color { RED, BLACK };

template<typename K, typename V>
struct Node {
    K key;
    V value;
    Color color;
    Node<K, V> *left, *right, *parent;
    Node(K k, V v) :
        key(k), value(v), color(RED), left(nullptr), right(nullptr),
        parent(nullptr) {}
};

template<typename K, typename V>
class RedBlackTree {
private:
    Node<K, V> *root;
    int rotations;
    int blackCount;
    int redCount;

    Node<K, V> *search(K key) {
        Node<K, V> *node = root;
        while (node && node->key != key) {
            if (key < node->key)
                node = node->left;
            else
                node = node->right;
        }
        return node;
    }

    void leftRotate(Node<K, V> *x) {
        Node<K, V> *y = x->right;
        x->right = y->left;
        if (y->left)
            y->left->parent = x;
        y->parent = x->parent;
        if (!x->parent)
            root = y;
        else if (x == x->parent->left)
            x->parent->left = y;
        else
            x->parent->right = y;
        y->left = x;
        x->parent = y;
        rotations++;
    }

    void rightRotate(Node<K, V> *y) {
        Node<K, V> *x = y->left;
        y->left = x->right;
        if (x->right)
            x->right->parent = y;
        x->parent = y->parent;
        if (!y->parent)
            root = x;
        else if (y == y->parent->right)
            y->parent->right = x;
        else
            y->parent->left = x;
        x->right = y;
        y->parent = x;
        rotations++;
    }

    void insertFixup(Node<K, V> *z) {
        while (z->parent && z->parent->color == RED) {
            if (z->parent == z->parent->parent->left) {
                Node<K, V> *y = z->parent->parent->right;
                if (y && y->color == RED) {
                    z->parent->color = BLACK;
                    y->color = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;
                } else {
                    if (z == z->parent->right) {
                        z = z->parent;
                        leftRotate(z);
                    }
                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    rightRotate(z->parent->parent);
                }
            } else {
                Node<K, V> *y = z->parent->parent->left;
                if (y && y->color == RED) {
                    z->parent->color = BLACK;
                    y->color = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;
                } else {
                    if (z == z->parent->left) {
                        z = z->parent;
                        rightRotate(z);
                    }
                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    leftRotate(z->parent->parent);
                }
            }
        }
        root->color = BLACK;
    }

    void transplant(Node<K, V> *u, Node<K, V> *v) {
        if (!u->parent)
            root = v;
        else if (u == u->parent->left)
            u->parent->left = v;
        else
            u->parent->right = v;
        if (v)
            v->parent = u->parent;
    }

    Node<K, V> *minimum(Node<K, V> *node) {
        while (node->left)
            node = node->left;
        return node;
    }

    void deleteFixup(Node<K, V> *x, Node<K, V> *xParent) {
        while (x != root && (!x || x->color == BLACK)) {
            if (x == xParent->left) {
                Node<K, V> *w = xParent->right;
                if (w && w->color == RED) {
                    w->color = BLACK;
                    xParent->color = RED;
                    leftRotate(xParent);
                    w = xParent->right;
                }
                if ((!w->left || w->left->color == BLACK) &&
                    (!w->right || w->right->color == BLACK)) {
                    w->color = RED;
                    x = xParent;
                    xParent = x->parent;
                } else {
                    if (!w->right || w->right->color == BLACK) {
                        if (w->left)
                            w->left->color = BLACK;
                        w->color = RED;
                        rightRotate(w);
                        w = xParent->right;
                    }
                    w->color = xParent->color;
                    xParent->color = BLACK;
                    if (w->right)
                        w->right->color = BLACK;
                    leftRotate(xParent);
                    x = root;
                }
            } else {
                Node<K, V> *w = xParent->left;
                if (w && w->color == RED) {
                    w->color = BLACK;
                    xParent->color = RED;
                    rightRotate(xParent);
                    w = xParent->left;
                }
                if ((!w->right || w->right->color == BLACK) &&
                    (!w->left || w->left->color == BLACK)) {
                    w->color = RED;
                    x = xParent;
                    xParent = x->parent;
                } else {
                    if (!w->left || w->left->color == BLACK) {
                        if (w->right)
                            w->right->color = BLACK;
                        w->color = RED;
                        leftRotate(w);
                        w = xParent->left;
                    }
                    w->color = xParent->color;
                    xParent->color = BLACK;
                    if (w->left)
                        w->left->color = BLACK;
                    rightRotate(xParent);
                    x = root;
                }
            }
        }
        if (x)
            x->color = BLACK;
    }

    void deleteNode(Node<K, V> *z) {
        Node<K, V> *y = z;
        Node<K, V> *x;
        Node<K, V> *xParent = z->parent;
        Color yColor = y->color;
        if (!z->left) {
            x = z->right;
            transplant(z, z->right);
            if (!z->parent)
                xParent = nullptr;
            else
                xParent = z->parent;
        } else if (!z->right) {
            x = z->left;
            transplant(z, z->left);
            if (!z->parent)
                xParent = nullptr;
            else
                xParent = z->parent;
        } else {
            y = minimum(z->right);
            yColor = y->color;
            x = y->right;
            if (y->parent == z) {
                xParent = y;
            } else {
                xParent = y->parent;
                transplant(y, y->right);
                y->right = z->right;
                y->right->parent = y;
            }
            transplant(z, y);
            y->left = z->left;
            y->left->parent = y;
            y->color = z->color;
        }
        delete z;
        if (yColor == BLACK)
            deleteFixup(x, xParent);
    }

    void countKeys(Node<K, V> *node) {
        if (!node)
            return;
        if (node->color == BLACK)
            blackCount++;
        else
            redCount++;
        countKeys(node->left);
        countKeys(node->right);
    }

public:
    RedBlackTree() : root(nullptr), rotations(0), blackCount(0), redCount(0) {}

    void add(K key, V value) {
        Node<K, V> *z = new Node<K, V>(key, value);
        Node<K, V> *y = nullptr;
        Node<K, V> *x = root;
        while (x) {
            y = x;
            if (z->key == x->key) {
                std::cout << "KEY ALREADY EXISTS\n";
                delete z;
                return;
            }
            x = (z->key < x->key) ? x->left : x->right;
        }
        z->parent = y;
        if (!y)
            root = z;
        else if (z->key < y->key)
            y->left = z;
        else
            y->right = z;
        insertFixup(z);
        blackCount = redCount = 0;
        countKeys(root);
    }

    void lookup(K key) {
        Node<K, V> *node = search(key);
        if (!node)
            std::cout << "KEY NOT FOUND\n";
        else
            std::cout << node->value << "\n";
    }

    void remove(K key) {
        Node<K, V> *node = search(key);
        if (!node) {
            std::cout << "KEY NOT FOUND\n";
            return;
        }
        deleteNode(node);
        blackCount = redCount = 0;
        countKeys(root);
    }

    void printRotations() { std::cout << rotations << "\n"; }
    void printBlackCount() { std::cout << blackCount << "\n"; }
    void printRedCount() { std::cout << redCount << "\n"; }
};

int main() {
    int N;
    std::cin >> N;
    RedBlackTree<int, int> rbt;
    for (int i = 0; i < N; i++) {
        std::string op;
        std::cin >> op;
        if (op == "ADD") {
            int k, v;
            std::cin >> k >> v;
            rbt.add(k, v);
        } else if (op == "LOOKUP") {
            int k;
            std::cin >> k;
            rbt.lookup(k);
        } else if (op == "DELETE") {
            int k;
            std::cin >> k;
            rbt.remove(k);
        } else if (op == "PRINT_ROTATIONS") {
            rbt.printRotations();
        } else if (op == "PRINT_COUNT_BLACK_KEYS") {
            rbt.printBlackCount();
        } else if (op == "PRINT_COUNT_RED_KEYS") {
            rbt.printRedCount();
        }
    }
    return 0;
}
