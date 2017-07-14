/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_address_v4_120717235817
#define KSERGEY_address_v4_120717235817

namespace plusone {
namespace net {

__force_inline address_v4::address_v4() noexcept
{ data_.s_addr = 0; }

__force_inline address_v4::address_v4(unsigned long addr)
{
#if ULONG_MAX > 0xFFFFFFFF
    if (__unlikely(addr > 0xFFFFFFFF)) {
        throw address_error("Bad IPv4 address");
    }
#endif /* ULONG_MAX > 0xFFFFFFFF */

    data_.s_addr = htonl(addr);
}

__force_inline unsigned long address_v4::to_ulong() const noexcept
{
    return ntohl(data_.s_addr);
}

__force_inline std::string address_v4::to_string() const
{
    /* Placeholder for result */
    char addr_str[INET_ADDRSTRLEN];
    const char* addr = inet_ntop(AF_INET, &data_, addr_str, sizeof(addr_str));
    if (addr == nullptr) {
        return std::string();
    }
    return addr;
}

__force_inline address_v4 address_v4::from_string(const char* str)
{
    address_v4 result;
    if (inet_pton(AF_INET, str, &result.data_) != 1) {
        throw address_error("Convert to address from string error");
    }
    return result;
}

__force_inline address_v4 address_v4::from_string(const std::string& str)
{
    return from_string(str.c_str());
}

__force_inline bool address_v4::from_string(const char* str, address_v4& result)
{
    return inet_pton(AF_INET, str, &result.data_) == 1;
}

__force_inline bool address_v4::from_string(const std::string& str, address_v4& result)
{
    return from_string(str.c_str(), result);
}

__force_inline bool address_v4::is_loopback() const noexcept
{
    return (to_ulong() & 0xFF000000u) == 0x7F000000u;
}

__force_inline bool address_v4::is_unspecified() const noexcept
{
    return to_ulong() == 0;
}

__force_inline bool address_v4::is_class_a() const noexcept
{
    return (to_ulong() & 0x80000000u) == 0;
}

__force_inline bool address_v4::is_class_b() const noexcept
{
    return (to_ulong() & 0xC0000000u) == 0x80000000u;
}

__force_inline bool address_v4::is_class_c() const noexcept
{
    return (to_ulong() & 0xE0000000u) == 0xC0000000u;
}

__force_inline bool address_v4::is_multicast() const noexcept
{
    return (to_ulong() & 0xF0000000u) == 0xE0000000u;
}

__force_inline address_v4 address_v4::any()
{
    return address_v4();
}

__force_inline address_v4 address_v4::loopback()
{
    return address_v4(0x7F000001);
}

} /* namespace net */
} /* namespace plusone */

#endif /* KSERGEY_address_v4_120717235817 */
