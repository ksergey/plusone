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
    /* address data */
    in_addr data_;

public:
    /** construct default address (unspecified) */
    address_v4()
    { data_.s_addr = 0; }

    /** construct address */
    explicit address_v4(unsigned long addr)
    {
#if ULONG_MAX > 0xFFFFFFFF
        if (addr > 0xFFFFFFFF) {
            throw address_error{"bad IPv4 address"};
        }
#endif /* ULONG_MAX > 0xFFFFFFFF */

        data_.s_addr = htonl(addr);
    }

    /** IPv4 address to unsigned long */
    unsigned long to_ulong() const
    { return ntohl(data_.s_addr); }

    /** IPv4 address to string dot-notation */
    std::string to_string() const
    {
        /* placeholder for result */
        char addr_str[INET_ADDRSTRLEN];
        const char* addr = inet_ntop(AF_INET, &data_, addr_str, sizeof(addr_str));
        if (addr == nullptr) {
            return std::string();
        }
        return addr;
    }

    /** convert address from const char* */
    static address_v4 from_string(const char* str)
    {
        address_v4 result;
        if (inet_pton(AF_INET, str, &result.data_) != 1) {
            throw address_error{"failed to convert to address from string"};
        }
        return result;
    }

    /** convert address from std::string */
    static address_v4 from_string(const std::string& str)
    { return from_string(str.c_str()); }

    /** convert address from const char* */
    static bool from_string(const char* str, address_v4& result)
    { return inet_pton(AF_INET, str, &result.data_) == 1; }

    /** convert address from std::string */
    static bool from_string(const std::string& str, address_v4& result)
    { return from_string(str.c_str(), result); }

    /** return true if address is loopback */
    bool is_loopback() const
    { return (to_ulong() & 0xFF000000u) == 0x7F000000u; }

    /** return true if address is unspecified */
    bool is_unspecified() const
    { return to_ulong() == 0; }

    /** return true if address is a-class */
    bool is_class_a() const
    { return (to_ulong() & 0x80000000u) == 0; }

    /** return true if address is b-class */
    bool is_class_b() const
    { return (to_ulong() & 0xC0000000u) == 0x80000000u; }

    /** return true if address is c-class */
    bool is_class_c() const
    { return (to_ulong() & 0xE0000000u) == 0xC0000000u; }

    /** return true if address is multicast group */
    bool is_multicast() const
    { return (to_ulong() & 0xF0000000u) == 0xE0000000u; }

    /** construct unspecified address */
    static address_v4 any()
    { return address_v4(); }

    /** construct loopback address */
    static address_v4 loopback()
    { return address_v4(0x7F000001); }
};

} /* namespace net */
} /* namespace plusone */

#endif /* MADLIFE_address_v4_051116231424_MADLIFE */
