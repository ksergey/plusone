/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_channel_group_280417170938
#define KSERGEY_channel_group_280417170938

#include <netinet/ip.h>
#include <arpa/inet.h>
#include <linux/udp.h>
#include <tuple>
#include <type_traits>
#include <plusone/net/mmap_rx.hpp>

namespace dispatch {

template< std::size_t Index = 0, class... Ts >
__force_inline std::enable_if_t< Index == sizeof...(Ts), void > emit_packet(std::tuple< Ts... >& t,
        const endpoint_v4& ep, std::uint64_t timestamp, const std::uint8_t* data, std::size_t size)
{}

template< std::size_t Index = 0, class... Ts >
__force_inline std::enable_if_t< Index < sizeof...(Ts), void > emit_packet(std::tuple< Ts... >& t,
        const endpoint_v4& ep, std::uint64_t timestamp, const std::uint8_t* data, std::size_t size)
{
    if (std::get< Index >(t).contain(ep)) {
        std::get< Index >(t).process(timestamp, data, size);
    } else {
        emit_packet< Index + 1, Ts... >(t, ep, timestamp, data, size);
    }
}

template< class... Ts >
class channel_group final
{
private:
    plusone::net::mmap_rx& rx_queue_;
    std::tuple< Ts&... > packet_handlers_;

public:
    channel_group(const channel_group&) = delete;
    channel_group& operator=(const channel_group&) = delete;

    explicit channel_group(plusone::net::mmap_rx& rx_queue, Ts&... args)
        : rx_queue_{rx_queue}
        , packet_handlers_{args...}
    {}

    /** Dispatch RX queue */
    __force_inline void run_once()
    {
        auto frame = rx_queue_.get_next_frame();
        if (__unlikely(!frame)) {
            return ;
        }

        process(frame);
        frame.commit();
    }

private:
    __force_inline void process(const plusone::net::mmap_rx::frame& frame)
    {
        /* Extract IP header */
        const auto& ip = frame.as< iphdr >();

        /* Check UDP packet */
        if (__unlikely(ip.protocol != IPPROTO_UDP)) {
            return ;
        }

        /* Offset to UDP payload */
        std::size_t payload_offset = ip.ihl * 4;

        /* Extract UDP header */
        const auto& udp = frame.as< udphdr >(payload_offset);

        /* Update offset */
        payload_offset += sizeof(udphdr);

        /* Construct endpoint_v4 */
        endpoint_v4 ep{ip.daddr, ntohs(udp.dest)};

        /* Construct timestamp */
        const std::uint64_t timestamp = frame.sec() * 1000000000ul + frame.nsec();

        /* Payload data */
        const std::uint8_t* payload_data = frame.data() + payload_offset;

        /* Payload size */
        const std::size_t payload_size = udp.len;

        /* Fire event */
        emit_packet(packet_handlers_, ep, timestamp, payload_data, payload_size);
    }
};

} /* namespace dispatch */

#endif /* KSERGEY_channel_group_280417170938 */
