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

int main(int argc, char* argv[])
{
    auto sock = plusone::net::socket::create(PF_PACKET, SOCK_DGRAM, ETH_P_IP);
    sock.set_nonblock();

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

    char buffer[2000];
    while (true) {
        auto res = sock.recv(buffer, sizeof(buffer));
        if (!res) {
            if (res.again()) {
                continue;
            } else {
                break;
            }
        }

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
            << "   Destination address: " << inet_ntoa(dest.sin_addr) << '\n'
            << "   Destination port: " << ntohs(udp->dest) << '\n'
            << "   UDP Length: " << ntohs(udp->len) << '\n'
            << "   UDP Checksum: " << ntohs(udp->check) << '\n'
            << "   Payload size: " << size << '\n';
    }

    return 0;
}
