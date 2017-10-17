/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>
#include <cmath>
#include <plusone/math.hpp>

TEST_CASE("Upper power of two")
{
    constexpr auto v1 = plusone::upper_power_of_two(5);
    constexpr auto v2 = plusone::upper_power_of_two(v1);
    constexpr auto v3 = plusone::upper_power_of_two(16);
    constexpr auto v4 = plusone::upper_power_of_two(15);

    REQUIRE(v1 == 8);
    REQUIRE(v2 == 8);
    REQUIRE(v3 == 16);
    REQUIRE(v4 == 16);
}

TEST_CASE("Is power of two")
{
    constexpr auto v1 = plusone::is_power_of_two(4);
    constexpr auto v2 = plusone::is_power_of_two(3);
    constexpr auto v3 = plusone::is_power_of_two(16);
    constexpr auto v4 = plusone::is_power_of_two(17);

    REQUIRE(v1 == true);
    REQUIRE(v2 == false);
    REQUIRE(v3 == true);
    REQUIRE(v4 == false);
    REQUIRE(plusone::is_power_of_two(0) == false);
    REQUIRE(plusone::is_power_of_two(65536) == true);
}

TEST_CASE("Pow detail 0")
{
    constexpr auto v1 = plusone::detail::pow(10, 0);
    constexpr auto v2 = plusone::detail::pow(10, 1);
    constexpr auto v3 = plusone::detail::pow(10, 2);
    constexpr auto v4 = plusone::detail::pow(10, 5);

    REQUIRE(v1 == 1);
    REQUIRE(v2 == 10);
    REQUIRE(v3 == 100);
    REQUIRE(v4 == 100000);
}

TEST_CASE("Pow detail 1")
{
    constexpr auto arr = plusone::detail::make_pow_array< std::int64_t, 19 >(10);
    using arr_type = decltype(arr);

    REQUIRE(std::is_same< arr_type::value_type, std::int64_t >());
    REQUIRE(arr.size() == 19);

    for (std::size_t i = 0; i < arr.size(); ++i) {
        REQUIRE(arr[i] == std::pow(10ll, i));
    }
}

TEST_CASE("Pow10")
{
    for (unsigned i = 0; i < 19; ++i) {
        REQUIRE(plusone::pow10::get(i) == std::pow(10ll, i));
    }
}
