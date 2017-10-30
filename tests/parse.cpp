#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>
#include <plusone/string_view.hpp>
#include <plusone/parse.hpp>

using plusone::string_view;
using namespace plusone::string_view_literals;

TEST_CASE("Parse positive decimal")
{
    auto v1 = plusone::parse_positive_decimal8("10"sv);
    auto v2 = plusone::parse_positive_decimal8("0.123"sv);
    auto v3 = plusone::parse_positive_decimal8("9e2"sv);
    auto v4 = plusone::parse_positive_decimal8("3e-2"sv);
    auto v5 = plusone::parse_positive_decimal8("9.5e4"sv);
    auto v6 = plusone::parse_positive_decimal8("3.112e-3"sv);

    REQUIRE(v1 == 1000000000ul);
    REQUIRE(v2 == 12300000ul);
    REQUIRE(v3 == 90000000000ul);
    REQUIRE(v4 == 3000000ul);
    REQUIRE(v5 == 9500000000000ul);
    REQUIRE(v6 == 311200ul);
}

TEST_CASE("Parse signed decimal8")
{
    auto v1 = plusone::parse_decimal8("-10"sv);
    auto v2 = plusone::parse_decimal8("0.123"sv);
    auto v3 = plusone::parse_decimal8("9e2"sv);
    auto v4 = plusone::parse_decimal8("-3e-2"sv);
    auto v5 = plusone::parse_decimal8("-9.5e4"sv);
    auto v6 = plusone::parse_decimal8("3.112e-3"sv);
	auto v7 = plusone::parse_decimal8("0"sv);

    REQUIRE(v1 == -1000000000l);
    REQUIRE(v2 == 12300000l);
    REQUIRE(v3 == 90000000000l);
    REQUIRE(v4 == -3000000l);
    REQUIRE(v5 == -9500000000000l);
    REQUIRE(v6 == 311200l);
    REQUIRE(v7 == 0);
}