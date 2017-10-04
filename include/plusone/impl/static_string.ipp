/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_static_string_150817175315
#define KSERGEY_static_string_150817175315

#include <cstring>
#include <plusone/compiler.hpp>
#include <plusone/exception.hpp>
#include <plusone/expect.hpp>

namespace plusone {
namespace detail {

__force_inline bool equal(const char* left, std::size_t left_size,
        const char* right, std::size_t right_size) noexcept
{
    return left_size == right_size ? std::strncmp(left, right, left_size) == 0 : false;
}

} /* namespace detail */

template< std::size_t N >
__force_inline static_string< N >::static_string()
    : storage_{}
    , size_{}
{}

template< std::size_t N >
template< std::size_t M >
__force_inline static_string< N >::static_string(const static_string< M >& s)
{
    assign(s.data(), s.size());
}

template< std::size_t N >
__force_inline static_string< N >::static_string(const char* s, std::size_t count)
{
    assign(s, count);
}

template< std::size_t N >
__force_inline static_string< N >::static_string(const char* s)
    : static_string{s, std::strlen(s)}
{}

template< std::size_t N >
template< std::size_t M >
__force_inline static_string< N >::static_string(const char (&s)[M]) noexcept
{
    __expect( M - 1 <= N );
    __expect( s[M - 1] == '\0' );
    assign(s, M - 1);
}

template< std::size_t N >
__force_inline static_string< N >::static_string(const std::string& s)
    : static_string{s.data(), s.size()}
{}

template< std::size_t N >
__force_inline static_string< N >::static_string(const string_view& s)
    : static_string{s.data(), s.size()}
{}

template< std::size_t N >
__force_inline static_string< N >::static_string(std::initializer_list< char > il)
    : static_string{il.begin(), il.size()}
{}

template< std::size_t N >
__force_inline const char* static_string< N >::data() const noexcept
{
    return storage_;
}

template< std::size_t N >
__force_inline char* static_string< N >::data() noexcept
{
    return storage_;
}

template< std::size_t N >
__force_inline const char* static_string< N >::c_str() const noexcept
{
    return storage_;
}

template< std::size_t N >
__force_inline std::size_t static_string< N >::size() const noexcept
{
    return size_;
}

template< std::size_t N >
__force_inline std::size_t static_string< N >::length() const noexcept
{
    return size_;
}

template< std::size_t N >
__force_inline bool static_string< N >::empty() const noexcept
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
__force_inline void static_string< N >::set_size(std::size_t new_size)
{
    if (__unlikely(new_size > N)) {
        throw_ex< std::out_of_range >("Out of range in static_string::set_size");
    }
    size_ = new_size;
    storage_[size_] = '\0';
}

template< std::size_t N >
__force_inline const char& static_string< N >::at(std::size_t i) const
{
    if (__unlikely(i > size_)) {
        throw_ex< std::out_of_range >("Out of range in static_string::at");
    }
    return storage_[i];
}

template< std::size_t N >
__force_inline char& static_string< N >::at(std::size_t i)
{
    if (__unlikely(i > size_)) {
        throw_ex< std::out_of_range >("Out of range in static_string::at");
    }
    return storage_[i];
}

template< std::size_t N >
__force_inline const char& static_string< N >::operator[](std::size_t i) const noexcept
{
    return storage_[i];
}

template< std::size_t N >
__force_inline char& static_string< N >::operator[](std::size_t i) noexcept
{
    return storage_[i];
}

template< std::size_t N >
__force_inline const char& static_string< N >::front() const noexcept
{
    return (*this)[0u];
}

template< std::size_t N >
__force_inline char& static_string< N >::front() noexcept
{
    return (*this)[0u];
}

template< std::size_t N >
__force_inline const char& static_string< N >::back() const noexcept
{
    return storage_[size_ - 1u];
}

template< std::size_t N >
__force_inline char& static_string< N >::back() noexcept
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
__force_inline static_string< N >& static_string< N >::append(const std::string& s)
{
    return append(s.data(), s.size());
}

template< std::size_t N >
__force_inline static_string< N >& static_string< N >::append(const string_view& s)
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

template< std::size_t N >
__force_inline static_string< N >& static_string< N >::assign(const char* s)
{
    return assign(s, std::strlen(s));
}

template< std::size_t N >
template< std::size_t M >
__force_inline static_string< N >& static_string< N >::assign(const static_string< M >& s)
{
    return assign(s.data(), s.size());
}

template< std::size_t N >
__force_inline static_string< N >& static_string< N >::assign(const std::string& s)
{
    return assign(s.data(), s.size());
}

template< std::size_t N >
__force_inline static_string< N >& static_string< N >::assign(const string_view& s)
{
    return assign(s.data(), s.size());
}

template< std::size_t N >
__force_inline std::string static_string< N >::to_string() const
{
    return {data(), size()};
}

template< std::size_t N >
__force_inline string_view static_string< N >::to_string_view() const
{
    return {data(), size()};
}

template< std::size_t N >
__force_inline static_string< N >::operator string_view() const
{
    return {data(), size()};
}

template< std::size_t N, std::size_t M >
__force_inline bool operator==(const static_string< N >& left, const static_string< M >& right) noexcept
{
    return detail::equal(left.data(), left.size(), right.data(), right.size());
}

template< std::size_t N >
__force_inline bool operator==(const static_string< N >& left, const std::string& right) noexcept
{
    return detail::equal(left.data(), left.size(), right.data(), right.size());
}

template< std::size_t N >
__force_inline bool operator==(const std::string& left, const static_string< N >& right) noexcept
{
    return detail::equal(left.data(), left.size(), right.data(), right.size());
}

template< std::size_t N >
__force_inline bool operator==(const static_string< N >& left, const string_view& right) noexcept
{
    return detail::equal(left.data(), left.size(), right.data(), right.size());
}

template< std::size_t N >
__force_inline bool operator==(const string_view& left, const static_string< N >& right) noexcept
{
    return detail::equal(left.data(), left.size(), right.data(), right.size());
}

template< std::size_t N >
__force_inline bool operator==(const static_string< N >& left, const char* right) noexcept
{
    return detail::equal(left.data(), left.size(), right, std::strlen(right));
}

template< std::size_t N >
__force_inline bool operator==(const char* left, const static_string< N >& right) noexcept
{
    return detail::equal(left, std::strlen(left), right.data(), right.size());
}

} /* namespace plusone */

namespace std {

/** `std::hash` support. */
template< std::size_t N >
__force_inline std::size_t hash< plusone::static_string< N > >::operator()(const plusone::static_string< N >& s) const
{
    return std::hash< plusone::string_view >()(s);
}

} /* namespace std */

#endif /* KSERGEY_static_string_150817175315 */
