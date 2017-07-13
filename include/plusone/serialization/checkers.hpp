/*
 * Copyright (c) 2016-2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef MADLIFE_checkers_081116115904_MADLIFE
#define MADLIFE_checkers_081116115904_MADLIFE

#include <initializer_list>
#include "common.hpp"

namespace plusone {
namespace serialization {

/**
 * Create checker to check an input value greater @c value
 */
template< class T >
auto greater(const T& value);

/**
 * Create checker to check an input value greater or equal @c value
 */
template< class T >
auto greater_equal(const T& value);

/**
 * Create checker to check an input value less @c value
 */
template< class T >
auto less(const T& value);

/**
 * Create checker to check an input value less or equal @c value
 */
template< class T >
auto less_equal(const T& value);

/**
 * Create checker to check an input value not equal to @c value
 */
template< class T >
auto not_equal_to(const T& value);

/**
 * Create checker to check an input value greater or equal @c left and less or equal @c right
 */
template< class T >
auto inside(const T& left, const T& right);

/**
 * Create checker to check an input value greater @c left and less @c right
 */
template< class T >
auto strong_inside(const T& left, const T& right);

/**
 * Create checker to check an input string not empty
 */
auto not_empty_string();

/**
 * Create checker to check an input string size less or equal @c max_length
 */
auto max_length_string(size_t max_length);

/**
 * Create checker to check an input value is one of @c values
 */
template< class T >
auto one_of(std::initializer_list< T > values);

} /* namespace serialization */
} /* namespace plusone */

#include "impl/checkers.ipp"

#endif /* MADLIFE_checkers_081116115904_MADLIFE */
