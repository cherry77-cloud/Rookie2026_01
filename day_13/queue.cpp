#include <deque>
#include <iostream>
#include <list>
#include <optional>
#include <stdexcept>
#include <sstream>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

template <typename T, typename Container = std::deque<T>>
class MyQueue {
public:
    using container_type = Container;
    using value_type = T;
    using reference = T&;
    using const_reference = const T&;
    using size_type = typename Container::size_type;

private:
    container_type data;

public:
    MyQueue() = default;

    explicit MyQueue(const container_type& cont) : data(cont) {}
    explicit MyQueue(container_type&& cont) : data(std::move(cont)) {}

    reference front() {
        if (empty()) throw std::runtime_error("Queue is empty");
        return data.front();
    }

    const_reference front() const {
        if (empty()) throw std::runtime_error("Queue is empty");
        return data.front();
    }

    reference back() {
        if (empty()) throw std::runtime_error("Queue is empty");
        return data.back();
    }

    const_reference back() const {
        if (empty()) throw std::runtime_error("Queue is empty");
        return data.back();
    }

    [[nodiscard]] bool empty() const noexcept { return data.empty(); }
    [[nodiscard]] size_type size() const noexcept { return data.size(); }

    void push(const value_type& value) { data.push_back(value); }
    void push(value_type&& value) { data.push_back(std::move(value)); }

    template <typename... Args>
    void emplace(Args&&... args) {
        data.emplace_back(std::forward<Args>(args)...);
    }

    void pop() noexcept {
        if (!empty()) data.pop_front();
    }

    void swap(MyQueue& other) noexcept {
        using std::swap;
        swap(data, other.data);
    }

    auto begin() noexcept { return data.begin(); }
    auto end() noexcept { return data.end(); }
    auto begin() const noexcept { return data.begin(); }
    auto end() const noexcept { return data.end(); }
    auto cbegin() const noexcept { return data.cbegin(); }
    auto cend() const noexcept { return data.cend(); }
};

template <typename T, typename Container>
void swap(MyQueue<T, Container>& lhs, MyQueue<T, Container>& rhs) noexcept {
    lhs.swap(rhs);
}
