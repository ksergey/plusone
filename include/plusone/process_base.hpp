/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_process_base_040817123208
#define KSERGEY_process_base_040817123208

#include <ctime>

namespace plusone {

/** Place for process constants, defines, etc... */
struct process_base
{
    /** Process signal options */
    struct options
    {
        /** Kill process with SIGKILL/SIGABRT after slight delay */
        bool force_kill{false};

        /** Delay interval before force kill */
        std::time_t force_kill_delay{5};

        /** Use SIGABRT instead of SIGKILL in case of force kill process */
        bool force_kill_use_sigabrt{false};
    };
};

} /* namespace plusone */

#endif /* KSERGEY_process_base_040817123208 */
