/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_visitor_261117141401
#define KSERGEY_visitor_261117141401

#include <type_traits>
#include <utility>

namespace plusone {
namespace detail {

template< class... Fs >
struct visitor_impl;

template< class F, class... Fs >
struct visitor_impl< F, Fs... >
    : visitor_impl< Fs... >
    , F
{
    using F::operator();
    using visitor_impl< Fs... >::operator();

    visitor_impl(F&& f, Fs&&... fs)
        : visitor_impl< Fs... >{std::forward< Fs >(fs)...}
        , F{std::forward< F >(f)}
    {}
};

template< class F >
struct visitor_impl< F >
    : F
{
    using F::operator();

    visitor_impl(F&& f)
        : F{std::forward< F >(f)}
    {}
};

} /* namespace detail */

/**
 * Make functional object from lambdas.
 *
 * @code
 * auto visitor = make_visitor(
 *    [](int v) { std::cout << v << '\n'; },
 *    [](int x, int y) { std::cout << x + y << '\n'; },
 *    [](const std::string& v) { std::cout << v << '\n'; }
 * );
 * ...
 * visitor("test");
 * visitor(1);
 * visitor(3, 2);
 * @endcode
 */
template< class... Fs >
auto make_visitor(Fs&&... fs) noexcept
{
    using visitor_type = detail::visitor_impl< std::decay_t< Fs >... >;
    return visitor_type{std::forward< Fs >(fs)...};
}

} /* namespace plusone */

#endif /* KSERGEY_visitor_261117141401 */
