/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#include <plusone/static_string.hpp>

#include <benchmark/benchmark.h>

static void benchmark_string(benchmark::State& state)
{
    std::string s;
    for (auto _: state) {
        s.assign("123456789123456789123456789");
        benchmark::DoNotOptimize(&s);
    }
}
BENCHMARK(benchmark_string)
    ->Repetitions(10)
    ->ReportAggregatesOnly(true);

static void benchmark_static_string(benchmark::State& state)
{
    using static_string = plusone::static_string< 32 >;
    static_string s;
    for (auto _: state) {
        s.assign("123456789123456789123456789");
        benchmark::DoNotOptimize(&s);
    }
}
BENCHMARK(benchmark_static_string)
    ->Repetitions(10)
    ->ReportAggregatesOnly(true);

BENCHMARK_MAIN();
