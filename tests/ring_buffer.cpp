/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>
#include <plusone/ring_buffer.hpp>

TEST_CASE("Ring buffer default constructor")
{
    plusone::ring_buffer< int > v;

    REQUIRE(v.empty());
    REQUIRE(v.size() == 0);
    REQUIRE(v.capacity() == 0);
}

TEST_CASE("Ring buffer constructor")
{
    plusone::ring_buffer< int > v{16};

    REQUIRE(v.empty());
    REQUIRE(v.size() == 0);
    REQUIRE(v.capacity() == 16);
}

TEST_CASE("Ring buffer bad capacity error")
{
    CHECK_THROWS_AS(plusone::ring_buffer< int >{0}, const plusone::ring_buffer_error&);
    CHECK_THROWS_AS(plusone::ring_buffer< int >{3}, const plusone::ring_buffer_error&);
    CHECK_THROWS_AS(plusone::ring_buffer< int >{14}, const plusone::ring_buffer_error&);
    CHECK_NOTHROW(plusone::ring_buffer< int >{8});
}

TEST_CASE("Ring buffer emplace")
{
    plusone::ring_buffer< int > v{4};

    REQUIRE(v.empty());
    REQUIRE(v.size() == 0);
    REQUIRE(v.capacity() == 4);

    REQUIRE(v.emplace_back(5) == 5);
    REQUIRE(v.back() == 5);
    REQUIRE(v.front() == 5);
    REQUIRE(!v.empty());
    REQUIRE(v.size() == 1);
    REQUIRE(v.capacity() == 4);

    REQUIRE(v.emplace_back(9) == 9);
    REQUIRE(v.back() == 9);
    REQUIRE(v.front() == 5);
    REQUIRE(!v.empty());
    REQUIRE(v.size() == 2);
    REQUIRE(v.capacity() == 4);

    REQUIRE(v.emplace_back(15) == 15);
    REQUIRE(v.back() == 15);
    REQUIRE(v.front() == 5);
    REQUIRE(!v.empty());
    REQUIRE(v.size() == 3);
    REQUIRE(v.capacity() == 4);

    REQUIRE(v[0] == 5);
    REQUIRE(v[1] == 9);
    REQUIRE(v[2] == 15);

    REQUIRE(v.emplace_back(3) == 3);
    REQUIRE(v.back() == 3);
    REQUIRE(v.front() == 5);
    REQUIRE(v.size() == 4);

    REQUIRE(v[0] == 5);
    REQUIRE(v[1] == 9);
    REQUIRE(v[2] == 15);
    REQUIRE(v[3] == 3);

    REQUIRE(v.emplace_back(33) == 33);
    REQUIRE(v.back() == 33);
    REQUIRE(v.front() == 9);
    REQUIRE(v.size() == 4);

    REQUIRE(v[0] == 9);
    REQUIRE(v[1] == 15);
    REQUIRE(v[2] == 3);
    REQUIRE(v[3] == 33);

    REQUIRE(v.emplace_back(1) == 1);
    REQUIRE(v.back() == 1);
    REQUIRE(v.front() == 15);
    REQUIRE(v.size() == 4);
    REQUIRE(v.full());

    REQUIRE(v[0] == 15);
    REQUIRE(v[1] == 3);
    REQUIRE(v[2] == 33);
    REQUIRE(v[3] == 1);

    auto z = std::move(v);

    REQUIRE(v.empty());
    REQUIRE(v.size() == 0);
    REQUIRE(v.capacity() == 0);

    REQUIRE(z.back() == 1);
    REQUIRE(z.front() == 15);
    REQUIRE(z.size() == 4);
    REQUIRE(z.full());

    REQUIRE(z[0] == 15);
    REQUIRE(z[1] == 3);
    REQUIRE(z[2] == 33);
    REQUIRE(z[3] == 1);

    z.clear();
    REQUIRE(z.empty());
    REQUIRE(!z.full());
    REQUIRE(z.size() == 0);
    REQUIRE(z.capacity() == 4);
}
