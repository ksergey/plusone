/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_optional_120218182806
#define KSERGEY_optional_120218182806

#include <experimental/optional>

namespace plusone {

using std::experimental::optional;

using std::experimental::in_place_t;
using std::experimental::in_place;

using std::experimental::nullopt_t;
using std::experimental::nullopt;

using std::experimental::bad_optional_access;

using std::experimental::make_optional;

} /* namespace plusone */

namespace std {

using std::experimental::optional;

using std::experimental::in_place_t;
using std::experimental::in_place;

using std::experimental::nullopt_t;
using std::experimental::nullopt;

using std::experimental::bad_optional_access;

using std::experimental::make_optional;

} /* namespace std */

#endif /* KSERGEY_optional_120218182806 */
