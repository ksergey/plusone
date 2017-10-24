/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_type_traits_241017182855
#define KSERGEY_type_traits_241017182855

#include <type_traits>

namespace plusone {
namespace detail {

template< class... Ts >
struct make_void
{
    using type = void;
};

} /* namespace detail */

/**
 * Helper to write concepts.
 * @see Example at plusone/detail/queue.hpp
 */
template< class... Ts >
using void_t = typename detail::make_void< Ts... >::type;

} /* namespace plusone */

#endif /* KSERGEY_type_traits_241017182855 */
