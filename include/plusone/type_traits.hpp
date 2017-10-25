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

/**
 * Call F for each argument.
 *
 * Example:
 * @code
 *   int x = 1;
 *   double y = 2;
 *   std::string s = "hello";
 *
 *   for_each([](auto& s) {
 *     std::cout << x << '\n';
 *   }, x, y, s);
 * @endcode
 *
 * @see https://stackoverflow.com/a/28279483
 */
template< class F, class... Args >
void for_each(F&& f, Args&&... args)
{
    [](...){}((f(std::forward< Args >(args)), 0)...);
}

} /* namespace plusone */

#endif /* KSERGEY_type_traits_241017182855 */
