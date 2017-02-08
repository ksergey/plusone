/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_time_fmt_080217112836
#define KSERGEY_time_fmt_080217112836

#include <iostream>
#include <iomanip>
#include <plusone/clock.hpp>

namespace plusone {

struct time_fmt
{
    std::uint64_t timestamp;
};

inline std::ostream& operator<<(std::ostream& os, const time_fmt& value)
{
    time_t time = value.timestamp / 1000000000lu;
    unsigned ns = value.timestamp % 1000000000lu;
    struct tm tm;
    gmtime_r(&time, &tm);

    std::ios::fmtflags flags(os.flags());
    os << std::setfill('0')
        << std::setw(4) << (tm.tm_year + 1900) << '-'
        << std::setw(2) << (tm.tm_mon + 1) << '-'
        << std::setw(2) << tm.tm_mday << ' '
        << std::setw(2) << tm.tm_hour << ':'
        << std::setw(2) << tm.tm_min << ':'
        << std::setw(2) << tm.tm_sec << '.'
        << std::setw(9) << ns;
    os << std::setfill(' ');
    os.flags(flags);

    return os;
}

} /* namespace plusone */

#endif /* KSERGEY_time_fmt_080217112836 */
