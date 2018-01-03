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

TEST_CASE("Test2")
{
    plusone::linear_allocator alloc1{8};
    plusone::linear_allocator alloc2;

    auto res = alloc1.allocate(2, 2);
    REQUIRE( res );
    res = alloc2.allocate(2, 2);
    REQUIRE( !res );

    alloc2 = std::move(alloc1);
    res = alloc2.allocate(2, 2);
    REQUIRE( res );
    res = alloc1.allocate(2, 2);
    REQUIRE( !res );
}

TEST_CASE("Test3")
{
    plusone::linear_allocator a1;
    std::cout << a1.total_size() << '/' << a1.allocated_size() << '/' << a1.free_size() << '\n';

    a1 = plusone::linear_allocator{16};
    std::cout << a1.total_size() << '/' << a1.allocated_size() << '/' << a1.free_size() << '\n';

    auto res = a1.allocate(2);
    REQUIRE( res );
    std::cout << a1.total_size() << '/' << a1.allocated_size() << '/' << a1.free_size() << '\n';

    res = a1.allocate(8);
    REQUIRE( res );
    std::cout << a1.total_size() << '/' << a1.allocated_size() << '/' << a1.free_size() << '\n';

    res = a1.allocate(4);
    REQUIRE( res );
    REQUIRE( a1.allocated_size() == 14 );
    REQUIRE( a1.free_size() == 2 );
    std::cout << a1.total_size() << '/' << a1.allocated_size() << '/' << a1.free_size() << '\n';

    res = a1.allocate(4);
    REQUIRE( !res );
    REQUIRE( a1.allocated_size() == 14 );
    REQUIRE( a1.free_size() == 2 );
    std::cout << a1.total_size() << '/' << a1.allocated_size() << '/' << a1.free_size() << '\n';

    res = a1.allocate(2);
    REQUIRE( res );
    REQUIRE( a1.allocated_size() == 16 );
    REQUIRE( a1.free_size() == 0 );
    std::cout << a1.total_size() << '/' << a1.allocated_size() << '/' << a1.free_size() << '\n';

    a1.reset();
    REQUIRE( a1.allocated_size() == 0 );
    REQUIRE( a1.free_size() == 16 );
    std::cout << a1.total_size() << '/' << a1.allocated_size() << '/' << a1.free_size() << '\n';
}
