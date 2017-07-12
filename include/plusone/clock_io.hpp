/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_clock_io_210317223431
#define KSERGEY_clock_io_210317223431

#include "clock.hpp"

namespace plusone {

namespace detail {

template< class PeriodT, class OutPrecisionT, bool UTC >
struct io_clock_wrapper;

} /* namespace detail */

/** Clock format function */
template< class PeriodT, class OutPrecisionT, bool UTC >
std::ostream& operator<<(std::ostream& os,
        const detail::io_clock_wrapper< PeriodT, OutPrecisionT, UTC >& value);

/** Wrap value */
template< class PeriodT = ns, class OutPrecisionT = ns, bool UTC = true >
detail::io_clock_wrapper< PeriodT, OutPrecisionT, UTC > clock_fmt(std::uint64_t value) noexcept;

} /* namespace plusone */

#include "impl/clock_io.ipp"

#endif /* KSERGEY_clock_io_210317223431 */
