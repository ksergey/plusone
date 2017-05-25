/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_common_250517185354
#define KSERGEY_common_250517185354

#include <curl/curl.h>
#include "../../compiler.hpp"
#include "../../exception.hpp"

namespace plusone {
namespace net {
namespace curl {

/** CURL exception type */
using error = tagged_exception< struct curl_tag >;

/** CURL initialize and cleanup in RAII style */
struct cleanup
{
    cleanup(const struct cleanup&) = delete;
    cleanup& operator=(const struct cleanup&) = delete;

    cleanup()
    { curl_global_init(CURL_GLOBAL_ALL); }

    ~cleanup()
    { curl_global_cleanup(); }
};

} /* namespace curl */
} /* namespace net */
} /* namespace plusone */

#endif /* KSERGEY_common_250517185354 */
