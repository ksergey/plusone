/*
 * Copyright (c) 2016-2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef MADLIFE_signal_101116205803_MADLIFE
#define MADLIFE_signal_101116205803_MADLIFE

#include <functional>
#include <initializer_list>
#include <plusone/signal_base.hpp>

namespace plusone {

struct signal
    : signal_base
{
    /**
     * Set signal handler
     * @param[in] signal_no is handler signal number
     * @param[in] handler is handler function
     * @param[in] flags is handler flags
     */
    static void set_handler(int signal_no, handler_type handler, int flags = 0) noexcept;

    /**
     * Get signal handler
     * @param[in] signal_no is handler signal number
     * @return signal handler function
     */
    static handler_type get_handler(int signal_no) noexcept;

    /** Send signal to process */
    static void kill(pid_t process_id, int signal_no = SIGTERM) noexcept;

    /** Send signal to current process */
    static void raise(int signal_no = SIGTERM) noexcept;
};

} /* namespace plusone */

#include <plusone/impl/signal.ipp>

#endif /* MADLIFE_signal_101116205803_MADLIFE */
