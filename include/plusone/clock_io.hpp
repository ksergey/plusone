/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_clock_io_210317223431
#define KSERGEY_clock_io_210317223431

#include <iostream>
#include <iomanip>
#include "clock.hpp"

namespace plusone {
namespace detail {

/* Period output chars count */
template< class PeriodT >
constexpr std::size_t io_clock_period_width = 0;
template<>
constexpr std::size_t io_clock_period_width< ns > = 9;
template<>
constexpr std::size_t io_clock_period_width< us > = 6;
template<>
constexpr std::size_t io_clock_period_width< ms > = 3;

/** Wrapper around timestamp */
template< class PeriodT, class OutPrecisionT, bool UTC >
struct io_clock_wrapper
{
    /** UNIX time since EPOCH in PeriodT */
    std::uint64_t value;
};

/** time_t to struct tm */
template< bool UTC >
__force_inline void get_tm(time_t value, struct tm* tm)
{ localtime_r(&value, tm); }
template<>
__force_inline void get_tm< true >(time_t value, struct tm* tm)
{ gmtime_r(&value, tm); }

/** Format sub-second part */
template< class OutPrecisionT >
__force_inline void put_subsec(std::ostream& os, unsigned value_ns)
{
    os << '.'
        << std::setfill('0') << std::setw(io_clock_period_width< OutPrecisionT >)
        << (value_ns / OutPrecisionT::ratio)
        << std::setfill(' ') << std::setw(0);
}
template<>
__force_inline void put_subsec< sec >(std::ostream& os, unsigned value_ns)
{}

} /* namespace detail */

/** Clock format function */
template< class PeriodT, class OutPrecisionT, bool UTC >
__force_inline std::ostream& operator<<(std::ostream& os,
        const detail::io_clock_wrapper< PeriodT, OutPrecisionT, UTC >& value)
{
    struct tm tm;
    detail::get_tm< UTC >(value.value / PeriodT::in_sec, &tm);
    unsigned value_ns = (value.value % PeriodT::in_sec) * PeriodT::ratio;
    os << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    detail::put_subsec< OutPrecisionT >(os, value_ns);
    return os;
}

/** Wrap value */
template< class PeriodT = ns, class OutPrecisionT = ns, bool UTC = true >
__force_inline detail::io_clock_wrapper< PeriodT, OutPrecisionT, UTC > clock_fmt(std::uint64_t value) noexcept
{ return detail::io_clock_wrapper< PeriodT, OutPrecisionT, UTC >{value}; }

} /* namespace plusone */

#endif /* KSERGEY_clock_io_210317223431 */
