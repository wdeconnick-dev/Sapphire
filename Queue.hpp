/*
 *  Queue.hpp
 *  Sapphire
 *
 *  Created by Wayne De Connick
 *  Project began November 06, 2026.
 *  Project ended December 30, 2026.
 */

#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <cstddef>

namespace sapphire {
    template <typename T>
    class Queue {
    public:
        using count_t = std::size_t;
        
        Queue();
        Queue(const Queue& other);
       ~Queue();
        
        Queue& operator=(const Queue& other);
        
        void clear();
        void de_queue();
        void deep_copy(const Queue& other);
        void en_queue(const T&);
        count_t get_frame_count() const { return m_frame_counter; }
        bool is_empty() const { return m_head == nullptr; }
        T& peek();
        const T& peek() const;
        
    private:
        struct QueueFrame {
            T data;
            QueueFrame* forward_ptr;
            QueueFrame* backward_ptr;
        };
        
        using frame_t = QueueFrame;
        
        frame_t* m_head;
        frame_t* m_tail;
        count_t m_frame_counter;
    };
}

#include "Queue.inl"

#endif // end QUEUE_HPP
