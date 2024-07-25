#include "../exercise.h"

// READ: 移动构造函数 <https://zh.cppreference.com/w/cpp/language/move_constructor>
// READ: 移动赋值 <https://zh.cppreference.com/w/cpp/language/move_assignment>
// READ: 运算符重载 <https://zh.cppreference.com/w/cpp/language/operators>

class DynFibonacci {
    size_t *cache;
    int cached;

public:
    // TODO: 实现动态设置容量的构造器
    DynFibonacci(int capacity): cache(new size_t[capacity]()), cached(0) {
        cache[0] = 0; // 初始化第一个斐波那契数
        if (capacity > 1) {
            cache[1] = 1;  // 初始化第二个斐波那契数
            cached = 2;  // 第一个计算的斐波那契数
        }
    }

    // TODO: 实现移动构造器
    DynFibonacci(DynFibonacci&& other) noexcept :cache(other.cache), cached(other.cached){
        other.cache = nullptr;
        other.cached = 0;
    }

    // TODO: 实现移动赋值
    // NOTICE: ⚠ 注意移动到自身问题 ⚠
    DynFibonacci &operator=(DynFibonacci &&other) noexcept {
        // 要排除移除到自身的情况  this != &other
        if (this != &other) {
            delete[] cache;  // 释放当前对象的资源
            // 接管other对象的资源
            cache = other.cache;
            cached = other.cached;
            // 释放other的资源
            other.cache = nullptr;
            other.cached = 0;
        }
        return *this;  // 返回接管other资源的当前对象
    }

    // TODO: 实现析构器，释放缓存空间
    ~DynFibonacci() {
        delete[] cache;
    }

    // TODO: 实现正确的缓存优化斐波那契计算
    size_t operator[](int i) {
        // 来个特判  数下标不合法
        if (i < 0) {
            ASSERT(false, "Negative index is not allowed");
            return 0;  
        }
        for (; cached<=i; ++cached) {
            cache[cached] = cache[cached - 1] + cache[cached - 2];
        }
        return cache[i];
    }

    // 报错，缺少一个[] const重载函数
    size_t operator[](int i) const {
        return cache[i];
    }

    // NOTICE: 不要修改这个方法
    bool is_alive() const {
        return cache;
    }
};

int main(int argc, char **argv) {
    DynFibonacci fib(12);
    ASSERT(fib[10] == 55, "fibonacci(10) should be 55");

    DynFibonacci const fib_ = std::move(fib);
    ASSERT(!fib.is_alive(), "Object moved");
    ASSERT(fib_[10] == 55, "fibonacci(10) should be 55");

    DynFibonacci fib0(6);
    DynFibonacci fib1(12);

    fib0 = std::move(fib1);
    fib0 = std::move(fib0);
    ASSERT(fib0[10] == 55, "fibonacci(10) should be 55");

    return 0;
}
