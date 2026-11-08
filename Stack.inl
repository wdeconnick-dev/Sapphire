/*
 *  Stack.inl
 *  Sapphire
 *
 *  Created by Wayne De Connick
 *  Project began November 06, 2026.
 *  Project ended December 30, 2026.
 */

#include <cstdlib>
#include <stdexcept>

namespace sapphire {
    template <typename T>
    Stack<T>::Stack() : m_top{nullptr}, m_frame_counter{0} {}

    template <typename T>
    Stack<T>::Stack(const Stack& other) : m_top{nullptr}, m_frame_counter{0} {
        deep_copy(other);
    }

    template <typename T>
    Stack<T>::~Stack() {
        clear();
    }

    template <typename T>
    Stack<T>& Stack<T>::operator=(const Stack& other) {
        if (this == &other) {
            return *this;
        }
        deep_copy(other);
        return *this;
    }

    template <typename T>
    void Stack<T>::clear() {
        while (m_top != nullptr) {
            pop();
        }
    }

    template <typename T>
    void Stack<T>::deep_copy(const Stack& other) {
        if (other.m_top == nullptr) {
            return;
        }
        clear();
        frame_t* frame{other.m_top};
        for (count_t i{0}; i < other.m_frame_counter; ++i) {
            push(frame->data);
            frame = frame->next;
        }
        reverse();
    }

    template <typename T>
    T& Stack<T>::peek() {
        if (m_top == nullptr) {
            throw std::runtime_error("Code 500: Stack is empty!");
        }
        return m_top->data;
    }

    template <typename T>
    const T& Stack<T>::peek() const {
        if (m_top == nullptr) {
            throw std::runtime_error("Code 501: Stack is empty!");
        }
        return m_top->data;
    }

    template <typename T>
    void Stack<T>::pop() {
        if (m_top == nullptr) {
            throw std::runtime_error("Code 502: Stack is empty!");
        }
        frame_t* frame{m_top};
        m_top = frame->next;
        frame->next = nullptr;
        delete frame;
        --m_frame_counter;
    }

    template <typename T>
    void Stack<T>::push(const T& record) {
        frame_t* frame = new frame_t;
        frame->data = record;
        frame->next = m_top;
        m_top = frame;
        ++m_frame_counter;
    }

    template <typename T>
    void Stack<T>::reverse() {
        if (m_top == nullptr) {
            return;
        }
        frame_t* previous{nullptr};
        frame_t* current{m_top};
        frame_t* following;
        while (current->next != nullptr) {
            following = current->next;
            current->next = previous;
            previous = current;
            current = following;
        }
        current->next = previous;
        m_top = current;
    }
}
