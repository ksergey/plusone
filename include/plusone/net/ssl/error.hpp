/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_error_010817231503
#define KSERGEY_error_010817231503

#include <system_error>

namespace plusone {
namespace net {
namespace ssl {
namespace error {

/** Return SSL error category */
const std::error_category& get_ssl_category();

} /* namespace error */

/** Throw SSL error */
void throw_error(int code);

} /* namespace ssl */
} /* namespace net */
} /* namespace plusone */

#include <plusone/net/ssl/impl/error.ipp>

#endif /* KSERGEY_error_010817231503 */
