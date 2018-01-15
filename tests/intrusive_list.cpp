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

TEST_CASE("IntrusiveList move semantic")
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

    list_t list1;
    REQUIRE( list1.size() == 0 );
    REQUIRE( list1.empty() );
    REQUIRE( list1.begin() == list1.end() );
    REQUIRE( list1.cbegin() == list1.cend() );
    REQUIRE( list1.rbegin() == list1.rend() );

    entry x4{9};
    entry x5{11};
    list1.push_back(x4);
    list1.push_back(x5);

    REQUIRE( list1.size() == 2 );
    REQUIRE( !list1.empty() );
    REQUIRE( list1.begin() != list1.end() );
    REQUIRE( list1.cbegin() != list1.cend() );
    REQUIRE( list1.rbegin() != list1.rend() );
    REQUIRE( list1.front().val == 9 );
    REQUIRE( list1.back().val == 11 );

    list_t list2(std::move(list));
    REQUIRE( list.size() == 0 );
    REQUIRE( list.empty() );
    REQUIRE( list.begin() == list.end() );
    REQUIRE( list.cbegin() == list.cend() );
    REQUIRE( list.rbegin() == list.rend() );
    REQUIRE( list2.size() == 3 );
    REQUIRE( !list2.empty() );
    REQUIRE( list2.begin() != list2.end() );
    REQUIRE( list2.cbegin() != list2.cend() );
    REQUIRE( list2.rbegin() != list2.rend() );
    REQUIRE( list2.front().val == 3 );
    REQUIRE( list2.back().val == 5 );

    list = std::move(list1);
    REQUIRE( list.size() == 2 );
    REQUIRE( !list.empty() );
    REQUIRE( list.begin() != list.end() );
    REQUIRE( list.cbegin() != list.cend() );
    REQUIRE( list.rbegin() != list.rend() );
    REQUIRE( list.front().val == 9 );
    REQUIRE( list.back().val == 11 );
}
