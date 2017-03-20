/*
 * Copyright (c) 2016-2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef MADLIFE_common_071116143044_MADLIFE
#define MADLIFE_common_071116143044_MADLIFE

#include <stdexcept>
#include "../compiler.hpp"
#include "../exception.hpp"
#include "./deps/json.hpp"

namespace plusone {
namespace serialization {

/* For convenience */
using json = nlohmann::json;

/* Serialization error classes */
using check_error = tagged_exception< struct check_tag >;
using policy_error = tagged_exception< struct policy_tag >;
using io_error = tagged_exception< struct io_tag >;

} /* namespace serialization */
} /* namespace plusone */

#endif /* MADLIFE_common_071116143044_MADLIFE */
