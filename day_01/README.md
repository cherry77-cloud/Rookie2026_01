## 一. `std::vector` 的扩容过程

当向 `std::vector` 添加元素并且当前容量不足以容纳新元素时，`std::vector` 会进行扩容。扩容过程通常包括以下步骤：
- 分配更大的内存块：通常是当前容量的两倍（具体增长因子取决于实现）。
- 移动元素到新内存：将当前所有元素移动到新分配的内存中。
- 销毁旧元素并释放旧内存：释放旧内存块。
- 插入新元素：将新元素添加到扩容后的内存中。

---

## 二. `std::vector::push_back` 和 `std::vector::emplace_back` 的区别
两者都用于在 `std::vector` 的末尾添加新元素，但它们的实现方式不同：

`push_back`
- 对给定的对象进行拷贝或移动构造。
- 需要先构造对象，再将其拷贝或移动到 `vector` 中。
- 适用于已有对象的情况。

`emplace_back`
- 直接在 `vector` 的末尾构造新元素，无需拷贝或移动操作。
- 通过传递构造参数，直接在容器内部构造对象。
- 更高效，尤其是对于复杂对象。

---

## 三. 什么时候会使用 `std::vector::reserve()`
`std::vector::reserve()` 用于预分配内存，以避免在添加新元素时频繁重新分配内存。

- 当知道将要存储大量元素时。希望避免频繁的内存重新分配和元素拷贝。
- 提高性能，减少内存分配和元素移动的开销。

---

## 四. 如果 `std::vector` 的元素是指针，需要注意什么
当 `std::vector` 存储指针时，需要注意以下问题：

- **内存管理**：需要手动释放指针指向的内存，否则会导致内存泄漏。
- **所有权和生命周期**：确保指针指向的对象在 `vector` 使用期间有效。明确谁拥有对象的所有权。
- **异常安全**：在构造和填充 `vector` 时，确保异常发生时不会泄漏内存。
- **智能指针**：推荐使用 `std::unique_ptr` 或 `std::shared_ptr` 替代原始指针。
- **避免悬垂指针**：确保指针指向的对象未被提前释放。
- **深拷贝与浅拷贝**：如果需要复制 `vector`，需明确是深拷贝（复制指针指向的对象）还是浅拷贝（仅复制指针本身）。
---

## 五. 什么是迭代器失效？如何避免？
当 `vector` 进行插入、删除或扩容操作时，迭代器可能会失效。

- 失效场景：
  - 扩容：所有迭代器都会失效。
  - 中间插入或删除：从操作点到末尾的迭代器会失效。
- 解决方案：
  - 使用标准库提供的算法（如 `std::remove` 和 `std::remove_if`）结合 `vector::erase` 来删除元素。
  - 避免在遍历时直接修改 `vector`。

---

## 六. 常用接口

### 构造与析构
| 方法 | 描述 |
|------|------|
| `vector()` | 默认构造函数，创建一个空的 `vector`。 |
| `vector(size_type count)` | 创建一个包含 `count` 个默认初始化元素的 `vector`。 |
| `vector(size_type count, const T& value)` | 创建一个包含 `count` 个值为 `value` 的元素的 `vector`。 |
| `vector(const vector& other)` | 拷贝构造函数，创建一个与 `other` 相同的 `vector`。 |
| `vector(vector&& other)` | 移动构造函数，将 `other` 的资源移动到新 `vector`。 |
| `~vector()` | 析构函数，释放 `vector` 的所有资源。 |

```c++
std::vector<int> vec1;                // 空 vector
std::vector<int> vec2(5);             // 5 个默认初始化的元素
std::vector<int> vec3(5, 10);         // 5 个值为 10 的元素
std::vector<int> vec4(vec3);          // 拷贝构造
std::vector<int> vec5(std::move(vec4)); // 移动构造
```

---

### 容量操作
| 方法 | 描述 |
|------|------|
| `size()` | 返回 `vector` 中元素的数量。 |
| `capacity()` | 返回 `vector` 当前分配的内存容量。 |
| `empty()` | 检查 `vector` 是否为空。 |
| `reserve(size_type new_cap)` | 预分配至少 `new_cap` 的内存。 |
| `shrink_to_fit()` | 请求移除未使用的容量，减少内存占用。 |

