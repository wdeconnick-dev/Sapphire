/*
 *  String.cpp
 *  Sapphire
 *
 *  Created by Wayne De Connick
 *  Project began November 06, 2026.
 *  Project ended December 30, 2026.
 */

#include "String.hpp"
#include "parameters.hpp"
#include <stdexcept>

namespace sapphire {
    String::String(): m_length{0}, m_capacity{sapphire::limits::array_max} {
        m_array = new char[m_capacity];
        m_array[0] = '\0';
    }

    String::String(const char* ptr): m_length{0}, m_capacity{sapphire::limits::array_max} {
        m_array = new char[m_capacity];
        // Since m_length is already guaranteed to be 0, calling assign()
        // rather than append() is a form of "fail-safe insurance"
        assign(ptr);
    }

    String::String(const String& other) : m_length{0}, m_capacity{sapphire::limits::array_max} {
        m_array = new char[m_capacity];
        copy(other);
    }

    String::~String() {
        //clear(); // Call clear() when handling cryptographic keys or passwords
        delete[] m_array;
        m_array = nullptr; // Not ncessary but included for sake of completeness
    }

    String& String::operator=(const String& other) {
        if (this == &other) {
            return *this;
        }
        copy(other);
        return *this;
    }

    char& String::operator[](std::size_t index) {
        return m_array[index];
    }

    const char& String::operator[](std::size_t index) const {
        return m_array[index];
    }

    void String::assign(const char* ptr) {
        if (ptr == nullptr) {
            return;
        }
        m_length = 0;
        append(ptr);
    }

    char& String::at(std::size_t index) {
        if (index >= m_length) {
            throw std::out_of_range("Code 300: String index is out of bounds!");
        }
        return m_array[index];
    }

    const char& String::at(std::size_t index) const {
        if (index >= m_length) {
            throw std::out_of_range("Code 301: String index is out of bounds!");
        }
        return m_array[index];
    }

    void String::append(const char* ptr) {
        if (ptr == nullptr) {
            return;
        }
        while (*ptr != '\0') {
            if (m_length + 1 >= m_capacity) {
                resize();
            }
            m_array[m_length] = *ptr;
            ++m_length;
            ++ptr;
        }
        // The check below is included as "fail-safe insurance" in case
        // the >= in the conditional above is changed to a > which will
        // result in an off-by-one error
        if (m_length + 1 >= m_capacity) {
            resize();
        }
        m_array[m_length] = '\0';
    }

    void String::clear() {
        if (m_array != nullptr) {
            m_array[0] = '\0';
        }
        m_length = 0;
    }

    void String::copy(const String& other) {
        assign(other.m_array);
    }

    void String::resize() {
        char* arr{new char[m_capacity * 2]};
        for (std::size_t i{0}; i < m_capacity; ++i) {
            arr[i] = m_array[i];
        }
        delete[] m_array;
        m_array = arr;
        m_capacity = m_capacity * 2;
    }
}
