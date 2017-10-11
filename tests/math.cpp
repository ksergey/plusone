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

struct parse_float
{
    constexpr bool is_digit(char ch) const noexcept
    {
        return ch >= '0' && ch <= '9';
    }

    __force_inline std::int64_t operator()(const char* data, std::size_t size) const noexcept
    {
        if (__unlikely(size == 0)) {
            return 0;
        }

        const char* first = data;
        const char* last = first + size;

        bool negative{false};
        if (*first == '-') {
            negative = true;
            ++first;
        } else if (*first == '+') {
            ++first;
        }

        std::int64_t mantissa{0};
        std::int8_t exponent_offset{0};

        for (; first < last; ++first) {
            if (!is_digit(*first)) {
                break;
            }
            mantissa = mantissa * 10 + (*first - '0');
        }

        if (*first == '.') {
            ++first;
            for (; first < last; ++first) {
                if (!is_digit(*first)) {
                    break;
                }
                mantissa = mantissa * 10 + (*first - '0');
                --exponent_offset;
            }
        }

        std::int8_t exponent = 0;
        if (__unlikely(*first == 'e')) {
            ++first;
            bool negative_exponent{false};
            if (*first == '-') {
                negative_exponent = true;
                ++first;
            } else if (*first == '+') {
                ++first;
            }
            for (; first < last; ++first) {
                if (!is_digit(*first)) {
                    break;
                }
                exponent = exponent * 10 + (*first - '0');
            }
            if (negative_exponent) {
                exponent = -exponent;
            }
        }
        exponent += exponent_offset;
        exponent += 8;

        if (__unlikely(exponent < 0 || exponent > plusone::pow10::table_size)) {
            /* Error */
            return 0;
        }

        std::int64_t ret = mantissa * plusone::pow10::get(exponent);

        return negative ? -ret : ret;
    }

    auto operator()(const std::string& s) const noexcept
    {
        return (*this)(s.data(), s.size());
    }
};

void dump(const std::string& v)
{
    auto r = parse_float{}(v.data(), v.size());
    std::cout << "in=" << v << " out=" << r << "(" << r * 0.00000001 << ")\n";
}

TEST_CASE("Parse Float")
{
    dump("1.2");
    dump("2.2");
    dump("-2.2135");
    dump("-3.2135");
    dump("1.2e-5");
    dump("1.2e5");
    dump("1.2e-7");
    dump("1.2e-8");
}
