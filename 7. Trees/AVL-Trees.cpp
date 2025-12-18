// Evgenii Khovalyg DSAI-02

#include <iostream>
#include <string>

using namespace std;

int numberOfRotations = 0;

template <typename K, typename V>
class Node {
    K key;
    V value;
    int height = 0;
    Node *left = nullptr;
    Node *right = nullptr;
    Node *parent = nullptr;

    static void swap(Node &a, Node &b) {
        const K a_key = a.key;
        const V a_value = a.value;
        a.key = b.key;
        a.value = b.value;
        b.key = a_key;
        b.value = a_value;
    }

    static void updateHeight(Node *node) {
        node->height = max(getHeight(node->left), getHeight(node->right)) + 1;
    }

    static Node *searchSuccessor(const Node *node) {
        if (node == nullptr) {
            return nullptr;
        }

        if (node->right != nullptr) {
            return getMin(node->right);
        }

        Node *lowestGrandParent = node->parent;
        while (lowestGrandParent != nullptr &&
               node == lowestGrandParent->right) {
            node = lowestGrandParent;
            lowestGrandParent = lowestGrandParent->parent;
        }

        return lowestGrandParent;
    }

    static void rightRotate(Node *node) {
        swap(*node, *node->left);
        Node *temp = node->right;
        node->right = node->left;
        node->left = node->left->left;
        node->right->left = node->right->right;
        node->right->right = temp;
        updateHeight(node->right);
        updateHeight(node);
        numberOfRotations++;
    }

    static void leftRotate(Node *node) {
        swap(*node, *node->right);
        Node *temp = node->left;
        node->left = node->right;
        node->right = node->right->right;
        node->left->right = node->left->left;
        node->left->left = temp;
        updateHeight(node->left);
        updateHeight(node);
        numberOfRotations++;
    }

    static void balance(Node *node) {
        int balance = getBalance(node);
        if (balance == -2) {
            if (getBalance(node->left) == 1) {
                leftRotate(node->left);
            }
            rightRotate(node);
        } else if (balance == 2) {
            if (getBalance(node->right) == -1) {
                rightRotate(node->right);
            }
            leftRotate(node);
        }
    }

public:
    Node(const K k, const V v) : key(k), value(v) {}

    ~Node() {
        delete left;
        delete right;
    }

    static void insert(Node *node, const K key, const V value) {
        if (key < node->key) {
            if (node->left == nullptr) {
                node->left = new Node(key, value);
                node->left->parent = node;
            } else {
                insert(node->left, key, value);
            }
        } else {
            if (node->right == nullptr) {
                node->right = new Node(key, value);
                node->right->parent = node;
            } else {
                insert(node->right, key, value);
            }
        }
        updateHeight(node);
        balance(node);
    }

    static Node *search(Node *node, const K key) {
        if (node == nullptr) {
            return nullptr;
        }
        if (key < node->key) {
            return search(node->left, key);
        }
        if (key > node->key) {
            return search(node->right, key);
        }
        return node;
    }

    static Node *getMin(Node *node) {
        while (node != nullptr) {
            if (node->left == nullptr) {
                return node;
            }
            node = node->left;
        }
        return nullptr;
    }

    static Node *getMax(Node *node) {
        while (node != nullptr) {
            if (node->right == nullptr) {
                return node;
            }
            node = node->right;
        }
        return nullptr;
    }

    static Node *deleteNode(Node *node, const K key) {
        if (node == nullptr) {
            return nullptr;
        }
        if (key < node->key) {
            if (node->left != nullptr) {
                node->left = deleteNode(node->left, key);
            }
        } else if (key > node->key) {
            if (node->right != nullptr) {
                node->right = deleteNode(node->right, key);
            }
        } else {
            if (node->left != nullptr && node->right != nullptr) {
                Node *successor = searchSuccessor(node);
                node->key = successor->key;
                node->value = successor->value;
                if (successor->key <= node->right->key) {
                    node->right = deleteNode(node->right, successor->key);
                } else {
                    successor->parent = deleteNode(successor->parent, key);
                }
            } else {
                Node *temp = (node->left == nullptr) ? node->right : node->left;
                if (temp == nullptr) {
                    temp = node;
                    node = nullptr;
                } else {
                    *node = *temp;
                }
                delete temp;
            }
        }

        if (node != nullptr) {
            updateHeight(node);
            balance(node);
        }
        return node;
    }
    static void printTree(const Node *node) {
        if (node->left != nullptr) {
            printTree(node->left);
        }
        cout << node->key << " " << getBalance(node) << endl;
        if (node->right != nullptr) {
            printTree(node->right);
        }
    }

    V getValue() const { return value; }
    K getKey() const { return key; }
    static int getHeight(const Node *node) {
        if (node == nullptr) {
            return -1;
        }
        return node->height;
    }
    static int getBalance(const Node *node) {
        if (node == nullptr) {
            return 0;
        }
        return getHeight(node->right) - getHeight(node->left);
    }
};


int main() {
    int n;
    cin >> n;
    string op;
    int key;
    int value;
    Node<int, int> *root = nullptr;
    while (n--) {
        cin >> op;
        if (op == "ADD") {
            cin >> key >> value;
            if (root == nullptr) {
                root = new Node(key, value);
            } else {
                if (Node<int, int>::search(root, key) == nullptr) {
                    Node<int, int>::insert(root, key, value);
                } else {
                    cout << "KEY ALREADY EXISTS\n";
                }
            }
        } else if (op == "LOOKUP") {
            cin >> key;
            if (root != nullptr) {
                Node<int, int> *node = Node<int, int>::search(root, key);
                if (node == nullptr) {
                    cout << "KEY NOT FOUND\n";
                } else {
                    cout << node->getValue() << "\n";
                }
            } else {
                cout << "KEY NOT FOUND\n";
            }
        } else if (op == "DELETE") {
            cin >> key;
            if (root != nullptr) {
                if (Node<int, int>::search(root, key) != nullptr) {
                    root = Node<int, int>::deleteNode(root, key);
                } else {
                    cout << "KEY NOT FOUND\n";
                }
            } else {
                cout << "KEY NOT FOUND\n";
            }
        } else if (op == "PRINT_ROTATIONS") {
            if (root != nullptr) {
                cout << numberOfRotations << "\n";
            } else {
                cout << 0 << "\n";
            }
        }
    }
    delete root;
}
