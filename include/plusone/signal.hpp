/*
 * Copyright (c) 2016-2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef MADLIFE_signal_101116205803_MADLIFE
#define MADLIFE_signal_101116205803_MADLIFE

#include <csignal>
#include <cstring>
#include <functional>
#include <initializer_list>
#include "compiler.hpp"

namespace plusone {

class signal
{
private:
    using callback_type = std::function< void (int sig) >;

    /* Callback function */
    static callback_type callback_;

public:
    /** Setup signal handler */
    template< class Callback >
    static __force_inline void setup(std::initializer_list< int > sigs, Callback&& callback)
    {
        callback_ = callback;

        for (auto sig: sigs) {
            struct sigaction sa{};
            std::memset(&sa, 0, sizeof(sa));
            sa.sa_handler = native_signal_handler;
            ::sigaction(sig, &sa, nullptr);
        }
    }

    /** Setup handler for SIGINT and SIGTERM signals */
    template< class Callback >
    static __force_inline void setup(Callback&& callback)
    { setup({SIGINT, SIGTERM}, std::forward< Callback >(callback)); }

private:
    /* Native signal handler func */
    static void native_signal_handler(int sig)
    {
        if (callback_) {
            callback_(sig);
        }
    }
};

signal::callback_type signal::callback_{};

} /* namespace plusone */

#endif /* MADLIFE_signal_101116205803_MADLIFE */
