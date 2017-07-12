/*
 * Copyright (c) 2016-2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef MADLIFE_signal_101116205803_MADLIFE
#define MADLIFE_signal_101116205803_MADLIFE

#include <functional>
#include <initializer_list>

namespace plusone {

class signal
{
private:
    using callback_type = std::function< void (int sig) >;

    /* Callback function */
    static callback_type global_callback_;

public:
    /** Setup signal handler */
    template< class Callback >
    static void setup(std::initializer_list< int > sigs, Callback&& callback);

    /** Setup handler for SIGINT and SIGTERM signals */
    template< class Callback >
    static void setup(Callback&& callback);

private:
    /* Native signal handler func */
    static void native_signal_handler(int sig);
};

} /* namespace plusone */

#include "impl/signal.ipp"

#endif /* MADLIFE_signal_101116205803_MADLIFE */
