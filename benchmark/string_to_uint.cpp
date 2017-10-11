/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#include <vector>
#include <string>
#include <random>
#include <limits>
#include <algorithm>
#include <plusone/math.hpp>
#include <benchmark/benchmark.h>

std::vector< std::string > create_integer_strings()
{
    std::vector< std::string > result;
    result.reserve(200);

    std::random_device rd;
    std::mt19937 generator{rd()};

    std::uint64_t min{1};
    for (std::size_t i = 0; i != 20; ++i) {
        std::uint64_t max{10 * min - 1};

        if (max < min) {
            max = std::numeric_limits< std::uint64_t >::max();
        }

        std::uniform_int_distribution< std::uint64_t > random{min, max};

        for (std::size_t j = 0; j != 10; ++j) {
            result.emplace_back(std::to_string(random(generator)));
        }

        min *= 10;
    }

    std::shuffle(result.begin(), result.end(), generator);

    return result;
}

static const auto data = create_integer_strings();

struct string_to_uint_1
{
    std::uint64_t operator()(const char* data, std::size_t size) const noexcept
    {
        std::uint64_t ret = 0;
        const char* first = data;
        const char* last = first + size;

        while (first < last) {
            ret *= 10;
            ret += *(first++) - '0';
        }
        return ret;
    }
};

struct string_to_uint_2
{
    std::uint64_t operator()(const char* first, const char* last) const noexcept
    {
        if (first == last) {
            return 0;
        }
        --last;
        return 10 * (*this)(first, last) + (*last - '0');
    }

    __force_inline std::uint64_t operator()(const char* data, std::size_t size) const noexcept
    {
        return (*this)(data, data + size);
    }
};

struct string_to_uint_3
{
    __force_inline std::uint64_t operator()(const char* data, std::size_t size) const noexcept
    {
        std::uint64_t ret = 0;

        switch (size)  {
            case 21: ret += (data[size - 21] - '0') * plusone::pow10::get(21 - 1);
            case 20: ret += (data[size - 20] - '0') * plusone::pow10::get(20 - 1);
            case 19: ret += (data[size - 19] - '0') * plusone::pow10::get(19 - 1);
            case 18: ret += (data[size - 18] - '0') * plusone::pow10::get(18 - 1);
            case 17: ret += (data[size - 17] - '0') * plusone::pow10::get(17 - 1);
            case 16: ret += (data[size - 16] - '0') * plusone::pow10::get(16 - 1);
            case 15: ret += (data[size - 15] - '0') * plusone::pow10::get(15 - 1);
            case 14: ret += (data[size - 14] - '0') * plusone::pow10::get(14 - 1);
            case 13: ret += (data[size - 13] - '0') * plusone::pow10::get(13 - 1);
            case 12: ret += (data[size - 12] - '0') * plusone::pow10::get(12 - 1);
            case 11: ret += (data[size - 11] - '0') * plusone::pow10::get(11 - 1);
            case 10: ret += (data[size - 10] - '0') * plusone::pow10::get(10 - 1);
            case 9:  ret += (data[size - 9] - '0') * plusone::pow10::get(9 - 1);
            case 8:  ret += (data[size - 8] - '0') * plusone::pow10::get(8 - 1);
            case 7:  ret += (data[size - 7] - '0') * plusone::pow10::get(7 - 1);
            case 6:  ret += (data[size - 6] - '0') * plusone::pow10::get(6 - 1);
            case 5:  ret += (data[size - 5] - '0') * plusone::pow10::get(5 - 1);
            case 4:  ret += (data[size - 4] - '0') * plusone::pow10::get(4 - 1);
            case 3:  ret += (data[size - 3] - '0') * plusone::pow10::get(3 - 1);
            case 2:  ret += (data[size - 2] - '0') * plusone::pow10::get(2 - 1);
            case 1:  ret += (data[size - 1] - '0') * plusone::pow10::get(1 - 1);
            default:
                     break;
        }

        return ret;
    }
};

template< class Func >
static void run_bench(benchmark::State& state)
{
    Func f;

    auto data_copy = data;

    for (auto _: state) {
        for (auto& s: data_copy) {
#if 0
            auto x = std::to_string(f(s.data(), s.size()));
            if (x != s) {
                throw std::runtime_error("bench error " + s + ' ' + x);
            }
#endif
            benchmark::DoNotOptimize(f(s.data(), s.size()));
        }
    }
}

BENCHMARK_TEMPLATE(run_bench, string_to_uint_1);
BENCHMARK_TEMPLATE(run_bench, string_to_uint_2);
BENCHMARK_TEMPLATE(run_bench, string_to_uint_3);

BENCHMARK_MAIN();
