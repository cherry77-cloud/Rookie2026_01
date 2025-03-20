#include <algorithm>
#include <functional>
#include <iostream>
#include <vector>
#include <deque>
#include <stdexcept>
#include <sstream>
#include <string>
#include <type_traits>
#include <memory>

template<typename T, typename Container = std::vector<T>, typename Compare = std::less<typename Container::value_type>>
class MyPriorityQueue {
public:
    using container_type = Container;
    using value_compare = Compare;
    using value_type = typename Container::value_type;
    using reference = typename Container::reference;
    using const_reference = typename Container::const_reference;
    using size_type = typename Container::size_type;

private:
    container_type data;
    value_compare comp;

    void heapify_up(size_type index) {
        while (index > 0) {
            size_type parent = (index - 1) / 2;
            if (!comp(data[index], data[parent])) {
                std::swap(data[index], data[parent]);
                index = parent;
            } else {
                break;
            }
        }
    }

    void heapify_down(size_type index) {
        const size_type size = data.size();
        while (true) {
            size_type largest = index;
            const size_type left = 2 * index + 1;
            const size_type right = 2 * index + 2;

            if (left < size && !comp(data[left], data[largest])) {
                largest = left;
            }
            if (right < size && !comp(data[right], data[largest])) {
                largest = right;
            }

            if (largest != index) {
                std::swap(data[index], data[largest]);
                index = largest;
            } else {
                break;
            }
        }
    }

    void make_heap() {
        if (data.empty()) return;
        for (size_type i = (data.size() / 2); i-- > 0; ) {
            heapify_down(i);
        }
        heapify_down(0);
    }

public:
    MyPriorityQueue() = default;

    explicit MyPriorityQueue(const Compare& compare) : comp(compare) {}

    explicit MyPriorityQueue(const Container& cont, const Compare& compare = Compare())
        : data(cont), comp(compare) {
        make_heap();
    }

    explicit MyPriorityQueue(Container&& cont, const Compare& compare = Compare())
        : data(std::move(cont)), comp(compare) {
        make_heap();
    }

    template<typename InputIt>
    MyPriorityQueue(InputIt first, InputIt last, const Compare& compare = Compare())
        : data(first, last), comp(compare) {
        make_heap();
    }

    MyPriorityQueue(const MyPriorityQueue&) = default;
    MyPriorityQueue(MyPriorityQueue&&) noexcept = default;
    MyPriorityQueue& operator=(const MyPriorityQueue&) = default;
    MyPriorityQueue& operator=(MyPriorityQueue&&) noexcept = default;

    void push(const value_type& value) {
        data.push_back(value);
        heapify_up(data.size() - 1);
    }

    void push(value_type&& value) {
        data.push_back(std::move(value));
        heapify_up(data.size() - 1);
    }

    template<typename... Args>
    void emplace(Args&&... args) {
        data.emplace_back(std::forward<Args>(args)...);
        heapify_up(data.size() - 1);
    }

    void pop() {
        if (empty()) {
            throw std::runtime_error("Priority queue is empty");
        }
        std::swap(data.front(), data.back());
        data.pop_back();
        if (!empty()) {
            heapify_down(0);
        }
    }

    [[nodiscard]] const_reference top() const {
        if (empty()) {
            throw std::runtime_error("Priority queue is empty");
        }
        return data.front();
    }

    [[nodiscard]] bool empty() const noexcept { return data.empty(); }
    [[nodiscard]] size_type size() const noexcept { return data.size(); }

    void swap(MyPriorityQueue& other) noexcept(
        noexcept(std::swap(data, other.data)) && 
        noexcept(std::swap(comp, other.comp))) {
        using std::swap;
        swap(data, other.data);
        swap(comp, other.comp);
    }
};

template<typename T, typename Container, typename Compare>
void swap(
    MyPriorityQueue<T, Container, Compare>& lhs,
    MyPriorityQueue<T, Container, Compare>& rhs
) noexcept(noexcept(lhs.swap(rhs))) {
    lhs.swap(rhs);
}
