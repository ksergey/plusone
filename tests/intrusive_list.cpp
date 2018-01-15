/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>
#include <plusone/intrusive_list.hpp>
#include <iostream>

TEST_CASE("IntrusiveList Default Constructor")
{
    plusone::intrusive_list< plusone::intrusive_list_node > list;
    REQUIRE( list.size() == 0 );
    REQUIRE( list.empty() );
    REQUIRE( list.begin() == list.end() );
    REQUIRE( list.cbegin() == list.cend() );
    REQUIRE( list.rbegin() == list.rend() );
}

struct entry
    : plusone::intrusive_list_node
{
    int val;

    entry() = default;

    entry(int v)
    {
        val = v;
    }
};

TEST_CASE("IntrusiveList")
{
    using list_t = plusone::intrusive_list< entry >;
    list_t list;
    REQUIRE( list.size() == 0 );
    REQUIRE( list.empty() );
    REQUIRE( list.begin() == list.end() );
    REQUIRE( list.cbegin() == list.cend() );
    REQUIRE( list.rbegin() == list.rend() );

    entry x1{3};
    entry x2{2};
    entry x3{5};

    list.push_back(x1);
    list.push_back(x2);
    list.push_back(x3);

    REQUIRE( list.size() == 3 );
    REQUIRE( !list.empty() );
    REQUIRE( list.begin() != list.end() );
    REQUIRE( list.cbegin() != list.cend() );
    REQUIRE( list.rbegin() != list.rend() );
    REQUIRE( list.front().val == 3 );
    REQUIRE( list.back().val == 5 );

    for (auto& e: list) {
        std::cout << e.val << ' ';
    }
    std::cout << '\n';

    list.erase(list.begin());
    REQUIRE( list.size() == 2 );
    REQUIRE( list.front().val == 2 );
    REQUIRE( list.back().val == 5 );

    for (auto& e: list) {
        std::cout << e.val << ' ';
    }
    std::cout << '\n';

    list_t::remove(x3);
    REQUIRE( list.size() == 1 );
    REQUIRE( list.front().val == 2 );
    REQUIRE( list.back().val == 2 );

    for (auto& e: list) {
        std::cout << e.val << ' ';
    }
    std::cout << '\n';
}
