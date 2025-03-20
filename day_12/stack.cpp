#include <iostream>
#include <deque>
#include <stdexcept>
#include <sstream>
#include <string>
#include <vector>
#include <utility>

template <typename T, typename Container = std::deque<T>>
class MyStack {
public:
    using value_type      = T;
    using container_type  = Container;
    using reference       = T&;
    using const_reference = const T&;
    using size_type       = typename Container::size_type;

private:
    Container data;

public:
    MyStack() = default;

    explicit MyStack(const Container& cont) : data(cont) {}
    explicit MyStack(Container&& cont) : data(std::move(cont)) {}

    reference top() {
        if (empty()) {
            throw std::runtime_error("Stack is empty.");
        }
        return data.back();
    }

    const_reference top() const {
        if (empty()) {
            throw std::runtime_error("Stack is empty.");
        }
        return data.back();
    }

    bool empty() const noexcept {
        return data.empty();
    }

    size_type size() const noexcept {
        return data.size();
    }

    void push(const T& value) {
        data.push_back(value);
    }

    void push(T&& value) {
        data.push_back(std::move(value));
    }

    template <typename... Args>
    void emplace(Args&&... args) {
        data.emplace_back(std::forward<Args>(args)...);
    }

    void pop() {
        if (empty()) {
            throw std::runtime_error("Stack is empty.");
        }
        data.pop_back();
    }

    void swap(MyStack& other) noexcept {
        std::swap(data, other.data);
    }

    bool operator==(const MyStack& other) const {
        return data == other.data;
    }

    bool operator!=(const MyStack& other) const {
        return !(*this == other);
    }
};

template <typename T, typename Container>
void swap(MyStack<T, Container>& lhs, MyStack<T, Container>& rhs) noexcept {
    lhs.swap(rhs);
}
