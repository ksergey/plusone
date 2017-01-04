/*
 * Copyright (c) 2016-2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_backport_051216162634
#define KSERGEY_backport_051216162634

#include <experimental/string_view>
#include <experimental/optional>

namespace plusone {

/** c++17 string_view class */
using std::experimental::string_view;

/** c++17 optional class */
using std::experimental::optional;
using std::experimental::make_optional;
using std::experimental::nullopt_t;
using std::experimental::nullopt;

} /* namespace plusone */

#endif /* KSERGEY_backport_051216162634 */
