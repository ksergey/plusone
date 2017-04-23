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

/** Boolean option type */
template< int Level, int Name >
class boolean_option
{
private:
    /* Option value */
    int value_{0};

public:
    boolean_option() = default;
    ~boolean_option() = default;

    /** Construct boolean option */
    explicit boolean_option(bool value)
        : value_{value ? 1 : 0}
    {}

    /** Return option value */
    __force_inline bool value() const noexcept
    { return !!value_; }

    /** Cast to bool */
    __force_inline explicit operator bool() const noexcept
    { return !!value_; }

    /** Option level */
    __force_inline int level() const noexcept
    { return Level; }

    /** Option name */
    __force_inline int name() const noexcept
    { return Name; }

    /** Option data */
    __force_inline void* data() noexcept
    { return &value_; }

    /** Option data */
    __force_inline const void* data() const noexcept
    { return &value_; }

    /** Option size */
    __force_inline std::size_t size() const noexcept
    { return sizeof(value_); }

    /** Resize option data */
    __force_inline void resize(std::size_t size)
    {
        if (size != sizeof(value_)) {
            throw option_error{"Boolean socket option resize"};
        }
    }
};

/** Integer option type */
template< int Level, int Name >
class integer_option
{
private:
    /* Option value */
    int value_{0};

public:
    integer_option() = default;
    ~integer_option() = default;

    /** Construct integer option */
    explicit integer_option(int value)
        : value_{value}
    {}

    /** Return option value */
    __force_inline int value() const noexcept
    { return value_; }

    /** Cast to bool */
    __force_inline explicit operator bool() const noexcept
    { return !!value_; }

    /** Option level */
    __force_inline int level() const noexcept
    { return Level; }

    /** Option name */
    __force_inline int name() const noexcept
    { return Name; }

    /** Option data */
    __force_inline void* data() noexcept
    { return &value_; }

    /** Option data */
    __force_inline const void* data() const noexcept
    { return &value_; }

    /** Option size */
    __force_inline std::size_t size() const noexcept
    { return sizeof(value_); }

    /** Resize option data */
    __force_inline void resize(std::size_t size)
    {
        if (size != sizeof(value_)) {
            throw option_error{"Integer socket option resize"};
        }
    }
};

/** Struct option type */
template< int Level, int Name, class StructT >
class struct_option
{
protected:
    StructT value_{};

public:
    struct_option() = default;

    /** Construct option */
    explicit struct_option(const StructT& value)
        : value_{value}
    {}

    /** Return option value */
    __force_inline const StructT& value() const noexcept
    { return value_; }

    /** Return option value for modification */
    __force_inline StructT& value() noexcept
    { return value_; }

    /** Option level */
    __force_inline int level() const noexcept
    { return Level; }

    /** Option name */
    __force_inline int name() const noexcept
    { return Name; }

    /** Option data */
    __force_inline void* data() noexcept
    { return &value_; }

    /** Option data */
    __force_inline const void* data() const noexcept
    { return &value_; }

    /** Option size */
    __force_inline std::size_t size() const noexcept
    { return sizeof(value_); }
};

/** Multicast request type */
template< int Name >
class multicast_request_v4_option
{
private:
    /* Option value */
    ip_mreq value_{};

public:
    multicast_request_v4_option() = default;

    /** Construct multicast option */
    explicit multicast_request_v4_option(const address_v4& multicast_address,
            const address_v4& network_interface = address_v4::any())
        : multicast_request_v4_option()
    {
        value_.imr_multiaddr.s_addr = htonl(multicast_address.to_ulong());
        value_.imr_interface.s_addr = htonl(network_interface.to_ulong());
    }

    /** Option level */
    __force_inline int level() const noexcept
    { return IPPROTO_IP; }

    /** Option name */
    __force_inline int name() const noexcept
    { return Name; }

    /** Option data */
    __force_inline const void* data() const noexcept
    { return &value_; }

    /** Option size */
    __force_inline std::size_t size() const noexcept
    { return sizeof(value_); }
};

/** Multicast source request type */
template< int Name >
class multicast_source_request_v4_option
{
private:
    /* Option value */
    ip_mreq_source value_{};

public:
    multicast_source_request_v4_option() = default;

    /** Construct option */
    multicast_source_request_v4_option(const address_v4& multicast_address,
            const address_v4& multicast_source_address,
            const address_v4& network_interface = address_v4::any())
        : multicast_source_request_v4_option()
    {
        value_.imr_multiaddr.s_addr = htonl(multicast_address.to_ulong());
        value_.imr_interface.s_addr = htonl(network_interface.to_ulong());
        value_.imr_sourceaddr.s_addr = htonl(multicast_source_address.to_ulong());
    }

    /** Option level */
    __force_inline int level() const noexcept
    { return IPPROTO_IP; }

    /** Option name */
    __force_inline int name() const noexcept
    { return Name; }

    /** Option data */
    __force_inline const void* data() const noexcept
    { return &value_; }

    /** Option size */
    __force_inline std::size_t size() const noexcept
    { return sizeof(value_); }
};

/** Generic option type */
class generic_socket_option
{
private:
    int level_{-1};
    int name_{-1};
    const void* data_{nullptr};
    std::size_t size_{0};

public:
    generic_socket_option() = default;

    /** Construct option */
    generic_socket_option(int level, int name, const void* data, std::size_t size)
        : level_(level)
        , name_(name)
        , data_(data)
        , size_(size)
    {}

    /** Construct option */
    template< class T >
    generic_socket_option(int level, int name, const T& value)
        : level_(level)
        , name_(name)
        , data_(&value)
        , size_(sizeof(value))
    {}

    /** Option level */
    __force_inline int level() const noexcept
    { return level_; }

    /** Option name */
    __force_inline int name() const noexcept
    { return name_; }

    /** Option data */
    __force_inline const void* data() const noexcept
    { return data_; }

    /** Option size */
    __force_inline std::size_t size() const noexcept
    { return size_; }
};

} /* namespace detail */
} /* namespace net */
} /* namespace plusone */

#endif /* MADLIFE_socket_option_types_061116000852_MADLIFE */
