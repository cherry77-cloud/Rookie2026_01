#include <iostream>
#include <stdexcept>
#include <sstream>
#include <string>
#include <new>
#include <utility>

template <typename T>
class Deque {
private:
    T* elements;        // 存储元素的原始内存指针
    size_t capacity;    // 总容量
    size_t frontIndex;  // 队列头索引（第一个元素位置）
    size_t backIndex;   // 队列尾索引（最后一个元素的下一个位置）
    size_t size_;       // 当前元素数量

public:
    Deque() : elements(nullptr), capacity(0), frontIndex(0), backIndex(0), size_(0) {}

    ~Deque() {
        clear();
        ::operator delete(elements);
    }

    Deque(const Deque&) = delete;
    Deque& operator=(const Deque&) = delete;

    Deque(Deque&& other) noexcept
        : elements(other.elements),
          capacity(other.capacity),
          frontIndex(other.frontIndex),
          backIndex(other.backIndex),
          size_(other.size_) {
        other.elements = nullptr;
        other.capacity = 0;
        other.size_ = 0;
    }

    Deque& operator=(Deque&& other) noexcept {
        if (this != &other) {
            clear();
            ::operator delete(elements);
            elements = other.elements;
            capacity = other.capacity;
            frontIndex = other.frontIndex;
            backIndex = other.backIndex;
            size_ = other.size_;
            other.elements = nullptr;
            other.capacity = 0;
            other.size_ = 0;
        }
        return *this;
    }

    template <typename U>
    void push_front(U&& value) {
        ensure_capacity();
        frontIndex = (frontIndex - 1 + capacity) % capacity;
        new (&elements[frontIndex]) T(std::forward<U>(value));
        ++size_;
    }

    template <typename U>
    void push_back(U&& value) {
        ensure_capacity();
        new (&elements[backIndex]) T(std::forward<U>(value));
        backIndex = (backIndex + 1) % capacity;
        ++size_;
    }

    void pop_front() {
        if (empty()) throw std::out_of_range("Deque is empty");
        elements[frontIndex].~T();
        frontIndex = (frontIndex + 1) % capacity;
        --size_;
    }

    void pop_back() {
        if (empty()) throw std::out_of_range("Deque is empty");
        backIndex = (backIndex - 1 + capacity) % capacity;
        elements[backIndex].~T();
        --size_;
    }

    T& operator[](int index) {
        return const_cast<T&>(static_cast<const Deque*>(this)->operator[](index));
    }

    const T& operator[](int index) const {
        if (index < 0) index += size_;
        if (index < 0 || index >= static_cast<int>(size_)) {
            throw std::out_of_range("Index out of range");
        }
        return elements[(frontIndex + index) % capacity];
    }

    size_t size() const noexcept { return size_; }
    bool empty() const noexcept { return size_ == 0; }

    void clear() noexcept {
        while (!empty()) {
            pop_back();
        }
        frontIndex = backIndex = 0;
    }

private:
    void ensure_capacity() {
        if (size_ < capacity) return;）
        const size_t new_cap = (capacity == 0) ? 1 : capacity * 2;
        T* new_elements = static_cast<T*>(::operator new(new_cap * sizeof(T)));
        size_t new_front = 0;
        try {
            for (size_t i = 0; i < size_; ++i) {
                const size_t pos = (frontIndex + i) % capacity;
                new (&new_elements[i]) T(std::move(elements[pos]));
                elements[pos].~T();
            }
        } catch (...) {
            for (size_t j = 0; j < new_front; ++j) {
                new_elements[j].~T();
            }
            ::operator delete(new_elements);
            throw;
        }
        ::operator delete(elements);
        elements = new_elements;
        capacity = new_cap;
        frontIndex = 0;
        backIndex = size_;
    }
};
