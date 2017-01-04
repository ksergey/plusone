/*
 * Copyright (c) 2016-2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef MADLIFE_checkers_081116115904_MADLIFE
#define MADLIFE_checkers_081116115904_MADLIFE

#include <stdexcept>
#include <string>
#include <set>
#include <functional>
#include "common.hpp"

namespace plusone {
namespace serialization {

/** stl-based checker */
template< class T, template< class > class C >
class stl_comparator
{
private:
    T value_;

public:
    explicit stl_comparator(const T& value = 0)
        : value_(value)
    {}

    __force_inline void operator()(const T& value) const
    {
        static const C< T > cmp{};
        if (__unlikely(!cmp(value, value_))) {
            throw check_error{"value not passed validation"};
        }
    }
};

/** check value inside of a range */
template<
    class T,
    template< class > class Left,
    template< class > class Right
>
class inside_checker
{
private:
    const T left_;
    const T right_;

public:
    inside_checker(const T& left, const T& right)
        : left_(left)
        , right_(right)
    {}

    __force_inline void operator()(const T& value) const
    {
        static const Left< T > cmp_left{};
        static const Right< T > cmp_right{};

        if (__unlikely(!cmp_left(value, left_) || !cmp_right(value, right_))) {
            throw check_error{"value not passed validation, not inside range"};
        }
    }
};

template< class T >
__force_inline auto greater(const T& value)
{ return stl_comparator< T, std::greater >(value); }

template< class T >
__force_inline auto greater_equal(const T& value)
{ return stl_comparator< T, std::greater_equal >(value); }

template< class T >
__force_inline auto less(const T& value)
{ return stl_comparator< T, std::less >(value); }

template< class T >
__force_inline auto less_equal(const T& value)
{ return stl_comparator< T, std::less_equal >(value); }

template< class T >
__force_inline auto not_equal_to(const T& value)
{ return stl_comparator< T, std::not_equal_to >(value); }

template< class T >
__force_inline auto inside(const T& left, const T& right)
{ return inside_checker< T, std::greater_equal, std::less_equal >(left, right); }

template< class T >
__force_inline auto strong_inside(const T& left, const T& right)
{ return inside_checker< T, std::greater, std::less >(left, right); }

__force_inline auto not_empty_string()
{
    return [](const std::string& str) {
        if (__unlikely(str.empty())) {
            throw check_error{"value not passed validation, empty string"};
        }
    };
}

__force_inline auto max_length_string(size_t max_length)
{
    return [max_length](const std::string& str) {
        if (__unlikely(str.size() > max_length)) {
            throw check_error{"value not passed validation, string too long"};
        }
    };
}

template< class T >
__force_inline auto one_of(std::initializer_list< T > values)
{
    const std::set< T > values_set(values);
    return [values_set](const T& value) {
        if (__unlikely(values_set.count(value) == 0)) {
            throw check_error{"value not passed validation, out of possible values"};
        }
    };
}

} /* namespace serialization */
} /* namespace plusone */

#endif /* MADLIFE_checkers_081116115904_MADLIFE */
