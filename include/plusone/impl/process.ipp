/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_process_040817123016
#define KSERGEY_process_040817123016

#include <sys/time.h>
#include <plusone/signal.hpp>
#include <plusone/compiler.hpp>
#include <plusone/expect.hpp>
#include <plusone/cacheline_padded.hpp>

namespace plusone {
namespace detail {

/* Options used inside process_signal_handler */
static process::options global_options;

/* Flag indicates process still running */
static plusone::cacheline_padded< sig_atomic_t > process_running_flag{0};

__force_inline static void setup_signal_alarm(std::time_t interval) noexcept
{
    struct ::itimerval itimerval;
    std::memset(&itimerval, 0, sizeof(itimerval));
    itimerval.it_value.tv_sec = interval;
    ::setitimer(ITIMER_REAL, &itimerval, nullptr);
}

/** Signal handler */
__force_inline static void process_signal_handler(int signum)
{
    switch (signum) {
        case SIGINT:
        case SIGTERM:
        case SIGQUIT:
            {
                *process_running_flag = 0;

                if (global_options.force_kill) {
                    setup_signal_alarm(global_options.force_kill_delay);
                }
            }
            break;

        case SIGALRM:
            {
                __expect( global_options.force_kill );
                __expect( *process_running_flag == 0 );

                signal::raise(global_options.force_kill_use_sigabrt ? SIGABRT : SIGKILL);
            }
            break;
        default:
            break;
    }
}

} /* namespace detail */

__force_inline void process::install_signal_handlers(const process::options& options) noexcept
{
    /* Cancel timer */
    detail::setup_signal_alarm(0);

    detail::global_options = options;
    *detail::process_running_flag = 1;

    signal::set_handler(SIGINT, detail::process_signal_handler);
    signal::set_handler(SIGTERM, detail::process_signal_handler);
    signal::set_handler(SIGQUIT, detail::process_signal_handler);
    signal::set_handler(SIGALRM, detail::process_signal_handler);
}

__force_inline bool process::running() noexcept
{
    return *detail::process_running_flag == 1;
}

} /* namespace plusone */

#endif /* KSERGEY_process_040817123016 */
