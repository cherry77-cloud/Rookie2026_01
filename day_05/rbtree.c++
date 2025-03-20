#include <iostream>
#include <sstream>
#include <string>

enum class Color { RED, BLACK };

template <typename Key, typename Value>
class RedBlackTree {
    class Node {
    public:
        Key key;
        Value value;
        Color color;
        Node *left;
        Node *right;
        Node *parent;

        Node(const Key &k, const Value &v, Color c, Node *p = nullptr)
            : key(k), value(v), color(c), left(nullptr), right(nullptr), parent(p) {}

        Node() : color(Color::BLACK), left(nullptr), right(nullptr), parent(nullptr) {}
    };

private:
    Node *root;
    size_t size;
    Node *Nil;

    Node *lookUp(Key key) {
        Node *cmpNode = root;
        while (cmpNode) {
            if (key < cmpNode->key) cmpNode = cmpNode->left;
            else if (key > cmpNode->key) cmpNode = cmpNode->right;
            else return cmpNode;
        }
        return nullptr;
    }

    void rightRotate(Node *node) {
        Node *l_son = node->left;
        node->left = l_son->right;
        if (l_son->right) l_son->right->parent = node;
        l_son->parent = node->parent;
        if (!node->parent) root = l_son;
        else if (node == node->parent->left) node->parent->left = l_son;
        else node->parent->right = l_son;
        l_son->right = node;
        node->parent = l_son;
    }

    void leftRotate(Node *node) {
        Node *r_son = node->right;
        node->right = r_son->left;
        if (r_son->left) r_son->left->parent = node;
        r_son->parent = node->parent;
        if (!node->parent) root = r_son;
        else if (node == node->parent->left) node->parent->left = r_son;
        else node->parent->right = r_son;
        r_son->left = node;
        node->parent = r_son;
    }

    void insertFixup(Node *target) {
        while (target->parent && target->parent->color == Color::RED) {
            if (target->parent == target->parent->parent->left) {
                Node *uncle = target->parent->parent->right;
                if (uncle && uncle->color == Color::RED) {
                    target->parent->color = Color::BLACK;
                    uncle->color = Color::BLACK;
                    target->parent->parent->color = Color::RED;
                    target = target->parent->parent;
                } else {
                    if (target == target->parent->right) {
                        target = target->parent;
                        leftRotate(target);
                    }
                    target->parent->color = Color::BLACK;
                    target->parent->parent->color = Color::RED;
                    rightRotate(target->parent->parent);
                }
            } else {
                Node *uncle = target->parent->parent->left;
                if (uncle && uncle->color == Color::RED) {
                    target->parent->color = Color::BLACK;
                    uncle->color = Color::BLACK;
                    target->parent->parent->color = Color::RED;
                    target = target->parent->parent;
                } else {
                    if (target == target->parent->left) {
                        target = target->parent;
                        rightRotate(target);
                    }
                    target->parent->color = Color::BLACK;
                    target->parent->parent->color = Color::RED;
                    leftRotate(target->parent->parent);
                }
            }
        }
        root->color = Color::BLACK;
    }

    void insertNode(const Key &key, const Value &value) {
        Node *newNode = new Node(key, value, Color::RED);
        Node *parent = nullptr;
        Node *cmpNode = root;
        while (cmpNode) {
            parent = cmpNode;
            if (newNode->key < cmpNode->key) cmpNode = cmpNode->left;
            else if (newNode->key > cmpNode->key) cmpNode = cmpNode->right;
            else {
                delete newNode;
                return;
            }
        }
        size++;
        newNode->parent = parent;
        if (!parent) root = newNode;
        else if (newNode->key < parent->key) parent->left = newNode;
        else parent->right = newNode;
        insertFixup(newNode);
    }

    void inorderTraversal(Node *node) const {
        if (node) {
            inorderTraversal(node->left);
            std::cout << node->key << " " << node->value << " ";
            inorderTraversal(node->right);
        }
    }

    void replaceNode(Node *targetNode, Node *newNode) {
        if (!targetNode->parent) root = newNode;
        else if (targetNode == targetNode->parent->left) targetNode->parent->left = newNode;
        else targetNode->parent->right = newNode;
        if (newNode) newNode->parent = targetNode->parent;
    }

    Node *findMinimumNode(Node *node) {
        while (node->left) node = node->left;
        return node;
    }

