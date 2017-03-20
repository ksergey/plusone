/*
 * Copyright (c) 2016-2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef MADLIFE_meta_081116114243_MADLIFE
#define MADLIFE_meta_081116114243_MADLIFE

#include <type_traits>
#include "common.hpp"

namespace plusone {
namespace serialization {

/* Check if a class have a serialize method */

template< class T, class DTO, typename = void >
struct has_serialize
    : std::false_type
{};

template< class T, class DTO >
struct has_serialize< T, DTO, typename std::enable_if<
        std::is_same<
            decltype(std::declval< T >().serialize(std::declval< DTO& >())),
            void
        >::value
    >::type >
    : std::true_type
{};

template< class T >
using if_integral_type = typename std::enable_if<
    std::is_integral< T >::value
>::type;

template< class T, class DTO >
using if_serializable_type = typename std::enable_if<
    has_serialize< T, DTO >::value
>::type;

} /* namespace serialization */
} /* namespace plusone */

#endif /* MADLIFE_meta_081116114243_MADLIFE */
