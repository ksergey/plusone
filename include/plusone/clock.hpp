/*
 * Copyright (c) 2016-2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef MADLIFE_clock_091116011342_MADLIFE
#define MADLIFE_clock_091116011342_MADLIFE

#include <ctime>
#include <cstdint>
#include "compiler.hpp"

namespace plusone {

/** nanosecond period */
struct ns
{
    static constexpr uint64_t ratio = 1u;
    static constexpr uint64_t in_sec = 1000000000ul / ratio;
};

/** microsecond period */
struct us
{
    static constexpr uint64_t ratio = 1000ul;
    static constexpr uint64_t in_sec = 1000000000ul / ratio;
};

/** millisecond period */
struct ms
{
    static constexpr uint64_t ratio = 1000000ul;
    static constexpr uint64_t in_sec = 1000000000ul / ratio;
};

/** second period */
struct sec
{
    static constexpr uint64_t ratio = 1000000000ul;
    static constexpr uint64_t in_sec = 1000000000ul / ratio;
};

template< class PeriodT >
__force_inline uint64_t clock_now(clockid_t clock_id)
{
    timespec ts;
    clock_gettime(clock_id, &ts);
    return ts.tv_sec * PeriodT::in_sec + ts.tv_nsec / PeriodT::ratio;
}

/** return monotonic time */
template< class PeriodT >
__force_inline uint64_t clock_mono()
{ return clock_now< PeriodT >(CLOCK_MONOTONIC); }

/** return system-wide current time */
template< class PeriodT >
__force_inline uint64_t clock_time()
{ return clock_now< PeriodT >(CLOCK_REALTIME); }

/** return system-wide current time faster but less precise */
template< class PeriodT >
__force_inline uint64_t fastclock_time()
{ return clock_now< PeriodT >(CLOCK_REALTIME_COARSE); }

} /* namespace plusone */

#endif /* MADLIFE_clock_091116011342_MADLIFE */
