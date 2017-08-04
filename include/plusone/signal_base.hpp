/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_signal_base_040817112518
#define KSERGEY_signal_base_040817112518

#include <signal.h>

namespace plusone {

struct signal_base
{
    /** Signal handler type */
    typedef void (*handler_type)(int);

    /** Default signal handler function */
    static const handler_type default_handler;

    /** Ignore signal handler function */
    static const handler_type ignore_handler;
};

} /* namespace plusone */

#include <plusone/impl/signal_base.ipp>

#endif /* KSERGEY_signal_base_040817112518 */
