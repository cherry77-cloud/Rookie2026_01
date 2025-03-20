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
    // 构造函数
    Vector() : elements(nullptr), capacity(0), size(0) {}

    // 析构函数
    ~Vector() {
        clear();
        ::operator delete(elements);
    }

    // 拷贝构造函数
    Vector(const Vector& other) : capacity(other.capacity), size(other.size) {
        elements = static_cast<T*>(::operator new(capacity * sizeof(T)));
        for (size_t i = 0; i < size; ++i) {
            new (elements + i) T(other.elements[i]);
        }
    }

    // 移动构造函数
    Vector(Vector&& other) noexcept
        : elements(other.elements), capacity(other.capacity), size(other.size) {
        other.elements = nullptr;
        other.capacity = 0;
        other.size = 0;
    }

    // 拷贝赋值操作符
    Vector& operator=(const Vector& other) {
        if (this != &other) {
            Vector temp(other);
            swap(*this, temp);
        }
        return *this;
    }

    // 移动赋值操作符
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

    // 交换函数
    friend void swap(Vector& a, Vector& b) noexcept {
        using std::swap;
        swap(a.elements, b.elements);
        swap(a.capacity, b.capacity);
        swap(a.size, b.size);
    }

    // 添加元素到数组末尾
    void push_back(const T& value) {
        if (size >= capacity) {
            reserve(capacity == 0 ? 1 : capacity * 2);
        }
        new (elements + size) T(value);
        ++size;
    }

    // 获取数组中元素的个数
    size_t getSize() const {
        return size;
    }

    // 获取数组的容量
    size_t getCapacity() const {
        return capacity;
    }

    // 访问数组中的元素
    T& operator[](size_t index) {
        if (index >= size) {
            throw std::out_of_range("Index out of range");
        }
        return elements[index];
    }

    // const版本的访问数组中的元素
    const T& operator[](size_t index) const {
        if (index >= size) {
            throw std::out_of_range("Index out of range");
        }
        return elements[index];
    }

    // 在指定位置插入元素
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

    // 删除数组末尾的元素
    void pop_back() {
        if (size > 0) {
            --size;
            elements[size].~T();
        }
    }

    // 清空数组
    void clear() {
        for (size_t i = 0; i < size; ++i) {
            elements[i].~T();
        }
        size = 0;
    }

    // 使用迭代器遍历数组的开始位置
    T* begin() {
        return elements;
    }

    // 使用迭代器遍历数组的结束位置
    T* end() {
        return elements + size;
    }

    // 使用迭代器遍历数组的开始位置(const版本)
    const T* begin() const {
        return elements;
    }

    // 使用迭代器遍历数组的结束位置(const版本)
    const T* end() const {
        return elements + size;
    }

    // 打印数组中的元素
    void printElements() const {
        for (size_t i = 0; i < size; i++) {
            std::cout << elements[i] << " ";
        }
        std::cout << std::endl;
    }

private:
    // 扩展数组容量
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
