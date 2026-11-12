/*
 *  String.hpp
 *  Sapphire
 *
 *  Created by Wayne De Connick
 *  Project began November 06, 2026.
 *  Project ended December 30, 2026.
 */

#ifndef STRING_HPP
#define STRING_HPP

#include <cstddef>

namespace sapphire {
    class String {
    public:
        String();
        String(const char* ptr);
        String(const String& other);
       ~String();
        
        String& operator=(const String& other);
        char& operator[](std::size_t index);
        const char& operator[](std::size_t index) const;
        
        void assign(const char* ptr);
        void append(const char* ptr);
        void push_back(char c);
        void pop_back();
        char& at(std::size_t index);
        const char& at(std::size_t index) const;
        char& front();
        const char& front() const;
        char& back();
        const char& back() const;
        void clear();
        void copy(const String& other);
        void to_upper_case();
        void to_lower_case();
        bool is_empty() const { return m_length == 0; }
        std::size_t length() const { return m_length; }
        
    private:
        std::size_t m_length;
        std::size_t m_capacity;
        char* m_array;
        
        void resize();
    };

    bool operator==(const String& lhs, const String& rhs);
    bool operator!=(const String& lhs, const String& rhs);
}

#endif // end STRING_HPP
