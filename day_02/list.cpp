#include <iostream>
#include <memory>
#include <stdexcept>
#include <sstream>
#include <string>

template<typename T>
class List {
public:
    template <typename L>
    friend std::ostream &operator<<(std::ostream &os, const List<L> &list);

private:
    struct Node {
        T data;
        std::unique_ptr<Node> next;
        Node *prev;
        Node(const T &value, std::unique_ptr<Node> nextNode, Node *prevNode)
            : data(value), next(std::move(nextNode)), prev(prevNode) {}
    };

    std::unique_ptr<Node> head;
    Node *tail;
    size_t size;

public:
    List() : head(nullptr), tail(nullptr), size(0) {}

    ~List() {
        clear();
    }

    void push_back(const T &value) {
        auto newNode = std::make_unique<Node>(value, nullptr, tail);
        if (tail) {
            tail->next = std::move(newNode);
            tail = tail->next.get();
        } else {
            head = std::move(newNode);
            tail = head.get();
        }
        ++size;
    }

    void push_front(const T &value) {
        auto newNode = std::make_unique<Node>(value, std::move(head), nullptr);
        if (newNode->next) {
            newNode->next->prev = newNode.get();
        } else {
            tail = newNode.get();
        }
        head = std::move(newNode);
        ++size;
    }

    size_t getSize() const {
        return size;
    }

    T &operator[](size_t index) {
        Node *current = head.get();
        for (size_t i = 0; i < index; ++i) {
            if (!current) throw std::out_of_range("Index out of range");
            current = current->next.get();
        }
        if (!current) throw std::out_of_range("Index out of range");
        return current->data;
    }

    const T &operator[](size_t index) const {
        Node *current = head.get();
        for (size_t i = 0; i < index; ++i) {
            if (!current) throw std::out_of_range("Index out of range");
            current = current->next.get();
        }
        if (!current) throw std::out_of_range("Index out of range");
        return current->data;
    }

    void pop_back() {
        if (size > 0) {
            if (tail->prev) {
                tail = tail->prev;
                tail->next.reset();
            } else {
                head.reset();
                tail = nullptr;
            }
            --size;
        }
    }

    void pop_front() {
        if (size > 0) {
            if (head->next) {
                head = std::move(head->next);
                head->prev = nullptr;
            } else {
                head.reset();
                tail = nullptr;
            }
            --size;
        }
    }

    Node *getNode(const T &val) {
        Node *node = head.get();
        while (node != nullptr && node->data != val) {
            node = node->next.get();
        }
        return node;
    }

    T *find(const T &val) {
        Node *node = getNode(val);
        return node ? &node->data : nullptr;
    }

    void remove(const T &val) {
        Node *node = getNode(val);
        if (!node) return;

        if (node->prev) {
            node->prev->next = std::move(node->next);
        } else {
            head = std::move(node->next);
        }

        if (node->next) {
            node->next->prev = node->prev;
        } else {
            tail = node->prev;
        }

        --size;
    }

    bool empty() const {
        return size == 0;
    }

    void clear() {
        while (head) {
            head = std::move(head->next);
        }
        tail = nullptr;
        size = 0;
    }

    void printElements() const {
        for (Node *current = head.get(); current; current = current->next.get()) {
            std::cout << current->data << " ";
        }
        std::cout << std::endl;
    }
};

template <typename T>
std::ostream &operator<<(std::ostream &os, const List<T> &list) {
    for (typename List<T>::Node *current = list.head.get(); current; current = current->next.get()) {
        os << current->data << " ";
    }
    return os;
}
