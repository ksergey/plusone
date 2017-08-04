/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_signal_120717221550
#define KSERGEY_signal_120717221550

#include <cstring>
#include <plusone/compiler.hpp>

namespace plusone {
namespace detail {

/* Fill sigaction struct */
__force_inline void init_sigaction(struct ::sigaction& sa, signal::handler_type handler, int flags = 0)
{
    std::memset(&sa, 0, sizeof(sa));
    ::sigemptyset(&sa.sa_mask);
    sa.sa_flags = flags;
    sa.sa_handler = handler;
}

} /* namespace detail */

__force_inline void signal::set_handler(int signal_no, handler_type handler, int flags) noexcept
{
    struct ::sigaction sa;
    detail::init_sigaction(sa, handler, flags);
    ::sigaction(signal_no, &sa, nullptr);
}

__force_inline signal::handler_type signal::get_handler(int signal_no) noexcept
{
    struct ::sigaction sa;
    ::sigaction(signal_no, nullptr, &sa);
    return sa.sa_handler;
}

__force_inline void signal::kill(pid_t process_id, int signal_no) noexcept
{
    ::kill(process_id, signal_no);
}

__force_inline void signal::raise(int signal_no) noexcept
{
    ::raise(signal_no);
}

__force_inline void signal::set_alarm_timer(std::time_t interval) noexcept
{
    struct ::itimerval val;
    std::memset(&val, 0, sizeof(val));
    val.it_value.tv_sec = interval;
    ::setitimer(ITIMER_REAL, &val, nullptr);
}

} /* namespace plusone */

#endif /* KSERGEY_signal_120717221550 */
