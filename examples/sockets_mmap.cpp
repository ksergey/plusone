/*
 * Copyright (c) 2016-2017 Sergey Kovalevich <inndie@gmail.com>
 */

#include <arpa/inet.h>
#include <linux/if_packet.h>
#include <net/if.h>
#include <netinet/if_ether.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>

#include <iostream>
#include <string>
#include <cctype>
#include <vector>
#include <plusone/net/socket.hpp>
#include <plusone/net/socket_option.hpp>

using namespace std::string_literals;

constexpr const std::size_t ring_buffers_count = 100000;

class packet_rx_ring final
{
private:
    /* option value */
    tpacket_req req_{};

public:
    /** default constructor */
    packet_rx_ring() = default;

    /** construct option */
    explicit packet_rx_ring(std::size_t ring_buffers_count)
    {
        req_.tp_block_size = ring_buffers_count * getpagesize();
        req_.tp_block_nr = 1;
        req_.tp_frame_size = getpagesize();
        req_.tp_frame_nr = ring_buffers_count;
    }

    __force_inline auto block_size() const noexcept { return req_.tp_block_size; }
    __force_inline auto block_nr() const noexcept { return req_.tp_block_nr; }
    __force_inline auto frame_size() const noexcept { return req_.tp_frame_size; }
    __force_inline auto frame_nr() const noexcept { return req_.tp_frame_nr; }

    __force_inline auto length() const noexcept { return block_size() * block_nr(); }

    /** option level */
    __force_inline int level() const noexcept { return SOL_PACKET; }

    /** option name */
    __force_inline int name() const noexcept { return PACKET_RX_RING; }

    /** option data */
    __force_inline const void* data() const noexcept { return &req_; }

    /** option size */
    __force_inline size_t size() const noexcept { return sizeof(req_); }
};

void process_ip_packet(const unsigned char* data, std::size_t len);

int main(int argc, char* argv[])
{
    if (argc < 2) {
        std::cerr << "usage: sockets_mmap <iface>\n";
        return EXIT_FAILURE;
    }

    const char* iface = argv[1];

    try {
        //auto rx_ring_fd = plusone::net::socket::create(PF_PACKET, SOCK_DGRAM, htons(ETH_P_IP));
        auto rx_ring_fd = plusone::net::socket::create(PF_PACKET, SOCK_DGRAM, 0);

        packet_rx_ring req(ring_buffers_count);
        auto res = rx_ring_fd.set_option(req);
        if (!res) {
            throw std::runtime_error("option packet_rx_ring failed");
        }

        ifreq ifr;
        std::memset(&ifr, 0, sizeof(ifr));
        strncpy(ifr.ifr_name, iface, sizeof(ifr.ifr_name));
        if (ioctl(rx_ring_fd.get(), SIOCGIFINDEX, &ifr) == -1) {
            throw std::runtime_error("ioctl error ("s + strerror(errno) + ")");
        }

        sockaddr_ll addr;
        std::memset(&addr, 0, sizeof(addr));
        addr.sll_family = AF_PACKET;
        addr.sll_protocol = htons(ETH_P_ALL);
        addr.sll_ifindex = ifr.ifr_ifindex;
        res = rx_ring_fd.bind(reinterpret_cast< sockaddr*>(&addr), sizeof(addr));
        if (!res) {
            throw std::runtime_error("socket bind error ("s + res.str() + ')');
        }

        void* rx_ring = ::mmap(0, req.length(), PROT_READ | PROT_WRITE, MAP_SHARED,
                rx_ring_fd.get(), 0);
        if (rx_ring == MAP_FAILED) {
            throw std::runtime_error("mmap error ("s + strerror(errno) + ")");
        }

        char* rx_ring_ptr = reinterpret_cast< char* >(rx_ring);

        std::vector< unsigned char* > frames;
        frames.resize(req.frame_nr());
        for (std::size_t idx = 0; idx < frames.size(); ++idx) {
            frames[idx] = reinterpret_cast< unsigned char* >(rx_ring_ptr) + idx * req.frame_size();
        }

        std::size_t rx_ring_idx = 0;

        tpacket_hdr* header = reinterpret_cast< tpacket_hdr* >(frames[rx_ring_idx]);

        while (true) {
            while (header->tp_status != TP_STATUS_KERNEL) {
                sockaddr_ll* packet_addr = reinterpret_cast< sockaddr_ll* >(reinterpret_cast< unsigned char* >(header)
                        + TPACKET_ALIGN(sizeof(tpacket_hdr)));
                if (packet_addr->sll_protocol == ntohs(ETH_P_IP)) {
                    /* process packet */
                    process_ip_packet(reinterpret_cast< unsigned char* >(header) + header->tp_net, header->tp_len);
                }

                header->tp_status = TP_STATUS_KERNEL;
                __sync_synchronize();
                rx_ring_idx = (rx_ring_idx + 1) % ring_buffers_count;
                header = reinterpret_cast< tpacket_hdr* >(frames[rx_ring_idx]);
            }

        }

    } catch (const std::exception& e) {
        std::cerr << "ERROR: " << e.what() << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

void process_ip_packet(const unsigned char* data, std::size_t len)
{
    if (len < sizeof(iphdr)) {
        return ;
    }

    const iphdr* ip_header = reinterpret_cast< const iphdr* >(data);
    const std::size_t iphdrlen = ip_header->ihl * 4;

    if (len < iphdrlen) {
        return ;
    }

    if (ip_header->protocol != 0x11) {
        return ;
    }

    /* UDP */

    if (len < iphdrlen + sizeof(udphdr)) {
        return ;
    }

    const unsigned char* saddr = reinterpret_cast< const unsigned char* >(&ip_header->saddr);
    const unsigned char* daddr = reinterpret_cast< const unsigned char* >(&ip_header->daddr);
    const udphdr* udp_header = reinterpret_cast< const udphdr* >(data + iphdrlen);
    const char* payload = reinterpret_cast< const char* >(udp_header + 1);
    const std::size_t payload_size = len - sizeof(iphdr) - sizeof(udphdr);

    std::cout << "[udp] "
        << int(saddr[0]) << '.' << int(saddr[1]) << '.' << int(saddr[2]) << '.' << int(saddr[3]) << ':' << ntohs(udp_header->source) << " -> "
        << int(daddr[0]) << '.' << int(daddr[1]) << '.' << int(daddr[2]) << '.' << int(daddr[3]) << ':' << ntohs(udp_header->dest) << '\n';

    std::cout << "  ";
    for (std::size_t i = 0; i < len; i++) {
        if (isprint(payload[i])) {
            std::cout << payload[i];
        } else {
            std::cout << '.';
        }
    }
    std::cout << '\n';
}
