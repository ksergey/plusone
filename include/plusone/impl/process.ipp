/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_process_040817123016
#define KSERGEY_process_040817123016

#include <atomic>
#include <plusone/signal.hpp>
#include <plusone/compiler.hpp>
#include <plusone/expect.hpp>
#include <plusone/cacheline_padded.hpp>

namespace plusone {
namespace detail {

__force_inline process::options& process_options() noexcept
{
    static process::options value;
    return value;
}

__force_inline std::atomic_bool& process_running() noexcept
{
    static cacheline_padded< std::atomic_bool > value;
    return *value;
}

__force_inline static void process_signal_handler(int signum)
{
    switch (signum) {
        case SIGINT:
        case SIGTERM:
        case SIGQUIT:
            {
                process_running().store(false);
                if (process_options().force_kill) {
                    signal::set_alarm_timer(process_options().force_kill_delay);
                }
            }
            break;

        case SIGALRM:
            {
                __expect( process_options().force_kill );
                __expect( process_running().load() == false );

                signal::raise(process_options().force_kill_use_sigabrt ? SIGABRT : SIGKILL);
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
    signal::set_alarm_timer(0);

    /* Update process options */
    detail::process_options() = options;

    /* Now running active */
    detail::process_running().store(true);

    /* Setup handlers */
    signal::set_handler(SIGINT, detail::process_signal_handler);
    signal::set_handler(SIGTERM, detail::process_signal_handler);
    signal::set_handler(SIGQUIT, detail::process_signal_handler);
    signal::set_handler(SIGALRM, detail::process_signal_handler);
}

__force_inline void process::uninstall_signal_handlers() noexcept
{
    signal::set_alarm_timer(0);

    detail::process_running().store(false);

    signal::set_handler(SIGINT, SIG_DFL);
    signal::set_handler(SIGTERM, SIG_DFL);
    signal::set_handler(SIGQUIT, SIG_DFL);
    signal::set_handler(SIGALRM, SIG_DFL);
}

__force_inline bool process::running() noexcept
{
    return detail::process_running().load();
}

__force_inline void process::shutdown() noexcept
{
    signal::raise(SIGTERM);
}

} /* namespace plusone */

#endif /* KSERGEY_process_040817123016 */
