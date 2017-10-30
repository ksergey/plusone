#pragma once

#include <plusone/expect.hpp>
#include <plusone/compiler.hpp>
#include <plusone/math.hpp>
#include <plusone/exception.hpp>

namespace plusone {
namespace detail {

struct parse_result
{
    std::uint64_t value{0};
    bool sign{true};
    unsigned count{0};
};

__force_inline void parse_reverse_while_digit(const char* data, std::size_t size, parse_result& result) noexcept
{
    result = {};

    const char* cur = data + size;
    for (; result.count < size; ++result.count) {
        --cur;
        if (__unlikely((*cur < '0') || (*cur > '9'))) {
            if (*cur == '-') {
                result.sign = false;
                ++result.count;
            } else if (*cur == '+') {
                /* sign already true. */
                ++result.count;
            }
            break;
        } else {
            result.value += static_cast< std::uint8_t >(*cur - '0') * pow10::get(result.count);
        }
    }
}

__force_inline std::int64_t apply_sign(std::int64_t value, bool sign) noexcept
{
    return sign ? value : -value;
}

__force_inline std::int64_t apply_pow(std::int64_t value, unsigned e) noexcept
{
    return value * pow10::get(e);
}

__force_inline constexpr std::int64_t apply(std::int64_t value, bool sign, unsigned p) noexcept
{
    auto res = value * pow10::get(p);
    return sign ? res : -res;
}

__force_inline std::pair< std::int64_t, bool > parse_decimal8(const char* data, std::size_t size) noexcept
{
    static constexpr auto error = std::make_pair< std::int64_t, bool >(0, false);

    if (__unlikely(size == 0)) {
        return error;
    }

    parse_result result;
    parse_reverse_while_digit(data, size, result);
    if (size == result.count) {
        return {apply(result.value, result.sign, 8), true};
    }
    size -= (result.count + 1);

    if (__likely(data[size] == '.')) {
        if (__unlikely(result.count > 8 || !result.sign)) {
            return error;
        }
        auto short_part = apply(result.value, true, 8 - result.count);

        parse_reverse_while_digit(data, size, result);
        if (__likely(size == result.count)) {
            return {apply_sign(apply_pow(result.value, 8) + short_part, result.sign), true};
        }
    } else if (__likely(data[size] == 'e')) {
        int p = result.sign ? result.value : -result.value;

        parse_reverse_while_digit(data, size, result);
        if (size == result.count) {
            p += 8;
            if (__unlikely(p >= static_cast< int >(pow10::table_size) || p < 0)) {
                return error;
            }
            return {apply(result.value, result.sign, p), true};
        }

        size -= (result.count + 1);

        if (__unlikely(data[size] != '.')) {
            return error;
        }

        if (__unlikely(!result.sign)) {
            return error;
        }

        p = p - result.count + 8;
        if (__unlikely(p < 0 || p >= static_cast< int >(pow10::table_size))) {
            return error;
        }

        auto short_part = apply(result.value, true, p);
        p += result.count;

        if (__unlikely(p >= static_cast< int >(pow10::table_size))) {
            return error;
        }

        parse_reverse_while_digit(data, size, result);
        if (__unlikely(size != result.count)) {
            return error;
        }

        return {apply_sign(apply_pow(result.value, p) + short_part, result.sign), true};
    }

    return error;
}
} /* namespace detail */

__force_inline std::int64_t parse_positive_decimal8(const string_view& str)
{
    auto result = detail::parse_decimal8(str.data(), str.size());
    if (__unlikely(!result.second || result.first < 0)) {
        throw_ex< std::runtime_error >("Not a positive decimal8 ({})", str);
    }
    return result.first;
}

__force_inline std::int64_t parse_decimal8(const string_view& str)
{
    auto result = detail::parse_decimal8(str.data(), str.size());
    if (__unlikely(!result.second)) {
        throw_ex< std::runtime_error >("Not a decimal8 ({})", str);
    }
    return result.first;
}

} /* namespace plusone */
