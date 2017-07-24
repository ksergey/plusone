/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_signal_120717221550
#define KSERGEY_signal_120717221550

#include <csignal>
#include <cstring>
#include <plusone/compiler.hpp>

namespace plusone {

signal::callback_type signal::global_callback_{};

template< class Callback >
__force_inline void signal::setup(std::initializer_list< int > sigs, Callback&& callback)
{
    global_callback_ = callback;

    for (auto sig: sigs) {
        struct sigaction sa{};
        std::memset(&sa, 0, sizeof(sa));
        sa.sa_handler = native_signal_handler;
        ::sigaction(sig, &sa, nullptr);
    }
}

template< class Callback >
__force_inline void signal::setup(Callback&& callback)
{
    setup({SIGINT, SIGTERM}, std::forward< Callback >(callback));
}

__force_inline void signal::native_signal_handler(int sig)
{
    if (global_callback_) {
        global_callback_(sig);
    }
}

} /* namespace plusone */

#endif /* KSERGEY_signal_120717221550 */
