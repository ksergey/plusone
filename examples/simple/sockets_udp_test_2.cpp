/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#include <netinet/udp.h> /* Provides declarations for udp header */
#include <netinet/tcp.h> /* Provides declarations for tcp header */
#include <netinet/ip.h>  /* Provides declarations for ip header */
#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include <cstring>
#include <iostream>
#include <plusone/net/socket.hpp>
#include <plusone/net/socket_option.hpp>
#include <plusone/net/endpoint.hpp>
#include <plusone/net/tcp.hpp>
#include "time_fmt.hpp"

using plusone::time_fmt;

int main(int argc, char* argv[])
{
    auto sock = plusone::net::socket::create(PF_PACKET, SOCK_DGRAM, ETH_P_IP);
    sock.set_nonblock();
    sock.set_option(plusone::net::timestamp_ns{true});

    sockaddr_ll addr;
    std::memset(&addr, 0, sizeof(addr));
    addr.sll_family = AF_PACKET;
    addr.sll_protocol = htons(ETH_P_IP);
    addr.sll_ifindex = 0;

    auto bind_res = sock.bind(reinterpret_cast< sockaddr* >(&addr), sizeof(addr));
    if (__unlikely(!bind_res)) {
        std::cout << "Failed to bind socket: " << bind_res.str() << "\n";
        return 1;
    }

    /* Buffers for recv */
    char packet_buffer[2000];
    char control_buffer[2000];

    /* Prepare recv struct */
    msghdr header;
    std::memset(&header, 0, sizeof(header));
    iovec msg_iovec;
    std::memset(&msg_iovec, 0, sizeof(msg_iovec));

    msg_iovec.iov_base = packet_buffer;
    msg_iovec.iov_len = sizeof(packet_buffer);
    /* I don't use the feature because read raw packets */
    header.msg_name = nullptr;
    header.msg_namelen = 0;
    header.msg_iov = &msg_iovec;
    header.msg_iovlen = 1;
    header.msg_control = control_buffer;
    header.msg_controllen = sizeof(control_buffer);


    while (true) {
        auto res = sock.recvmsg(&header);
        if (!res) {
            if (res.again()) {
                continue;
            } else {
                break;
            }
        }

        if (__unlikely(header.msg_flags & MSG_TRUNC)) {
            std::cout << "Skipping truncated message\n";
            continue;
        } else if (__unlikely(header.msg_flags)) {
            std::cout << "Skipping unhandled flags\n";
            continue;
        }

        std::uint64_t packet_timestamp = 0;

        /* Get timestamp */
        cmsghdr* h = CMSG_FIRSTHDR(&header);
        while (h) {
            if (h->cmsg_level == SOL_SOCKET && h->cmsg_type == SCM_TIMESTAMPNS) {
                const timespec* ts = reinterpret_cast< timespec* >(CMSG_DATA(h));
                packet_timestamp = ts->tv_sec * 1000000000ul + ts->tv_nsec;
            }

            h = CMSG_NXTHDR(&header, h);
        }

        char* buffer = packet_buffer;

        iphdr* ip = reinterpret_cast< iphdr* >(buffer);
        if (__unlikely(ip->protocol != 17)) {
            continue;
        }
        udphdr* udp = reinterpret_cast< udphdr* >(buffer + ip->ihl * 4);

        const std::size_t payload_offset = ip->ihl * 4 + sizeof(udphdr);
        const char* data = buffer + payload_offset;
        const std::size_t size = res.bytes() - payload_offset;

        sockaddr_in dest;
        std::memset(&dest, 0, sizeof(dest));
        dest.sin_addr.s_addr = ip->daddr;

        std::cout
            << "UDP packet received:\n"
            << "   Timestamp: " << time_fmt{packet_timestamp} << '\n'
            << "   Destination address: " << inet_ntoa(dest.sin_addr) << '\n'
            << "   Destination port: " << ntohs(udp->dest) << '\n'
            << "   UDP Length: " << ntohs(udp->len) << '\n'
            << "   UDP Checksum: " << ntohs(udp->check) << '\n'
            << "   Payload size: " << size << '\n';
    }

    return 0;
}
