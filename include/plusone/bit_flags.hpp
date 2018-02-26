/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_bit_flags_260218120609
#define KSERGEY_bit_flags_260218120609

#include <type_traits>
#include <initializer_list>

namespace plusone {

/// Bit flags
template< class T >
class bit_flags
{
private:
    using storage = typename std::underlying_type< T >::type;
    static_assert( std::is_integral< storage >(), "" );

    storage value_{0};

public:
    constexpr bit_flags(const bit_flags&) = default;
    constexpr bit_flags& operator=(const bit_flags&) = default;
    constexpr bit_flags(bit_flags&&) noexcept = default;
    constexpr bit_flags& operator=(bit_flags&&) noexcept = default;

    constexpr bit_flags() = default;

    constexpr bit_flags(T flag)
        : value_{static_cast< storage >(flag)}
    {}

    constexpr bit_flags(std::initializer_list< T > flags)
    {
        for (auto& flag: flags) {
            add(flag);
        }
    }

    constexpr storage value() const noexcept
    {
        return value_;
    }

    constexpr bool is_set(T flag) const noexcept
    {
        auto flag_raw = static_cast< storage >(flag);
        return flag_raw == (value_ & flag_raw);
    }

    constexpr bit_flags& add(T flag) noexcept
    {
        value_ |= static_cast< storage >(flag);
        return *this;
    }

    constexpr bit_flags& remove(T flag) noexcept
    {
        value_ &= ~static_cast< storage >(flag);
        return *this;
    }

    constexpr bit_flags& toggle(T flag) noexcept
    {
        value_ ^= static_cast< storage >(flag);
        return *this;
    }

    constexpr bit_flags& clear() noexcept
    {
        value_ = 0;
        return *this;
    }

    constexpr bool operator==(const bit_flags& flags) const noexcept
    {
        return value_ == flags.value_;
    }

    constexpr bool operator!=(const bit_flags& flags) const noexcept
    {
        return !(*this == flags);
    }

    constexpr bool operator==(T flag) const noexcept
    {
        return value_ == static_cast< storage >(flag);
    }

    constexpr bool operator!=(T flag) const noexcept
    {
        return !(*this == flag);
    }
};

} /* namespace plusone */

#endif /* KSERGEY_bit_flags_260218120609 */
