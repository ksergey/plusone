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
    address_v4() noexcept;

    /** Construct address */
    explicit address_v4(unsigned long addr);

    /** IPv4 address to unsigned long */
    unsigned long to_ulong() const noexcept;

    /** IPv4 address to string dot-notation */
    std::string to_string() const;

    /** Convert address from const char* */
    static address_v4 from_string(const char* str);

    /** Convert address from std::string */
    static address_v4 from_string(const std::string& str);

    /** Convert address from const char* */
    static bool from_string(const char* str, address_v4& result);

    /** Convert address from std::string */
    static bool from_string(const std::string& str, address_v4& result);

    /** Return true if address is loopback */
    bool is_loopback() const noexcept;

    /** Return true if address is unspecified */
    bool is_unspecified() const noexcept;

    /** Return true if address is a-class */
    bool is_class_a() const noexcept;

    /** Return true if address is b-class */
    bool is_class_b() const noexcept;

    /** Return true if address is c-class */
    bool is_class_c() const noexcept;

    /** Return true if address is multicast group */
    bool is_multicast() const noexcept;

    /** Construct unspecified address */
    static address_v4 any();

    /** Construct loopback address */
    static address_v4 loopback();
};

} /* namespace net */
} /* namespace plusone */

#include "impl/address_v4.ipp"

#endif /* MADLIFE_address_v4_051116231424_MADLIFE */
