/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>
#include <plusone/static_vector.hpp>
#include <plusone/static_vector_base.hpp>

TEST_CASE("static_vector_base")
{
    char storage[5 * sizeof(int)];
    plusone::static_vector_base< int > v{reinterpret_cast< int* >(storage), 5};

    REQUIRE(v.empty());
    REQUIRE(v.size() == 0);
    REQUIRE(v.capacity() == 5);

    SUBCASE("Adding to the vector increases it's size (1)") {
        auto& e = v.emplace_back(99);
        REQUIRE(!v.empty());
        REQUIRE(v.size() == 1);
        REQUIRE(e == 99);
        REQUIRE(v.front() == 99);
        REQUIRE(v.back() == 99);
        REQUIRE(v[0] == 99);
        REQUIRE(*v.begin() == 99);
    }

    SUBCASE("Adding to the vector increases it's size (2)") {
        v.emplace_back(66);
        v.emplace_back(33);
        REQUIRE(!v.empty());
        REQUIRE(v.size() == 2);
        REQUIRE(v.front() == 66);
        REQUIRE(v.back() == 33);
        REQUIRE(v[0] == 66);
        REQUIRE(v[1] == 33);
        auto it = v.begin();
        REQUIRE(*it == 66);
        ++it;
        REQUIRE(*it == 33);
    }

}

TEST_CASE("static_vector default constructor")
{
    plusone::static_vector< int > v;

    REQUIRE(v.empty());
    REQUIRE(v.size() == 0);
    REQUIRE(v.capacity() == 0);
}

TEST_CASE("static_vector constructor")
{
    plusone::static_vector< int > v{5};

    REQUIRE(v.empty());
    REQUIRE(v.size() == 0);
    REQUIRE(v.capacity() == 5);

    SUBCASE("Adding to the vector increases it's size (1)") {
        auto& e = v.emplace_back(99);
        REQUIRE(!v.empty());
        REQUIRE(v.size() == 1);
        REQUIRE(e == 99);
        REQUIRE(v.front() == 99);
        REQUIRE(v.back() == 99);
        REQUIRE(v[0] == 99);
        REQUIRE(*v.begin() == 99);
    }

    SUBCASE("Adding to the vector increases it's size (2)") {
        v.emplace_back(66);
        v.emplace_back(33);
        REQUIRE(!v.empty());
        REQUIRE(v.size() == 2);
        REQUIRE(v.front() == 66);
        REQUIRE(v.back() == 33);
        REQUIRE(v[0] == 66);
        REQUIRE(v[1] == 33);
        auto it = v.begin();
        REQUIRE(*it == 66);
        ++it;
        REQUIRE(*it == 33);
    }

    SUBCASE("Move semantic") {
        plusone::static_vector< int > x{10};
        x.emplace_back(1000);
        x.emplace_back(10000);
        REQUIRE(x.size() == 2);
        REQUIRE(x.capacity() == 10);

        REQUIRE(v.empty());
        REQUIRE(v.size() == 0);
        REQUIRE(v.capacity() == 5);

        v = std::move(x);
        REQUIRE(x.empty());
        REQUIRE(x.size() == 0);
        REQUIRE(x.capacity() == 5);

        REQUIRE(v.size() == 2);
        REQUIRE(v.capacity() == 10);
        REQUIRE(v.back() == 10000);
        REQUIRE(v.front() == 1000);
    }
}
