/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_context_base_010817185854
#define KSERGEY_context_base_010817185854

#include <plusone/exception.hpp>

namespace plusone {
namespace net {
namespace ssl {

/** The class used for defined various enums */
struct context_base
{
    /**
     * SSL methods.
     *
     * actually methods much more
     */
    enum method
    {
        sslv23,
        sslv23_client,
        sslv23_server
    };

    /** SSL context exception type */
    using error = plusone::tagged_exception< context_base >;
};

} /* namespace ssl */
} /* namespace net */
} /* namespace plusone */

#endif /* KSERGEY_context_base_010817185854 */
