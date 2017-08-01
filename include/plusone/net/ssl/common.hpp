/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_common_020817010520
#define KSERGEY_common_020817010520

#include <plusone/exception.hpp>

namespace plusone {
namespace net {
namespace ssl {

/** SSL exception type */
using error = plusone::tagged_exception< struct ssl_error_tag >;

} /* namespace ssl */
} /* namespace net */
} /* namespace plusone */

#endif /* KSERGEY_common_020817010520 */
