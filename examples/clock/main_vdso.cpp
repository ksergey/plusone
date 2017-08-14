/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#include <cstdlib>
#include <exception>
#include <iostream>
#include <plusone/clock.hpp>
#include <plusone/exception.hpp>
#include <dlfcn.h>

std::int64_t (*clock_gettime_ns)(clockid_t);
int (*clock_gettime_1)(clockid_t, timespec*);

using plusone::throw_ex;

class initializer final
{
private:
    void* handle_{nullptr};

public:
    initializer(const initializer&) = delete;
    initializer& operator=(const initializer&) = delete;

    initializer()
    {
        handle_ = dlopen("linux-vdso.so.1", RTLD_LAZY | RTLD_LOCAL | RTLD_NOLOAD);
        if (!handle_) {
            throw_ex< std::runtime_error >("Failed to load linux vdso");
        }

        void* ptr = dlsym(handle_, "__vdso_clock_gettime");
        if (!ptr) {
            reset();
            throw_ex< std::runtime_error >("Failed to load __vdso_clock_gettime");
        }

        clock_gettime_1 = reinterpret_cast< int (*)(clockid_t, timespec*) >(ptr);
    }

    ~initializer()
    {
        reset();
    }

    void reset()
    {
        if (handle_) {
            dlclose(handle_);
            handle_ = nullptr;
        }
    }
};

template< class PeriodT >
std::uint64_t get_now(clockid_t clock_id) noexcept
{
    timespec ts;
    clock_gettime_1(clock_id, &ts);
    return ts.tv_sec * PeriodT::in_sec + ts.tv_nsec / PeriodT::ratio;
}

template< class PeriodT >
__force_inline std::uint64_t get_time() noexcept
{
    return get_now< PeriodT >(CLOCK_REALTIME);
}

int main(int argc, char* argv[])
{
    /* TODO: integrate this code into library */
    try {
        initializer init;

        unsigned count = 100000000;
        auto start = plusone::rdtsc();
        for (unsigned i = 0; i < count; ++i) {
            auto res = get_time< plusone::ns >();
        }
        auto stop = plusone::rdtsc();
        std::cout << "total=" << (stop - start) << " per-frame=" << ((stop - start) / count) << '\n';
    } catch (const std::exception& e) {
        std::cout << "ERROR: " << e.what() << '\n';
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
