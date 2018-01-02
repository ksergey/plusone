/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>
#include <plusone/linear_allocator.hpp>
#include <iostream>

TEST_CASE("Constructor")
{
    plusone::linear_allocator alloc{32};
    auto res = alloc.allocate(2);
    std::cout << (void*) res << '\n';
    res = alloc.allocate(2);
    std::cout << (void*) res << '\n';
    res = alloc.allocate(2);
    std::cout << (void*) res << '\n';

    std::cout << "---\n";
    alloc.reset();

    res = alloc.allocate(2, 2);
    std::cout << (void*) res << '\n';
    res = alloc.allocate(2, 2);
    std::cout << (void*) res << '\n';
    res = alloc.allocate(2, 2);
    std::cout << (void*) res << '\n';

    std::cout << "---\n";
    alloc.reset();

    res = alloc.allocate(2, 8);
    std::cout << (void*) res << '\n';
    res = alloc.allocate(2, 8);
    std::cout << (void*) res << '\n';
    res = alloc.allocate(2, 8);
    std::cout << (void*) res << '\n';

    std::cout << "---\n";
    alloc.reset();

    res = alloc.allocate(3, 2);
    std::cout << (void*) res << '\n';
    res = alloc.allocate(3, 2);
    std::cout << (void*) res << '\n';
    res = alloc.allocate(3, 2);
    std::cout << (void*) res << '\n';
}

TEST_CASE("Test1")
{
    plusone::linear_allocator alloc{8};
    auto res = alloc.allocate(2, 2);
    REQUIRE( res );
    res = alloc.allocate(2, 2);
    REQUIRE( res );
    res = alloc.allocate(2, 2);
    REQUIRE( res );
}
