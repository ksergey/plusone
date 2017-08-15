/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_static_string_150817175315
#define KSERGEY_static_string_150817175315

#include <cstring>
#include <plusone/compiler.hpp>
#include <plusone/exception.hpp>

namespace plusone {

template< std::size_t N >
__force_inline constexpr static_string< N >::static_string()
    : storage_{}
    , size_{}
{}

template< std::size_t N >
__force_inline constexpr const char* static_string< N >::data() const noexcept
{
    return storage_;
}

template< std::size_t N >
__force_inline constexpr char* static_string< N >::data() noexcept
{
    return storage_;
}

template< std::size_t N >
__force_inline constexpr const char* static_string< N >::c_str() const noexcept
{
    return storage_;
}

template< std::size_t N >
__force_inline constexpr std::size_t static_string< N >::size() const noexcept
{
    return size_;
}

template< std::size_t N >
__force_inline constexpr std::size_t static_string< N >::length() const noexcept
{
    return size_;
}

template< std::size_t N >
__force_inline constexpr bool static_string< N >::empty() const noexcept
{
    return 0u == size_;
}

template< std::size_t N >
__force_inline constexpr std::size_t static_string< N >::capacity() noexcept
{
    return N;
}

template< std::size_t N >
__force_inline constexpr std::size_t static_string< N >::max_size() noexcept
{
    return N;
}

template< std::size_t N >
__force_inline constexpr const char& static_string< N >::at(std::size_t i) const
{
    return i <= size_
        ? storage_[i]
        : (throw_ex< std::out_of_range >("Out of range in static_string::at"), storage_[i]);
}

template< std::size_t N >
__force_inline constexpr char& static_string< N >::at(std::size_t i)
{
    return i <= size_
        ? storage_[i]
        : (throw_ex< std::out_of_range >("Out of range in static_string::at"), storage_[i]);
}

template< std::size_t N >
__force_inline constexpr const char& static_string< N >::operator[](std::size_t i) const noexcept
{
    return storage_[i];
}

template< std::size_t N >
__force_inline constexpr char& static_string< N >::operator[](std::size_t i) noexcept
{
    return storage_[i];
}

template< std::size_t N >
__force_inline constexpr const char& static_string< N >::front() const noexcept
{
    return (*this)[0u];
}

template< std::size_t N >
__force_inline constexpr char& static_string< N >::front() noexcept
{
    return (*this)[0u];
}

template< std::size_t N >
__force_inline constexpr const char& static_string< N >::back() const noexcept
{
    return storage_[size_ - 1u];
}

template< std::size_t N >
__force_inline constexpr char& static_string< N >::back() noexcept
{
    return storage_[size_ - 1u];
}

template< std::size_t N >
__force_inline void static_string< N >::clear() noexcept
{
    storage_[0u] = '\0';
    size_ = 0u;
}

template< std::size_t N >
__force_inline void static_string< N >::push_back(char ch)
{
    if (__unlikely(N - size_ == 0)) {
        throw_ex< std::out_of_range >("Array index out of bounds in static_string::push_back");
    }
    storage_[size_] = ch;
    storage_[++size_] = '\0';
}

template< std::size_t N >
__force_inline void static_string< N >::pop_back()
{
    if (__unlikely(size_ == 0)) {
        throw_ex< std::out_of_range >("Array index out of bounds in static_string::pop_back");
    }
    storage_[--size_] = '\0';
}

template< std::size_t N >
__force_inline static_string< N >& static_string< N >::append(const char* s, std::size_t count)
{
    if (__unlikely(size_ + count > N)) {
        throw_ex< std::out_of_range >("Array index out of bounds in static_string::append");
    }

    for (std::size_t i = 0u; i < count; ++i) {
        storage_[size_ + i] = s[i];
    }
    size_ += count;
    storage_[size_] = '\0';

    return *this;
}

template< std::size_t N >
__force_inline static_string< N >& static_string< N >::append(const char* s)
{
    return append(s, std::strlen(s));
}

template< std::size_t N >
template< std::size_t M >
__force_inline static_string< N >& static_string< N >::append(const static_string< M >& s)
{
    return append(s.data(), s.size());
}

template< std::size_t N >
__force_inline static_string< N >& static_string< N >::assign(const char* s, std::size_t count)
{
    if (__unlikely(count > N)) {
        throw_ex< std::out_of_range >("Array index out of bounds in static_string::assign");
    }

    for (std::size_t i = 0u; i < count; ++i) {
        storage_[i] = s[i];
    }

    size_ = count;
    storage_[size_] = '\0';

    return *this;
}

} /* namespace plusone */

#endif /* KSERGEY_static_string_150817175315 */
