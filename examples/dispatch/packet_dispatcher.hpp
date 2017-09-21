/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_packet_dispatcher_290417001519
#define KSERGEY_packet_dispatcher_290417001519

#include <netinet/ip.h>
#include <arpa/inet.h>
#include <linux/udp.h>
#include <tuple>
#include <type_traits>
#include <plusone/net/mmap_rx.hpp>

namespace dispatch {
namespace detail {

/** Push packet to handler which match endpoint */
template< std::size_t Index = 0, class... Ts >
__force_inline std::enable_if_t< Index == sizeof...(Ts), void > push_packet(std::tuple< Ts... >& t,
        const endpoint_v4& ep, std::uint64_t timestamp, const std::uint8_t* data, std::size_t size)
{}
/** Push packet to handler which match endpoint */
template< std::size_t Index = 0, class... Ts >
__force_inline std::enable_if_t< Index < sizeof...(Ts), void > push_packet(std::tuple< Ts... >& t,
        const endpoint_v4& ep, std::uint64_t timestamp, const std::uint8_t* data, std::size_t size)
{
    if (std::get< Index >(t).contain(ep)) {
        std::get< Index >(t).process_packet(ep, timestamp, data, size);
    } else {
        push_packet< Index + 1, Ts... >(t, ep, timestamp, data, size);
    }
}

} /* namespace detail */

/** Decode UDP packets and fire PacketHandler in case of match endpoint */
template< class... PacketHandlers >
class packet_dispatcher final
{
private:
    using packet_queue = plusone::net::mmap_rx;

    packet_queue& queue_;
    std::tuple< PacketHandlers&... > handlers_;

public:
    packet_dispatcher(const packet_dispatcher&) = delete;
    packet_dispatcher& operator=(const packet_dispatcher&) = delete;

    /** Construct dispatcher */
    explicit packet_dispatcher(packet_queue& queue, PacketHandlers&... handlers)
        : queue_{queue}
        , handlers_{handlers...}
    {}

    /** Dispatch one packet from queue if available */
    __force_inline void dispatch()
    {
        /* Get next packet */
        auto frame = queue_.get_next_frame();
        if (__unlikely(!frame)) {
            return;
        }
        /* Process packet (decode and fire handler) */
        process(frame);
    }

private:
    __force_inline void process(const packet_queue::frame& frame)
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
        detail::push_packet(handlers_, ep, timestamp, payload_data, payload_size);
    }
};

} /* namespace dispatch */

#endif /* KSERGEY_packet_dispatcher_290417001519 */
