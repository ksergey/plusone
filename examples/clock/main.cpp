/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#include <cstdlib>
#include <exception>
#include <iostream>
#include <plusone/clock.hpp>

int main(int argc, char* argv[])
{
    try {
        unsigned count = 100000000;
        auto start = plusone::rdtsc();
        for (unsigned i = 0; i < count; ++i) {
            auto res = plusone::clock_time< plusone::ns >();
        }
        auto stop = plusone::rdtsc();
        std::cout << "total=" << (stop - start) << " per-frame=" << ((stop - start) / count) << '\n';
    } catch (const std::exception& e) {
        std::cout << "ERROR: " << e.what() << '\n';
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
