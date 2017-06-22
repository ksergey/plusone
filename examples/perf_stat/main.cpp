/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#include <iostream>
#include <plusone/perf_stat.hpp>

int main(int argc, char* argv[])
{
    try {
        plusone::perf_stat stats;
        stats.setup<>({
            PERF_COUNT_HW_CACHE_REFERENCES,
            PERF_COUNT_HW_CACHE_MISSES,
            PERF_COUNT_HW_CPU_CYCLES,
            PERF_COUNT_HW_INSTRUCTIONS
        });

        stats.start();

        sleep(5);

        stats.stop();

        const auto cache_references = stats.value<>(PERF_COUNT_HW_CACHE_REFERENCES);
        const auto cache_misses = stats.value<>(PERF_COUNT_HW_CACHE_MISSES);
        const double cache_misses_rate = cache_references > 0 ? (double(cache_misses) / cache_references) : 0;

        std::cout << "   cache-references  = " << cache_references << '\n';
        std::cout << "   cache-misses      = " << cache_misses << " (" << cache_misses_rate << ")\n";
        std::cout << "   cpu-cycles        = " << stats.value<>(PERF_COUNT_HW_CPU_CYCLES) << '\n';
        std::cout << "   instructions      = " << stats.value<>(PERF_COUNT_HW_INSTRUCTIONS) << '\n';

    } catch (const std::exception& e) {
        std::cout << "ERROR: " << e.what() << '\n';
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
