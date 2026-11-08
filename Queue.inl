/*
 *  Queue.inl
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
    Queue<T>::Queue() : m_head{nullptr}, m_tail{nullptr}, m_frame_counter{0} {}

    template <typename T>
    Queue<T>::Queue(const Queue& other) : m_head{nullptr}, m_tail{nullptr}, m_frame_counter{0} {
        deep_copy(other);
    }

    template <typename T>
    Queue<T>::~Queue() {
        clear();
    }

    template <typename T>
    Queue<T>& Queue<T>::operator=(const Queue& other) {
        if (this == &other) {
            return *this;
        }
        deep_copy(other);
        return *this;
    }

    template <typename T>
    void Queue<T>::clear() {
        while (m_head != nullptr) {
            de_queue();
        }
    }

    template <typename T>
    void Queue<T>::de_queue() {
        if (m_head == nullptr) {
            throw std::runtime_error("Code 602: Queue is empty!");
        }
        frame_t* frame{m_head};
        if (m_head != m_tail) {
            m_head = frame->backward_ptr;
            frame->backward_ptr = nullptr;
            m_head->forward_ptr = nullptr;
        } else {
            m_head = nullptr;
            m_tail = nullptr;
        }
        delete frame;
        --m_frame_counter;
    }

    template <typename T>
    void Queue<T>::deep_copy(const Queue& other) {
        clear();
        if (other.m_head == nullptr) {
            return;
        }
        frame_t* frame{other.m_head};
        for (count_t i{0}; i < other.m_frame_counter; ++i) {
            en_queue(frame->data);
            frame = frame->backward_ptr;
        }
    }

    template <typename T>
    void Queue<T>::en_queue(const T& record) {
        frame_t* frame = new frame_t;
        frame->data = record;
        if (m_head == nullptr) {
            m_head = frame;
            frame->forward_ptr = nullptr;
        } else {
            frame->forward_ptr = m_tail;
            m_tail->backward_ptr = frame;
        }
        m_tail = frame;
        frame->backward_ptr = nullptr;
        ++m_frame_counter;
    }

    template <typename T>
    T& Queue<T>::peek() {
        if (m_head == nullptr) {
            throw std::runtime_error("Code 600: Queue is empty!");
        }
        return m_head->data;
    }

    template <typename T>
    const T& Queue<T>::peek() const {
        if (m_head == nullptr) {
            throw std::runtime_error("Code 601: Queue is empty!");
        }
        return m_head->data;
    }
}
