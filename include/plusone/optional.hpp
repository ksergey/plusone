/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_optional_120218182806
#define KSERGEY_optional_120218182806

#ifdef __has_include
#   if (__has_include(<optional>) && __cplusplus > 201402L)
#       include <optional>
namespace plusone {

using std::optional;
using std::in_place_t;
using std::in_place;
using std::nullopt_t;
using std::nullopt;
using std::bad_optional_access;
using std::make_optional;

} /* namespace plusone */
#   elif (__has_include(<experimental/optional>) && __cplusplus >= 201402L)
#       include <experimental/optional>
namespace plusone {

using std::experimental::optional;
using std::experimental::in_place_t;
using std::experimental::in_place;
using std::experimental::nullopt_t;
using std::experimental::nullopt;
using std::experimental::bad_optional_access;
using std::experimental::make_optional;

} /* namespace plusone */
#   else
#       error "Missing <optional>"
#   endif
#endif /* __has_include */

#endif /* KSERGEY_optional_120218182806 */
