#pragma once

#include <array>
#include <initializer_list>


inline constexpr size_t FS_VECTOR_DEFAULT_SIZE = 16;

template <typename T, size_t N = FS_VECTOR_DEFAULT_SIZE>
class fsvector
{
public:
    class iterator;

    fsvector() = default;

    template <typename... Ts>
    fsvector(Ts&&... args) : m_buffer{std::forward<Ts>(args)...}, m_size{sizeof...(Ts)}
    {
    }

    void push(T x)
    {
        if (m_size == N)
            return;
        m_buffer[m_size++] = std::move(x);
    }

    void pop()
    {
        if (m_size == 0)
            return;
        m_size--;
        m_buffer[m_size].~T();
    }

    T& operator[](size_t i) { return m_buffer[i]; }
    const T& operator[](size_t i) const { return m_buffer[i]; }

    iterator begin() { return iterator{m_buffer}; }
    iterator end() { return iterator{m_buffer + m_size}; }

    size_t size() const { return m_size; }

private:
    T m_buffer[N];
    size_t m_size = 0;
};


template <typename T, size_t N>
class fsvector<T, N>::iterator
{
public:
    iterator(T* ptr) : ptr{ptr} {}

    friend bool operator<=>(const iterator& a, const iterator& b) = default;
    iterator& operator++()
    {
        ptr++;
        return *this;
    }
    T& operator*() { return *ptr; }
    const T& operator*() const { return *ptr; }

private:
    T* ptr;
};
