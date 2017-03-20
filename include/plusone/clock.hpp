/*
 * Copyright (c) 2016-2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef MADLIFE_clock_091116011342_MADLIFE
#define MADLIFE_clock_091116011342_MADLIFE

#include <ctime>
#include <cstdint>
#include "compiler.hpp"

namespace plusone {

/** Nanosecond period */
struct ns
{
    static constexpr std::uint64_t ratio = 1u;
    static constexpr std::uint64_t in_sec = 1000000000ul / ratio;
};

/** Microsecond period */
struct us
{
    static constexpr std::uint64_t ratio = 1000ul;
    static constexpr std::uint64_t in_sec = 1000000000ul / ratio;
};

/** Millisecond period */
struct ms
{
    static constexpr std::uint64_t ratio = 1000000ul;
    static constexpr std::uint64_t in_sec = 1000000000ul / ratio;
};

/** Second period */
struct sec
{
    static constexpr std::uint64_t ratio = 1000000000ul;
    static constexpr std::uint64_t in_sec = 1000000000ul / ratio;
};

/**
 * Return current time
 * @param[in] clock_id - clock is (@see man clock_gettime)
 */
template< class PeriodT >
__force_inline std::uint64_t clock_now(clockid_t clock_id)
{
    timespec ts;
    clock_gettime(clock_id, &ts);
    return ts.tv_sec * PeriodT::in_sec + ts.tv_nsec / PeriodT::ratio;
}

/** Return monotonic time */
template< class PeriodT >
__force_inline std::uint64_t clock_mono()
{ return clock_now< PeriodT >(CLOCK_MONOTONIC); }

/** Return system-wide current time */
template< class PeriodT >
__force_inline std::uint64_t clock_time()
{ return clock_now< PeriodT >(CLOCK_REALTIME); }

/** Return system-wide current time faster but less precise */
template< class PeriodT >
__force_inline std::uint64_t fastclock_time()
{ return clock_now< PeriodT >(CLOCK_REALTIME_COARSE); }

} /* namespace plusone */

#endif /* MADLIFE_clock_091116011342_MADLIFE */
