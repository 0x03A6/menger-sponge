#pragma once

#ifndef STACK_HPP
#define STACK_HPP

template<typename T, int max_size>
struct Stack {
    T arr[max_size + 3];    //为了速度，下标 0 不存东西。好像大小是 max_size + 1 就可以，但是我懒得处理潜在的溢出错误了。
    T *cursor;
    Stack() {
        cursor = arr;
    }
    void push(const T x) {
        *(++cursor) = x;
    }
    void pop() {
        cursor--;
    }
    T &top() {
        return *cursor;
    }
    T *topPtr() {
        return cursor;
    }
    int size() {
        return cursor - arr;
    }
    bool empty() {
        return cursor == arr;
    }
    void clear() {
        cursor = arr;
    }
    void operator=(Stack<T, max_size> &stack) {
        cursor = arr + stack.size();
        memcpy(arr, stack.arr, (stack.size() + 2) * sizeof(T));   //这里好像 +1 就可以了，但是还是避免一下意外的溢出。
    }
};

#endif