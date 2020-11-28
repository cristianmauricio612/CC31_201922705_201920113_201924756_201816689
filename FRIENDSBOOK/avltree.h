#ifndef AVLTREE_H
#define AVLTREE_H

#include <functional>

using std::function;
using std::max;

template <typename T, typename R=T, T NONE=nullptr>
class AvlTree {
    struct Node {
        T element; Node* left; Node* right; int height;
        Node(T element): element(element),left(nullptr),right(nullptr),height(0) {}
    };

    typedef function<R(T)>      lbdKey;
    typedef function<void(T)>   lbdProc;

    Node*   root;
    int     len;
    lbdKey  key;
    int count;

public:
    AvlTree(lbdKey key=[](T a) {return a;}) : key(key), root(nullptr), len(0), count(0) {}
    ~AvlTree() { destroy(root); }

    int height() { return height(root); }
    int size() { return len; }
    void clear() { destroy(root); len = 0; }

    void add(T elem) { add(root, elem); }
    void postOrder(lbdProc proc) { postOrder(root, proc); }
    void postOrderTodo(lbdProc proc) { postOrderTodo(root, proc); }
    void inOrder(lbdProc proc) { inOrder(root, proc); }
    void clearI() {
        count = 0;
    }

private:
    void destroy(Node*& node) {
        if (node != nullptr) {
            destroy(node->left);
            destroy(node->right);
            delete node;
            node = nullptr;
        }
    }
    int height(Node* node) {
        return node == nullptr? -1: node->height;
    }
    void add(Node*& node, T elem) {
        if (node == nullptr) {
            node = new Node(elem);
            ++len;
        } else {
            if (key(elem) < key(node->element)) {
                add(node->left, elem);
            } else {
                add(node->right, elem);
            }
            balance(node); /* :O */
        }
    }
    void postOrder(Node* node, lbdProc proc) {
        if (node == nullptr) return;
            postOrder(node->right, proc);
        if (count < 50) {
            count++;
            proc(node->element);
            postOrder(node->left, proc);
        }
    }
    void postOrderTodo(Node* node, lbdProc proc) {
        if (node == nullptr) return;
        postOrderTodo(node->right, proc);
        proc(node->element);
        postOrderTodo(node->left, proc);
    }
    void inOrder(Node* node, lbdProc proc) {
        if (node == nullptr) return;
            inOrder(node->left, proc);
        if (count < 50) {
            count++;
            proc(node->element);
            inOrder(node->right, proc);
        }
    }
    void removeAux(Node*& node) {
        if(node->left == nullptr) {
            auto aux = node;
            node = node->right;
            delete aux;
        } else {
            //node->element = max(node->left);
        }
    }
    void remove(Node*& node, R val) {
        if(node!= nullptr) {
            if(val == key(node->element)) {
                removeAux(node);
            } else if (val < key(node->element)) {
                remove(node->left, val);
            } else
                remove(node->right, val);
            balance(node);
        }
    }
    Node*& find(Node*& node, R attr) {
        if (node == nullptr) {
            return dummynull;
        } else if (attr == key(node->element)) {
            return node;
        } else if (attr < key(node->element)) {
            return find(node->left, attr);
        } else {
            return find(node->right, attr);
        }
    }

    void updateHeight(Node* node) {
        if (node != nullptr) {
            node->height = max(height(node->left), height(node->right)) + 1;
        }
    }
    void rotateLeft(Node*& node) {
        Node* aux = node->right;
        node->right = aux->left;
        updateHeight(node);
        aux->left = node;
        updateHeight(aux);
        node = aux;
    }
    void rotateRight(Node*& node) {
        Node* aux = node->left;
        node->left = aux->right;
        updateHeight(node);
        aux->right = node;
        updateHeight(aux);
        node = aux;
    }
    void balance(Node*& node) {
        int hl = height(node->left);
        int hr = height(node->right);

        if (hr - hl < -1) {
            hl = height(node->left->left);
            hr = height(node->left->right);
            if (hr > hl) {
                rotateLeft(node->left);
            }
            rotateRight(node);
        } else if (hr - hl > 1) {
            hl = height(node->right->left);
            hr = height(node->right->right);
            if (hl > hr) {
                rotateRight(node->right);
            }
            rotateLeft(node);
        } else {
            updateHeight(node);
        }
    }

    Node* dummynull = nullptr;
};

#endif // AVLTREE_H
