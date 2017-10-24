/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#include <plusone/fixed_size_memory_pool.hpp>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>

TEST_CASE("test1")
{
    plusone::fixed_size_memory_pool< 512 > pool{2};
    REQUIRE( pool.allocate() != nullptr );
    REQUIRE( pool.allocate() != nullptr );
    REQUIRE( pool.allocate() == nullptr );
    REQUIRE( pool.allocate() == nullptr );

    pool.deallocate_all();

    REQUIRE( pool.allocate() != nullptr );
    REQUIRE( pool.allocate() != nullptr );
    REQUIRE( pool.allocate() == nullptr );

    pool.deallocate_all();

    auto e = pool.allocate();
    REQUIRE( e != nullptr );
    REQUIRE( pool.allocate() != nullptr);
    REQUIRE( pool.allocate() == nullptr);
    pool.deallocate(e);
    REQUIRE( pool.allocate() != nullptr);
    REQUIRE( pool.allocate() == nullptr);
}

struct T1
{
    T1* link;
};

struct T2
{

};

TEST_CASE("test1")
{
    /* This queue used inside pool */
    plusone::detail::queue< T1 > queue1;

    /* Should not compiler if uncomment. */
    //plusone::detail::queue< T2 > queue2;
}
