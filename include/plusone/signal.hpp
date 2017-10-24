/*
 * Copyright (c) 2016-2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef MADLIFE_signal_101116205803_MADLIFE
#define MADLIFE_signal_101116205803_MADLIFE

#include <signal.h>
#include <ctime>

namespace plusone {

/** Signal handler. */
struct signal
{
    /** Signal native handler type. */
    typedef void (*handler_type)(int);

    /**
     * Set signal handler.
     * @param[in] signal_no Signal number.
     * @param[in] handler Callback.
     * @param[in] flags Handler flags.
     */
    static void set_handler(int signal_no, handler_type handler, int flags = 0) noexcept;

    /** @overload */
    template< class F >
    static void set_handler(int signal_no, F&& func, int flags = 0);

    /** @overload */
    template< class F >
    static void set_handler(std::initializer_list< int > signals, F&& func, int flags = 0);

    /**
     * Get signal handler.
     * @param[in] signal_no is handler signal number.
     * @return signal handler function.
     */
    static handler_type get_handler(int signal_no) noexcept;

    /** Send signal to process. */
    static void kill(pid_t process_id, int signal_no = SIGTERM) noexcept;

    /** Send signal to current process. */
    static void raise(int signal_no = SIGTERM) noexcept;

    /**
     * Set SIGALRM timer.
     * Call set_timer(0) for cancel timer.
     */
    static void set_alarm_timer(std::time_t interval) noexcept;
};

} /* namespace plusone */

#include <plusone/impl/signal.ipp>

#endif /* MADLIFE_signal_101116205803_MADLIFE */
