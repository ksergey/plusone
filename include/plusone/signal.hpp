/*
 * Copyright (c) 2016-2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef MADLIFE_signal_101116205803_MADLIFE
#define MADLIFE_signal_101116205803_MADLIFE

#include <csignal>
#include <cstring>

namespace plusone {

/** signal callback function */
typedef void (*signal_handler)(int sig);

/** change signal handler */
inline void setup_signal(int sig, signal_handler cb)
{
    struct sigaction sa{};
    std::memset(&sa, 0, sizeof(sa));
    sa.sa_handler = cb;
    ::sigaction(sig, &sa, nullptr);
}

/** change signal handler for set of signals */
inline void setup_signal(std::initializer_list< int > sigs, signal_handler cb)
{
    for (auto sig: sigs) {
        setup_signal(sig, cb);
    }
}

/** reset to default signal handler */
inline void reset_signal(int sig)
{
    setup_signal(sig, SIG_DFL);
}

/** reset to default signal handler for set of signals */
inline void reset_signal(std::initializer_list< int > sigs)
{
    for (auto sig: sigs) {
        reset_signal(sig);
    }
}

} /* namespace plusone */

#endif /* MADLIFE_signal_101116205803_MADLIFE */
