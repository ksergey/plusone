/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_process_040817122959
#define KSERGEY_process_040817122959

#include <plusone/process_base.hpp>

namespace plusone {

/** Process control interface */
struct process
    : process_base
{
    /**
     * Install process signal handlers.
     * The function also reset running flag to true
     */
    static void install_signal_handlers(const process::options& options = process::options{}) noexcept;

    /** Set default handlers */
    static void uninstall_signal_handlers() noexcept;

    /** Return true if process still running state */
    static bool running() noexcept;

    /** Stop execution process */
    static void shutdown() noexcept;
};

} /* namespace plusone */

#include <plusone/impl/process.ipp>

#endif /* KSERGEY_process_040817122959 */