#pragma once

#include <cstdint>
#include "math.hpp"

namespace plusone {

/** Parse positive decimal8 string as number 
 E.g. : 2.1458e2, 22.1458
 */
std::int64_t parse_positive_decimal8(const string_view& str);

/** Parse decimal8 string as number 
 E.g. : -2.1458e2, 22.1458
 */
std::int64_t parse_decimal8(const string_view& str);

} /* namespace plusone */

#include <plusone/impl/parse.ipp>
