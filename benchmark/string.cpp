/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#include <plusone/static_string.hpp>

#include <benchmark/benchmark.h>

static void benchmark_string(benchmark::State& state)
{
    std::string s;
    for (auto _: state) {
        for (int x = 0; x < state.range(0); ++x) {
            s.assign("123456789123456789123456789");
        }
        benchmark::DoNotOptimize(&s);
    }
}
BENCHMARK(benchmark_string)->Arg(128);

static void benchmark_static_string(benchmark::State& state)
{
    using static_string = plusone::static_string< 32 >;
    static_string s;
    for (auto _: state) {
        for (int x = 0; x < state.range(0); ++x) {
            s.assign("123456789123456789123456789");
        }
        benchmark::DoNotOptimize(&s);
    }
}
BENCHMARK(benchmark_static_string)->Arg(128);

BENCHMARK_MAIN();
