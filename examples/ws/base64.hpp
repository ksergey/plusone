/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_base64_121017184149
#define KSERGEY_base64_121017184149

#include <array>
#include <iterator>
#include <type_traits>
#include <plusone/string_view.hpp>
#include <plusone/compiler.hpp>

namespace app {

template< class InputIterator, class OutputIterator >
OutputIterator base64_encode(InputIterator begin, InputIterator end, OutputIterator out);

/* TODO: base64_decode. */

namespace detail {

using plusone::string_view;
using namespace plusone::string_view_literals;

static constexpr string_view base64_chars =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789+/"sv;

} /* namespace detail */

template< class InputIterator, class OutputIterator >
__force_inline OutputIterator base64_encode(InputIterator begin, InputIterator end, OutputIterator out)
{
    static_assert( sizeof(decltype(*begin)) == 1, "" );

    std::array< std::uint8_t, 3 > array3;
    std::array< std::uint8_t, 4 > array4;
    std::size_t i = 0;

    while (begin != end) {
        array3[i++] = *(begin++);
        if (i == 3) {
            array4[0] = (array3[0] & 0xfc) >> 2;
            array4[1] = ((array3[0] & 0x03) << 4) + ((array3[1] & 0xf0) >> 4);
            array4[2] = ((array3[1] & 0x0f) << 2) + ((array3[2] & 0xc0) >> 6);
            array4[3] = array3[2] & 0x3f;
            for (auto c: array4) {
                *out++ = detail::base64_chars[c];
            }
            i = 0;
        }
    }

    if (i > 0) {
        for (std::size_t j = i; j < 3; ++j) {
            array3[j] = '\0';
        }

        array4[0] = (array3[0] & 0xfc) >> 2;
        array4[1] = ((array3[0] & 0x03) << 4) + ((array3[1] & 0xf0) >> 4);
        array4[2] = ((array3[1] & 0x0f) << 2) + ((array3[2] & 0xc0) >> 6);
        array4[3] = array3[2] & 0x3f;

        for (std::size_t j = 0; j < i + 1; ++j) {
            *out++ = detail::base64_chars[array4[j]];
        }

        while (i++ < 3) {
            *out++ = '=';
        }
    }

    return out;
}

} /* namespace app */

#endif /* KSERGEY_base64_121017184149 */
