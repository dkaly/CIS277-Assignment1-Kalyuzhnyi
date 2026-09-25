# CIS-277 Assignment 1: Network Packet Buffer Pool

## Student
Dimitri Kalyuzhnyi

## Description
Implements a generic stack and a fixed size memory pool 
The pool reserves one contiguous block of memory at startup, divides it into
equal sized blocks, and hands them out on demand. Free blocks are managed with
the project's own Stack<T>, so allocation and deallocation are both O(1).

## Stack Implementation
Stack<T> is a header-only template that stores elements in a std::vector<T>,
treating the back of the vector as the top of the stack.

- push(value) — add to the top
- pop() — remove and return the top
- top() — reference to the top element
- empty() — true if no elements
- size() — number of elements

pop() and top() throw std::out_of_range on an empty stack. Everything
lives in Stack.h because templates must be visible where theyre instantiated

## How to Compile
g++ -std=c++17 -Wall -Wextra -o test main.cpp MemoryPool.cpp

## How to Run
./test

## Analysis Questions
1. Why is a Stack appropriate for managing the free blocks in this memory pool?
    - Last in, first out and O(1) push/pop. No need to search, the most recently freed block is simply handed out next 
2. What happens when the free-block Stack becomes empty?
    - The pool is exhausted so allocate() returns nullptr to signal that no block is available
3. Why must a released block be returned to the Stack?
    -The pool's memory is reserved once at startup and never grows, so a block that isn't pushed back is permanently lost and can never be reused
4. What problem could occur if the same block were deallocated twice?
    -The block gets pushed onto the free stack twice, so two future allocate() calls return the same memory and corrupt each other's data.
5. What is the Big-O time complexity of allocate()? Explain why.
    - O(1) — it only checks emptiness, reads the top, and pops, all constant-time vector operations.

6. What is the Big-O time complexity of deallocate()? Explain why.
    - O(1) amortized — it validates the pointer and pushes it back onto the free stack, which is a constant-time vector push_back