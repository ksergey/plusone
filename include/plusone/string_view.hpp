/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_string_view_160817110650
#define KSERGEY_string_view_160817110650

#ifdef __has_include
#   if (__has_include(<string_view>) && __cplusplus > 201402L)
#       include <string_view>
namespace plusone {
    using std::basic_string_view;
    namespace string_view_literals = std::string_view_literals;
} /* namespace plusone */
#   elif (__has_include(<experimental/string_view>) && __cplusplus >= 201402L)
#       include <experimental/string_view>
namespace plusone {
    using std::experimental::basic_string_view;
    namespace string_view_literals = std::experimental::string_view_literals;
} /* namespace plusone */
#   else
#       error "Missing <string_view>"
#   endif
#endif /* __has_include */

namespace plusone {

using string_view = basic_string_view< char >;
using wstring_view = basic_string_view< wchar_t >;

} /* namespace plusone */

#endif /* KSERGEY_string_view_160817110650 */
