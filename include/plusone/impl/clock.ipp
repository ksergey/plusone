/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_clock_120717232211
#define KSERGEY_clock_120717232211

#include <dlfcn.h>
#include <plusone/compiler.hpp>

namespace plusone {

struct ns
{
    static constexpr std::uint64_t ratio = 1u;
    static constexpr std::uint64_t in_sec = 1000000000ul / ratio;
};

struct us
{
    static constexpr std::uint64_t ratio = 1000ul;
    static constexpr std::uint64_t in_sec = 1000000000ul / ratio;
};

struct ms
{
    static constexpr std::uint64_t ratio = 1000000ul;
    static constexpr std::uint64_t in_sec = 1000000000ul / ratio;
};

struct sec
{
    static constexpr std::uint64_t ratio = 1000000000ul;
    static constexpr std::uint64_t in_sec = 1000000000ul / ratio;
};

#if defined( PLUSONE_VDSO_CLOCK_GETTIME )

/* Init with default behaviour. */
int (*clock_gettime)(clockid_t, timespec* ts) = &::clock_gettime;

class vdso_clock_initializer
{
private:
    void* handle_;

public:
    __force_inline vdso_clock_initializer() noexcept
    {
        handle_ = dlopen("linux-vdso.so.1", RTLD_LAZY | RTLD_LOCAL | RTLD_NOLOAD);
        if (!handle_) {
            return ;
        }

        void* p = dlsym(handle_, "__vdso_clock_gettime");
        if (p) {
            plusone::clock_gettime = reinterpret_cast< int (*)(clockid_t, timespec*) >(p);
        }
    }

    __force_inline ~vdso_clock_initializer() noexcept
    {
        if (handle_) {
            plusone::clock_gettime = &::clock_gettime;
            dlclose(handle_);
        }
    }
};

static const vdso_clock_initializer clock_initializer;

#endif /* defined( PLUSONE_VDSO_CLOCK_GETTIME ) */

template< class PeriodT >
__force_inline std::uint64_t clock_now(clockid_t clock_id) noexcept
{
    timespec ts;
    clock_gettime(clock_id, &ts);
    return ts.tv_sec * PeriodT::in_sec + ts.tv_nsec / PeriodT::ratio;
}

template< class PeriodT >
__force_inline std::uint64_t clock_mono() noexcept
{
    return clock_now< PeriodT >(CLOCK_MONOTONIC);
}

template< class PeriodT >
__force_inline std::uint64_t clock_time() noexcept
{
    return clock_now< PeriodT >(CLOCK_REALTIME);
}

template<>
__force_inline std::uint64_t clock_time< sec >() noexcept
{
    return time(nullptr);
}

template< class PeriodT >
__force_inline std::uint64_t fastclock_time() noexcept
{
    return clock_now< PeriodT >(CLOCK_REALTIME_COARSE);
}

__force_inline std::uint64_t rdtsc() noexcept
{
    unsigned reslo, reshi;
    __asm__ __volatile__  ("rdtsc\n" : "=a" (reslo), "=d" (reshi));
    return (std::uint64_t(reshi) << 32) | reslo;
}

} /* namespace plusone */

#endif /* KSERGEY_clock_120717232211 */
