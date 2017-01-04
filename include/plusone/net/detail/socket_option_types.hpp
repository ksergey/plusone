/*
 * Copyright (c) 2016-2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef MADLIFE_socket_option_types_061116000852_MADLIFE
#define MADLIFE_socket_option_types_061116000852_MADLIFE

#include "../common.hpp"
#include "../address_v4.hpp"

namespace plusone {
namespace net {
namespace detail {

/** boolean option type */
template< int Level, int Name >
class boolean_option
{
private:
    /* option value */
    int value_{0};

public:
    /** default constructor */
    boolean_option() = default;

    /** destructor */
    ~boolean_option() = default;

    /** construct boolean option */
    explicit boolean_option(bool value)
        : value_(value ? 1 : 0)
    {}

    /** return option value */
    __force_inline bool value() const noexcept
    { return !!value_; }

    /** cast to bool */
    __force_inline explicit operator bool() const noexcept
    { return !!value_; }

    /** option level */
    __force_inline int level() const noexcept
    { return Level; }

    /** option name */
    __force_inline int name() const noexcept
    { return Name; }

    /** option data */
    __force_inline void* data() noexcept
    { return &value_; }

    /** option data */
    __force_inline const void* data() const noexcept
    { return &value_; }

    /** option size */
    __force_inline size_t size() const noexcept
    { return sizeof(value_); }

    /** resize option data */
    __force_inline void resize(size_t size)
    {
        if (size != sizeof(value_)) {
            throw option_error{"boolean socket option resize"};
        }
    }
};

/** integer option type */
template< int Level, int Name >
class integer_option
{
private:
    /* option value */
    int value_{0};

public:
    /** default constructor */
    integer_option() = default;

    /** destructor */
    ~integer_option() = default;

    /** construct integer option */
    explicit integer_option(bool value)
        : value_(value)
    {}

    /** return option value */
    __force_inline int value() const noexcept { return value_; }

    /** cast to bool */
    __force_inline explicit operator bool() const noexcept { return !!value_; }

    /** option level */
    __force_inline int level() const noexcept { return Level; }

    /** option name */
    __force_inline int name() const noexcept { return Name; }

    /** option data */
    __force_inline void* data() noexcept { return &value_; }

    /** option data */
    __force_inline const void* data() const noexcept { return &value_; }

    /** option size */
    __force_inline size_t size() const noexcept { return sizeof(value_); }

    /** resize option data */
    __force_inline void resize(size_t size)
    {
        if (size != sizeof(value_)) {
            throw option_error{"integer socket option resize"};
        }
    }
};

/** multicast request type */
template< int Name >
class multicast_request_v4_option
{
private:
    /* option value */
    ip_mreq value_{};

public:
    /** default constructor */
    multicast_request_v4_option() = default;

    /** construct multicast option */
    explicit multicast_request_v4_option(const address_v4& multicast_address,
            const address_v4& network_interface = address_v4::any())
        : multicast_request_v4_option()
    {
        value_.imr_multiaddr.s_addr = htonl(multicast_address.to_ulong());
        value_.imr_interface.s_addr = htonl(network_interface.to_ulong());
    }

    /** option level */
    __force_inline int level() const noexcept { return IPPROTO_IP; }

    /** option name */
    __force_inline int name() const noexcept { return Name; }

    /** option data */
    __force_inline const void* data() const noexcept { return &value_; }

    /** option size */
    __force_inline size_t size() const noexcept { return sizeof(value_); }
};

/** multicast source request type */
template< int Name >
class multicast_source_request_v4_option
{
private:
    /* option value */
    ip_mreq_source value_{};

public:
    /** default constructor */
    multicast_source_request_v4_option() = default;

    /** construct option */
    multicast_source_request_v4_option(const address_v4& multicast_address,
            const address_v4& multicast_source_address,
            const address_v4& network_interface = address_v4::any())
        : multicast_source_request_v4_option()
    {
        value_.imr_multiaddr.s_addr = htonl(multicast_address.to_ulong());
        value_.imr_interface.s_addr = htonl(network_interface.to_ulong());
        value_.imr_sourceaddr.s_addr = htonl(multicast_source_address.to_ulong());
    }

    /** option level */
    __force_inline int level() const noexcept { return IPPROTO_IP; }

    /** option name */
    __force_inline int name() const noexcept { return Name; }

    /** option data */
    __force_inline const void* data() const noexcept { return &value_; }

    /** option size */
    __force_inline size_t size() const noexcept { return sizeof(value_); }
};

/** generic option type */
class generic_socket_option
{
private:
    int level_{-1};
    int name_{-1};
    const void* data_{nullptr};
    std::size_t size_{0};

public:
    /** default constructor */
    generic_socket_option() = default;

    /** construct option */
    generic_socket_option(int name, int level, const void* data, std::size_t size)
        : level_(level)
        , name_(name)
        , data_(data)
        , size_(size)
    {}

    /** construct option */
    template< class T >
    generic_socket_option(int level, int name, const T& value)
        : level_(level)
        , name_(name)
        , data_(&value)
        , size_(sizeof(value))
    {}

    /** option level */
    __force_inline int level() const noexcept { return level_; }

    /** option name */
    __force_inline int name() const noexcept { return name_; }

    /** option data */
    __force_inline const void* data() const noexcept { return data_; }

    /** option size */
    __force_inline size_t size() const noexcept { return size_; }
};

} /* namespace detail */
} /* namespace net */
} /* namespace plusone */

#endif /* MADLIFE_socket_option_types_061116000852_MADLIFE */
