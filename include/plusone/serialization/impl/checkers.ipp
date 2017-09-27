/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_checkers_130717112216
#define KSERGEY_checkers_130717112216

#include <string>
#include <set>
#include <functional>

namespace plusone {
namespace serialization {
namespace detail {

/** STL-based checker */
template< class T, template< class > class C >
class stl_comparator
{
private:
    T value_;

public:
    explicit stl_comparator(const T& value = 0)
        : value_(value)
    {}

    void operator()(const T& value) const
    {
        static const C< T > cmp{};
        if (!cmp(value, value_)) {
            throw_ex< check_error >("Value not passed validation");
        }
    }
};

/** Check value inside of a range */
template< class T, template< class > class Left, template< class > class Right >
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

    void operator()(const T& value) const
    {
        static const Left< T > cmp_left{};
        static const Right< T > cmp_right{};

        if (!cmp_left(value, left_) || !cmp_right(value, right_)) {
            throw_ex< check_error >("Value not passed validation, not inside range");
        }
    }
};

} /* namespace detail */

template< class T >
__force_inline auto greater(const T& value)
{
    return detail::stl_comparator< T, std::greater >(value);
}

template< class T >
__force_inline auto greater_equal(const T& value)
{
    return detail::stl_comparator< T, std::greater_equal >(value);
}

template< class T >
__force_inline auto less(const T& value)
{
    return detail::stl_comparator< T, std::less >(value);
}

template< class T >
__force_inline auto less_equal(const T& value)
{
    return detail::stl_comparator< T, std::less_equal >(value);
}

template< class T >
__force_inline auto not_equal_to(const T& value)
{
    return detail::stl_comparator< T, std::not_equal_to >(value);
}

template< class T >
__force_inline auto inside(const T& left, const T& right)
{
    return detail::inside_checker< T, std::greater_equal, std::less_equal >(left, right);
}

template< class T >
__force_inline auto strong_inside(const T& left, const T& right)
{
    return detail::inside_checker< T, std::greater, std::less >(left, right);
}

namespace detail {

/* Not an empty string checker impl */
struct not_empty_string_impl
{
    void operator()(const std::string& s) const
    {
        if (s.empty()) {
            throw_ex< check_error >("Value not passed validation, empty string");
        }
    }

    template< std::size_t N >
    void operator()(const plusone::static_string< N >& s) const
    {
        if (s.empty()) {
            throw_ex< check_error >("Value not passed validation, empty string");
        }
    }
};

} /* namespace detail */

__force_inline auto not_empty_string()
{
    return detail::not_empty_string_impl{};
}

__force_inline auto max_length_string(size_t max_length)
{
    return [max_length](const std::string& str) {
        if (str.size() > max_length) {
            throw_ex< check_error >("Value not passed validation, string too long");
        }
    };
}

template< class T >
__force_inline auto one_of(std::initializer_list< T > values)
{
    const std::set< T > values_set(values);
    return [values_set](const T& value) {
        if (values_set.count(value) == 0) {
            throw_ex< check_error >("Value not passed validation, out of possible values");
        }
    };
}

} /* namespace serialization */
} /* namespace plusone */

#endif /* KSERGEY_checkers_130717112216 */
