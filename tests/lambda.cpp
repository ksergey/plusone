/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#include <iostream>
#include <string>
#include <plusone/compiler.hpp>

template< class... Ts >
struct lambda_visitor;

template< class T, class... Ts >
struct lambda_visitor< T, Ts... >
    : lambda_visitor< Ts... >
    , T
{
    using T::operator();
    using lambda_visitor< Ts... >::operator();

    lambda_visitor(T t, Ts... ts)
        : lambda_visitor< Ts... >(ts...)
        , T{t}
    {}
};

template< class T >
struct lambda_visitor< T >
    : T
{
    using T::operator();

    lambda_visitor(T t)
        : T{t}
    {}
};

template< class... Fs >
auto make_lambda(Fs&&... fs)
{
    using type = lambda_visitor< std::decay_t< Fs >... >;
    return type{std::forward< Fs >(fs)...};
}

int main(int argc, char* argv[])
{
    plusone::ignore_unused(argc, argv);

    int int_value = 5;
    std::string string_value{"Test"};

    try {
        auto v = make_lambda(
            [](int v) {
                std::cout << "int value " << v << '\n';
            },
            [](const std::string& v) {
                std::cout << "string value " << v << '\n';
            }
        );

        v(string_value);
        v(int_value);
        v(7);
    } catch (const std::exception& e) {
        std::cout << "ERROR: " << e.what() << '\n';
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
