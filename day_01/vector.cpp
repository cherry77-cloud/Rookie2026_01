#include <iostream>
#include <algorithm>
#include <sstream>
#include <string>
#include <stdexcept>
#include <utility>
 
template <typename T>
class Vector {
private:
    T* elements;        // 指向动态数组的指针
    size_t capacity;    // 数组的容量
    size_t size;        // 数组中元素的个数
 
public:
    Vector() : elements(nullptr), capacity(0), size(0) {}

    ~Vector() {
        clear();
        ::operator delete(elements);
    }
 
    Vector(const Vector& other) : capacity(other.capacity), size(other.size) {
        elements = static_cast<T*>(::operator new(capacity * sizeof(T)));
        for (size_t i = 0; i < size; ++i) {
            new (elements + i) T(other.elements[i]);
        }
    }
 
    Vector(Vector&& other) noexcept
        : elements(other.elements), capacity(other.capacity), size(other.size) {
        other.elements = nullptr;
        other.capacity = 0;
        other.size = 0;
    }
 
    Vector& operator=(const Vector& other) {
        if (this != &other) {
            Vector temp(other);
            swap(*this, temp);
        }
        return *this;
    }
 
    Vector& operator=(Vector&& other) noexcept {
        if (this != &other) {
            clear();
            ::operator delete(elements);
            elements = other.elements;
            capacity = other.capacity;
            size = other.size;
            other.elements = nullptr;
            other.capacity = 0;
            other.size = 0;
        }
        return *this;
    }
 
    friend void swap(Vector& a, Vector& b) noexcept {
        using std::swap;
        swap(a.elements, b.elements);
        swap(a.capacity, b.capacity);
        swap(a.size, b.size);
    }
 
    void push_back(const T& value) {
        if (size >= capacity) {
            reserve(capacity == 0 ? 1 : capacity * 2);
        }
        new (elements + size) T(value);
        ++size;
    }
 
    T& operator[](size_t index) {
        if (index >= size) {
            throw std::out_of_range("Index out of range");
        }
        return elements[index];
    }
 
    const T& operator[](size_t index) const {
        if (index >= size) {
            throw std::out_of_range("Index out of range");
        }
        return elements[index];
    }
 
    void insert(size_t index, const T& value) {
        if (index > size) {
            throw std::out_of_range("Index out of range");
        }
        if (size >= capacity) {
            reserve(capacity == 0 ? 1 : capacity * 2);
        }
        if (index < size) {
            new (elements + size) T(std::move(elements[size - 1]));
            for (size_t i = size - 1; i > index; --i) {
                elements[i] = std::move(elements[i - 1]);
            }
            elements[index] = value;
        } else {
            new (elements + size) T(value);
        }
        ++size;
    }
 
    void pop_back() {
        if (size > 0) {
            --size;
            elements[size].~T();
        }
    }
 
    void clear() {
        for (size_t i = 0; i < size; ++i) {
            elements[i].~T();
        }
        size = 0;
    }

    size_t getSize() const { return size; }
    size_t getCapacity() const { return capacity; }
    T* begin() { return elements; }
    T* end() { return elements + size; }
    const T* begin() const { return elements; }
    const T* end() const { return elements + size; }
 
private:
    void reserve(size_t new_capacity) {
        if (new_capacity <= capacity) return;
        T* new_elements = static_cast<T*>(::operator new(new_capacity * sizeof(T)));
        for (size_t i = 0; i < size; ++i) {
            new (new_elements + i) T(std::move(elements[i]));
            elements[i].~T();
        }
        ::operator delete(elements);
        elements = new_elements;
        capacity = new_capacity;
    }
};
