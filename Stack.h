#pragma once

#include <cstddef>   //size_t
#include <stdexcept> //std::out_of_range
#include <vector>

template <typename T>
class Stack {
public:
    void push(const T& value);
    T pop();
    T& top();
    bool empty() const;
    size_t size() const;

private:
    std::vector<T> data;
};

//definitions 
template <typename T>
void Stack<T>::push(const T& value) {
    data.push_back(value);
}

template <typename T>
T Stack<T>::pop() {
    if (data.empty()) {
        throw std::out_of_range("Stack::pop() on empty stack");
    }
    T value = std::move(data.back());
    data.pop_back();
    return value;
}

template <typename T>
T& Stack<T>::top() {
    if (data.empty()) {
        throw std::out_of_range("Stack::top() on empty stack");
    }
    return data.back();
}

template <typename T>
bool Stack<T>::empty() const {
    return data.empty();
}

template <typename T>
size_t Stack<T>::size() const {
    return data.size();
}