/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_signal_120717221550
#define KSERGEY_signal_120717221550

#include <sys/time.h>
#include <cstring>
#include <functional>
#include <unordered_map>
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

/* Signal handler helper. */
class signal_handler
{
private:
    /* Callback type. */
    using callback_type = std::function< void (int sig) >;

    std::unordered_map< int, callback_type > storage_;

public:
    static signal_handler& instance()
    {
        static signal_handler handler;
        return handler;
    }

    static void handle(int signal_no)
    {
        instance()(signal_no);
    }

    template< class F >
    void set_handler(int signal_no, F&& callback)
    {
        storage_[signal_no] = callback;
    }

private:
    signal_handler() = default;

    /* Handle signal `sig` */
    void operator()(int signal_no) const
    {
        auto found = storage_.find(signal_no);
        if (__unlikely(found == storage_.end())) {
            /* This is exception */
            return;
        }
        found->second(signal_no);
    }
};

} /* namespace detail */

__force_inline void signal::set_handler(int signal_no, handler_type handler, int flags) noexcept
{
    struct ::sigaction sa;
    detail::init_sigaction(sa, handler, flags);
    ::sigaction(signal_no, &sa, nullptr);
}

template< class F >
__force_inline void signal::set_handler(int signal_no, F&& func, int flags)
{
    detail::signal_handler::instance().set_handler(signal_no, std::forward< F >(func));
    set_handler(signal_no, detail::signal_handler::handle, flags);
}

template< class F >
__force_inline void signal::set_handler(std::initializer_list< int > signals, F&& func, int flags)
{
    for (int signal_no: signals) {
        set_handler(signal_no, std::forward< F >(func), flags);
    }
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
