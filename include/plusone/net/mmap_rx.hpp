/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_mmap_rx_240417160759
#define KSERGEY_mmap_rx_240417160759

#include "socket.hpp"
#include "socket_options.hpp"
#include "../mapped_region.hpp"
#include "../static_vector.hpp"
#include "../math.hpp"

namespace plusone {
namespace net {

/**
 * Memory mapped RX queue packet receiver
 *
 * @see https://www.kernel.org/doc/Documentation/networking/packet_mmap.txt
 */
class mmap_rx
{
private:
    /* PACKET_RX_RING request */
    using rx_ring_request = detail::struct_option< SOL_PACKET, PACKET_RX_RING, tpacket_req >;

    /* Timestamping option */
    static constexpr int timestamp_option = SOF_TIMESTAMPING_RAW_HARDWARE | SOF_TIMESTAMPING_RX_HARDWARE;

    /* RX socket */
    socket socket_;
    /* Shared memory mapping */
    mapped_region region_;
    /* Array of pointer to packet */
    static_vector< struct iovec > rd_;
    /* Frame size */
    std::size_t frame_size_{0};
    /* Frames count */
    std::size_t frame_nr_{0};
    /* Active frame */
    std::size_t frame_index_{0};

public:
    /** Mmap RX frame */
    class frame;

    mmap_rx(const mmap_rx&) = delete;
    mmap_rx& operator=(const mmap_rx&) = delete;

    /** Construct memory mmaped RX queue */
    mmap_rx(const char* netdev, std::size_t buffer_size, std::size_t max_packet_size = 2048);

    /** Destructor */
    virtual ~mmap_rx() = default;

    /** Get next frame */
    frame get_next_frame() noexcept;
};

} /* namespace net */
} /* namespace plusone */

#include "impl/mmap_rx.ipp"

#endif /* KSERGEY_mmap_rx_240417160759 */
