/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_clock_120717232211
#define KSERGEY_clock_120717232211

#include <ctime>
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

template< class PeriodT >
__force_inline std::uint64_t clock_now(clockid_t clock_id) noexcept
{
    timespec ts;
    ::clock_gettime(clock_id, &ts);
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
    __asm__ __volatile__  ("xorl %%eax,%%eax \n cpuid \n" ::: "%eax", "%ebx", "%ecx", "%edx");
    __asm__ __volatile__  ("rdtsc\n" : "=a" (reslo), "=d" (reshi));
    __asm__ __volatile__  ("xorl %%eax,%%eax \n cpuid \n" ::: "%eax", "%ebx", "%ecx", "%edx");
    return (std::uint64_t(reshi) << 32) | reslo;
}

} /* namespace plusone */

#endif /* KSERGEY_clock_120717232211 */
