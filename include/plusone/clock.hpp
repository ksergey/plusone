/*
 * Copyright (c) 2016-2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef MADLIFE_clock_091116011342_MADLIFE
#define MADLIFE_clock_091116011342_MADLIFE

#include <cstdint>

namespace plusone {

/** Nanosecond period */
struct ns;

/** Microsecond period */
struct us;

/** Millisecond period */
struct ms;

/** Second period */
struct sec;

/**
 * Return current time
 * @param[in] clock_id - clock is (@see man clock_gettime)
 */
template< class PeriodT >
std::uint64_t clock_now(clockid_t clock_id) noexcept;

/** Return monotonic time */
template< class PeriodT >
std::uint64_t clock_mono() noexcept;

/** Return system-wide current time */
template< class PeriodT >
std::uint64_t clock_time() noexcept;

/** Return system-wide current time faster but less precise */
template< class PeriodT >
std::uint64_t fastclock_time() noexcept;

/** Return clock cycles count from CPU */
std::uint64_t rdtsc() noexcept;

} /* namespace plusone */

#include "impl/clock.ipp"

#endif /* MADLIFE_clock_091116011342_MADLIFE */
