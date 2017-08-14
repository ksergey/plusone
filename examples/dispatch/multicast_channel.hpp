/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_multicast_channel_270417232305
#define KSERGEY_multicast_channel_270417232305

#include <plusone/net/socket.hpp>
#include <plusone/net/socket_options.hpp>
#include "endpoint_v4.hpp"
#include "exception.hpp"

namespace dispatch {

using plusone::throw_ex;

class multicast_channel final
{
private:
    using join_source_group = plusone::net::socket_options::ip::multicast::join_source_group;
    using leave_source_group = plusone::net::socket_options::ip::multicast::leave_source_group;

    /* Multicast channel ip:port */
    endpoint_v4 address_;
    /* Multicast channel underlyin socket */
    plusone::net::socket socket_;
    /* Join socket request */
    join_source_group join_request_;
    /* Leave socket request */
    leave_source_group leave_request_;

public:
    /** Config */
    struct config
    {
        std::string ip;
        std::uint16_t port;
        std::string src_ip;
    };

    /** Construct non-valid channel */
    multicast_channel() = default;

    multicast_channel(const multicast_channel&) = delete;
    multicast_channel& operator=(const multicast_channel&) = delete;

    /** Move constructor */
    __force_inline multicast_channel(multicast_channel&& s)
    { swap(s); }

    /** Move operator */
    __force_inline multicast_channel& operator=(multicast_channel&& s)
    {
        if (__likely(this != &s)) {
            swap(s);
        }
        return *this;
    }

    /** Construct channel */
    explicit multicast_channel(const config& cfg)
        : multicast_channel{cfg.ip, cfg.port, cfg.src_ip}
    {}

    /** Construct channel */
    multicast_channel(const std::string& ip, std::uint16_t port,
            const std::string& src_ip)
        : address_{ip, port}
    {
        socket_ = plusone::net::socket::create(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        auto res = socket_.bind(port);
        if (__unlikely(!res)) {
            throw_ex< error >("Failed to bind socket to port %d (%s)", port, res.str());
        }

        auto group_ip = plusone::net::address_v4::from_string(ip);
        auto group_src_ip = plusone::net::address_v4::from_string(src_ip);
        join_request_ = join_source_group{group_ip, group_src_ip};
        leave_request_ = leave_source_group{group_ip, group_src_ip};
    }

    /** Return channel underlyin socket */
    plusone::net::socket& socket() noexcept
    { return socket_; }

    /** Return multicast channel address (ipv4:port) */
    __force_inline const endpoint_v4& address() const noexcept
    { return address_; }

    /** Swap content with another object */
    __force_inline void swap(multicast_channel& ch)
    {
        std::swap(address_, ch.address_);
        std::swap(socket_, ch.socket_);
        std::swap(join_request_, ch.join_request_);
        std::swap(leave_request_, ch.leave_request_);
    }

    /** Join to multicast channel */
    __force_inline void join()
    {
        auto res = socket_.set_option(join_request_);
        if (__unlikely(!res)) {
            throw_ex< error >("Failed to join multicast group (%s)", res.str());
        }
    }

    /** Leave from multicast group */
    __force_inline void leave()
    {
        auto res = socket_.set_option(leave_request_);
        if (__unlikely(!res)) {
            throw_ex< error >("Failed to leave multicast group (%s)", res.str());
        }
    }

    /** Return true if channel valid */
    __force_inline explicit operator bool() const noexcept
    { return socket_.operator bool(); }

    /** Return true if channel not valid */
    __force_inline bool operator!() const noexcept
    { return !socket_.operator bool(); }
};

} /* namespace dispatch */

#endif /* KSERGEY_multicast_channel_270417232305 */
