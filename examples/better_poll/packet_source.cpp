/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#include "packet_source.hpp"
#include <net/if.h>
#include <iostream>
#include <stdexcept>
#include <plusone/net/socket_option.hpp>

namespace better_poll {

packet_source::packet_source(const char* iface, std::size_t socket_buffer_size)
{
    /* Create packet socket (skip ethernet header) */
    socket_ = plusone::net::socket::create(AF_PACKET, SOCK_DGRAM, ETH_P_IP);
    /* Make non-block mode */
    socket_.set_nonblock();
    /* Apply socket options */
    apply_socket_options(socket_buffer_size);

    /* Bind the socket */
    sockaddr_ll addr;
    std::memset(&addr, 0, sizeof(addr));
    addr.sll_family = AF_PACKET;
    addr.sll_protocol = htons(ETH_P_IP);
    if (iface) {
        addr.sll_ifindex = if_nametoindex(iface);
        if (__unlikely(addr.sll_ifindex == 0)) {
            throw std::runtime_error("Interface \"" + std::string{iface} + "\" not found");
        }
    } else {
        addr.sll_ifindex = 0;
    }
    auto result = socket_.bind(reinterpret_cast< sockaddr* >(&addr), sizeof(addr));
    if (__unlikely(!result)) {
        throw std::runtime_error("Failed to bind multicast socket (" + std::string{result.str()} + ')');
    }
}

__force_inline void packet_source::apply_socket_options(std::size_t socket_buffer_size)
{
    auto result = socket_.set_option(plusone::net::timestamp_ns{true});
    if (!result) {
        std::cout << "Failed to set option timestamp_ns\n";
    }

    constexpr int hardware_rx_timestamping = SOF_TIMESTAMPING_RAW_HARDWARE | SOF_TIMESTAMPING_RX_HARDWARE;
    result = socket_.set_option(plusone::net::packet_timestamp{hardware_rx_timestamping});
    if (!result) {
        std::cout << "Failed to set option packet_timestamp\n";
    }

    if (socket_buffer_size > 0) {
        result = socket_.set_option(plusone::net::rcvbuf{socket_buffer_size});
        if (!result) {
            std::cout << "Failed to set option rcvbuf\n";
        }
    }
}

} /* namespace better_poll */
