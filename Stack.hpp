/*
 *  Stack.hpp
 *  Sapphire
 *
 *  Created by Wayne De Connick
 *  Project began November 06, 2026.
 *  Project ended December 30, 2026.
 */

#ifndef STACK_HPP
#define STACK_HPP

#include <cstddef>

namespace sapphire {
    template <typename T>
    class Stack {
    public:
        using count_t = std::size_t;
        
        Stack();
        Stack(const Stack& other);
       ~Stack();
        
        Stack& operator=(const Stack& other);
        
        void clear();
        void deep_copy(const Stack& other);
        count_t get_frame_count() const { return m_frame_counter; }
        bool is_empty() const { return m_top == nullptr; }
        T& peek();
        const T& peek() const;
        void pop();
        void push(const T& record);
        void reverse();
        
    private:
        struct StackFrame {
            T data;
            StackFrame* next;
        };
        
        using frame_t = StackFrame;
        frame_t* m_top;
        count_t m_frame_counter;
    };
}

#include "Stack.inl"

#endif // end STACK_HPP