```c++
std::vector<int> vec = {1, 2, 3};
std::cout << vec.size();      // 输出 3
std::cout << vec.capacity(); // 输出 3
vec.reserve(10);             // 预分配 10 个元素的内存
vec.shrink_to_fit();         // 尝试减少容量
```

---

### 元素访问
| 方法 | 描述 |
|------|------|
| `operator[](size_type pos)` | 返回下标为 `pos` 的元素的引用（无边界检查）。 |
| `at(size_type pos)` | 返回下标为 `pos` 的元素的引用（有边界检查，越界抛出异常）。 |
| `front()` | 返回第一个元素的引用。 |
| `back()` | 返回最后一个元素的引用。 |
| `data()` | 返回指向底层数组的指针。 |

```c++
std::vector<int> vec = {1, 2, 3};
std::cout << vec[0];         // 输出 1
std::cout << vec.at(1);      // 输出 2
std::cout << vec.front();    // 输出 1
std::cout << vec.back();     // 输出 3
int* ptr = vec.data();       // 获取底层数组指针
```

---

### 修改操作
| 方法 | 描述 |
|------|------|
| `push_back(const T& value)` | 在末尾添加一个元素。 |
| `emplace_back(Args&&... args)` | 在末尾直接构造一个元素。 |
| `pop_back()` | 移除末尾的元素。 |
| `insert(iterator pos, const T& value)` | 在 `pos` 位置插入一个元素。 |
| `erase(iterator pos)` | 移除 `pos` 位置的元素。 |
| `erase(iterator first, iterator last)` | 移除 `[first, last)` 范围内的元素。 |
| `clear()` | 清空所有元素。 |
| `resize(size_type count)` | 调整 `vector` 的大小为 `count`。 |
| `swap(vector& other)` | 交换两个 `vector` 的内容。 |

```c++
std::vector<int> vec = {1, 2, 3};
vec.push_back(4);                // {1, 2, 3, 4}
vec.emplace_back(5);             // {1, 2, 3, 4, 5}
vec.pop_back();                  // {1, 2, 3, 4}
vec.insert(vec.begin() + 1, 10); // {1, 10, 2, 3, 4}
vec.erase(vec.begin() + 2);      // {1, 10, 3, 4}
vec.clear();                     // {}
vec.resize(3);                   // {0, 0, 0}
```

---

### 迭代器
| 方法 | 描述 |
|------|------|
| `begin()` | 返回指向第一个元素的迭代器。 |
| `end()` | 返回指向末尾（最后一个元素之后）的迭代器。 |
| `cbegin()` | 返回指向第一个元素的常量迭代器。 |
| `cend()` | 返回指向末尾的常量迭代器。 |
| `rbegin()` | 返回指向第一个元素的反向迭代器。 |
| `rend()` | 返回指向末尾的反向迭代器。 |

```c++
std::vector<int> vec = {1, 2, 3};
for (auto it = vec.begin(); it != vec.end(); ++it) {
    std::cout << *it << " "; // 输出 1 2 3
}
for (auto it = vec.rbegin(); it != vec.rend(); ++it) {
    std::cout << *it << " "; // 输出 3 2 1
}
```

---

### 其他操作
| 方法 | 描述 |
|------|------|
| `assign(size_type count, const T& value)` | 用 `count` 个 `value` 替换 `vector` 的内容。 |
| `assign(InputIt first, InputIt last)` | 用 `[first, last)` 范围内的元素替换 `vector` 的内容。 |
| `operator=` | 赋值操作符，支持拷贝赋值和移动赋值。 |
| `get_allocator()` | 返回 `vector` 的内存分配器。 |

```c++
std::vector<int> vec;
vec.assign(3, 10);           // {10, 10, 10}
std::vector<int> vec2 = {1, 2, 3};
vec = vec2;                  // 拷贝赋值
vec = std::move(vec2);       // 移动赋值
```
