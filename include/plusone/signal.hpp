/*
 * Copyright (c) 2016-2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef MADLIFE_signal_101116205803_MADLIFE
#define MADLIFE_signal_101116205803_MADLIFE

#include <csignal>
#include <cstring>
#include "compiler.hpp"

namespace plusone {

/** Signal callback function type */
typedef void (*signal_handler)(int sig);

/** Set signal handler */
__force_inline void setup_signal(int sig, signal_handler cb)
{
    struct sigaction sa{};
    std::memset(&sa, 0, sizeof(sa));
    sa.sa_handler = cb;
    ::sigaction(sig, &sa, nullptr);
}

/** Set signal handler for set of signals */
__force_inline void setup_signal(std::initializer_list< int > sigs, signal_handler cb)
{
    for (auto sig: sigs) {
        setup_signal(sig, cb);
    }
}

/** Reset to default signal handler */
__force_inline void reset_signal(int sig)
{ setup_signal(sig, SIG_DFL); }

/** Reset to default signal handler for set of signals */
__force_inline void reset_signal(std::initializer_list< int > sigs)
{
    for (auto sig: sigs) {
        reset_signal(sig);
    }
}

} /* namespace plusone */

#endif /* MADLIFE_signal_101116205803_MADLIFE */
