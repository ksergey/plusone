/*
 * Copyright (c) 2016-2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef MADLIFE_address_v4_051116231424_MADLIFE
#define MADLIFE_address_v4_051116231424_MADLIFE

#include "common.hpp"

namespace plusone {
namespace net {

/** IPv4 address object */
class address_v4 final
{
private:
    /* Address data */
    in_addr data_;

public:
    /** Construct default address (unspecified) */
    __force_inline address_v4()
    { data_.s_addr = 0; }

    /** Construct address */
    __force_inline explicit address_v4(unsigned long addr)
    {
#if ULONG_MAX > 0xFFFFFFFF
        if (__unlikely(addr > 0xFFFFFFFF)) {
            throw address_error{"bad IPv4 address"};
        }
#endif /* ULONG_MAX > 0xFFFFFFFF */

        data_.s_addr = htonl(addr);
    }

    /** IPv4 address to unsigned long */
    __force_inline unsigned long to_ulong() const
    { return ntohl(data_.s_addr); }

    /** IPv4 address to string dot-notation */
    __force_inline std::string to_string() const
    {
        /* Placeholder for result */
        char addr_str[INET_ADDRSTRLEN];
        const char* addr = inet_ntop(AF_INET, &data_, addr_str, sizeof(addr_str));
        if (addr == nullptr) {
            return std::string();
        }
        return addr;
    }

    /** Convert address from const char* */
    __force_inline static address_v4 from_string(const char* str)
    {
        address_v4 result;
        if (inet_pton(AF_INET, str, &result.data_) != 1) {
            throw address_error{"Convert to address from string error"};
        }
        return result;
    }

    /** Convert address from std::string */
    __force_inline static address_v4 from_string(const std::string& str)
    { return from_string(str.c_str()); }

    /** Convert address from const char* */
    __force_inline static bool from_string(const char* str, address_v4& result)
    { return inet_pton(AF_INET, str, &result.data_) == 1; }

    /** Convert address from std::string */
    __force_inline static bool from_string(const std::string& str, address_v4& result)
    { return from_string(str.c_str(), result); }

    /** Return true if address is loopback */
    __force_inline bool is_loopback() const
    { return (to_ulong() & 0xFF000000u) == 0x7F000000u; }

    /** Return true if address is unspecified */
    __force_inline bool is_unspecified() const
    { return to_ulong() == 0; }

    /** Return true if address is a-class */
    __force_inline bool is_class_a() const
    { return (to_ulong() & 0x80000000u) == 0; }

    /** Return true if address is b-class */
    __force_inline bool is_class_b() const
    { return (to_ulong() & 0xC0000000u) == 0x80000000u; }

    /** Return true if address is c-class */
    __force_inline bool is_class_c() const
    { return (to_ulong() & 0xE0000000u) == 0xC0000000u; }

    /** Return true if address is multicast group */
    __force_inline bool is_multicast() const
    { return (to_ulong() & 0xF0000000u) == 0xE0000000u; }

    /** Construct unspecified address */
    __force_inline static address_v4 any()
    { return address_v4(); }

    /** Construct loopback address */
    __force_inline static address_v4 loopback()
    { return address_v4(0x7F000001); }
};

} /* namespace net */
} /* namespace plusone */

#endif /* MADLIFE_address_v4_051116231424_MADLIFE */