    void removeFixup(Node *node) {
        if (node == Nil && node->parent == nullptr) return;
        while (node != root) {
            if (node == node->parent->left) {
                Node *sibling = node->parent->right;
                if (getColor(sibling) == Color::RED) {
                    setColor(sibling, Color::BLACK);
                    setColor(node->parent, Color::RED);
                    leftRotate(node->parent);
                    sibling = node->parent->right;
                }
                if (getColor(sibling->left) == Color::BLACK && getColor(sibling->right) == Color::BLACK) {
                    setColor(sibling, Color::RED);
                    node = node->parent;
                    if (node->color == Color::RED) {
                        node->color = Color::BLACK;
                        node = root;
                    }
                } else {
                    if (getColor(sibling->right) == Color::BLACK) {
                        setColor(sibling->left, Color::BLACK);
                        setColor(sibling, Color::RED);
                        rightRotate(sibling);
                        sibling = node->parent->right;
                    }
                    setColor(sibling, getColor(node->parent));
                    setColor(node->parent, Color::BLACK);
                    setColor(sibling->right, Color::BLACK);
                    leftRotate(node->parent);
                    node = root;
                }
            } else {
                Node *sibling = node->parent->left;
                if (getColor(sibling) == Color::RED) {
                    setColor(sibling, Color::BLACK);
                    setColor(node->parent, Color::RED);
                    rightRotate(node->parent);
                    sibling = node->parent->left;
                }
                if (getColor(sibling->right) == Color::BLACK && getColor(sibling->left) == Color::BLACK) {
                    setColor(sibling, Color::RED);
                    node = node->parent;
                    if (node->color == Color::RED) {
                        node->color = Color::BLACK;
                        node = root;
                    }
                } else {
                    if (getColor(sibling->left) == Color::BLACK) {
                        setColor(sibling->right, Color::BLACK);
                        setColor(sibling, Color::RED);
                        leftRotate(sibling);
                        sibling = node->parent->left;
                    }
                    setColor(sibling, getColor(node->parent));
                    setColor(node->parent, Color::BLACK);
                    setColor(sibling->left, Color::BLACK);
                    rightRotate(node->parent);
                    node = root;
                }
            }
        }
        setColor(node, Color::BLACK);
    }

    Color getColor(Node *node) {
        return node == nullptr ? Color::BLACK : node->color;
    }

    void setColor(Node *node, Color color) {
        if (node) node->color = color;
    }

    void dieConnectNil() {
        if (!Nil) return;
        if (Nil->parent) {
            if (Nil == Nil->parent->left) Nil->parent->left = nullptr;
            else Nil->parent->right = nullptr;
        }
    }

    void deleteNode(Node *del) {
        if (!del) return;
        Node *rep = del;
        Node *child = nullptr;
        Node *parentRP;
        Color origCol = rep->color;
        if (!del->left) {
            rep = del->right;
            parentRP = del->parent;
            origCol = getColor(rep);
            replaceNode(del, rep);
        } else if (!del->right) {
            rep = del->left;
            parentRP = del->parent;
            origCol = getColor(rep);
            replaceNode(del, rep);
        } else {
            rep = findMinimumNode(del->right);
            origCol = rep->color;
            if (rep != del->right) {
                parentRP = rep->parent;
                child = rep->right;
                parentRP->left = child;
                if (child) child->parent = parentRP;
                del->left->parent = rep;
                del->right->parent = rep;
                rep->left = del->left;
                rep->right = del->right;
                if (del->parent) {
                    if (del == del->parent->left) {
                        del->parent->left = rep;
                        rep->parent = del->parent;
                    } else {
                        del->parent->right = rep;
                        rep->parent = del->parent;
                    }
                } else {
                    root = rep;
                    root->parent = nullptr;
                }
            } else {
                child = rep->right;
                rep->left = del->left;
                del->left->parent = rep;
                if (del->parent) {
                    if (del == del->parent->left) {
                        del->parent->left = rep;
                        rep->parent = del->parent;
                    } else {
                        del->parent->right = rep;
                        rep->parent = del->parent;
                    }
                } else {
                    root = rep;
                    root->parent = nullptr;
                }
                parentRP = rep;
            }
        }
        if (rep) rep->color = del->color;
        else origCol = del->color;
        if (origCol == Color::BLACK) {
            if (child) removeFixup(child);
            else {
                Nil->parent = parentRP;
                if (parentRP) {
                    if (parentRP->left == nullptr) parentRP->left = Nil;
                    else parentRP->right = Nil;
                }
                removeFixup(Nil);
                dieConnectNil();
            }
        }
        delete del;
    }

public:
    RedBlackTree() : root(nullptr), size(0), Nil(new Node()) {
        Nil->color = Color::BLACK;
    }

    void insert(const Key &key, const Value &value) { insertNode(key, value); }

    void remove(const Key &key) {
        Node *nodeToBeRemoved = lookUp(key);
        if (nodeToBeRemoved) {
            deleteNode(nodeToBeRemoved);
            size--;
        }
    }

    Value *at(const Key &key) {
        auto ans = lookUp(key);
        return ans ? &ans->value : nullptr;
    }

    int getSize() { return size; }
    bool empty() { return size == 0; }

    void print() {
        inorderTraversal(root);
        std::cout << std::endl;
    }

    void clear() {
        deleteTree(root);
        root = nullptr;
        size = 0;
    }

    ~RedBlackTree() {
        deleteTree(root);
        delete Nil;
    }

private:
    void deleteTree(Node *node) {
        if (node) {
            deleteTree(node->left);
            deleteTree(node->right);
            delete node;
        }
    }
};
