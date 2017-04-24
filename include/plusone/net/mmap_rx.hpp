/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_mmap_rx_240417160759
#define KSERGEY_mmap_rx_240417160759

#include <net/if.h>
#include "socket.hpp"
#include "socket_options.hpp"
#include "../mapped_region.hpp"
#include "../static_vector.hpp"

namespace plusone {
namespace net {

/** Memory mapped RX queue packet receiver */
class mmap_rx
{
private:
    /* PACKET_RX_RING request */
    using rx_ring_request = detail::struct_option< SOL_PACKET, PACKET_RX_RING, tpacket_req >;

    /* Timestamping option */
    static constexpr int timestamp_option = SOF_TIMESTAMPING_RAW_HARDWARE | SOF_TIMESTAMPING_RX_HARDWARE;

    const std::size_t blocks_count_{0};
    socket socket_;
    mapped_region region_;
    static_vector< struct iovec > rd_;
    std::size_t block_num_{0};

public:
    mmap_rx(const mmap_rx&) = delete;
    mmap_rx& operator=(const mmap_rx&) = delete;

    /** Construct memory mmaped RX queue */
    mmap_rx(const char* netdev, std::size_t block_size, std::size_t blocks_count, std::size_t frame_size)
        : blocks_count_{blocks_count}
    {
        /* Create socket */
        socket_ = socket::create(AF_PACKET, SOCK_DGRAM, htons(ETH_P_IP));

        /* Apply timestamp option */
        auto option_result = socket_.set_option(socket_options::packet::timestamp{timestamp_option});
        if (__unlikely(!option_result)) {
            throw socket_error{"Failed to set socket option PACKET_TIMESTAMP (%s)", option_result.str()};
        }

        /* Apply TPACKET_V2 option */
        option_result = socket_.set_option(socket_options::packet::version{TPACKET_V2});
        if (__unlikely(!option_result)) {
            throw socket_error{"Failed to set socket option PACKET_VERSION (%s)", option_result.str()};
        }

        /* Prepare RX ring config */
        tpacket_req req;
        std::memset(&req, 0, sizeof(req));
        req.tp_block_size = block_size;
        req.tp_block_nr = blocks_count;
        req.tp_frame_size = frame_size;
        req.tp_frame_nr = (req.tp_block_size * req.tp_block_nr) / req.tp_frame_size;

        /* Apply PACKET_RX_RING option */
        option_result = socket_.set_option(rx_ring_request{req});
        if (__unlikely(!option_result)) {
            throw socket_error{"Failed to set socket option PACKET_RX_RING (%s)", option_result.str()};
        }

        /* Mmap socket RX queue */
        region_ = plusone::mapped_region{socket_.get(), req.tp_block_size * req.tp_block_nr};

        /* Prepare iovec buffers */
        rd_ = static_vector< struct iovec >{req.tp_block_nr};
        for (std::size_t i = 0; i < req.tp_block_nr; ++i) {
            auto& entry = rd_.emplace_back();
            entry.iov_base = region_.data() + (i * req.tp_block_size);
            entry.iov_len = req.tp_block_size;
        }

        /* Bind socket */
        struct sockaddr_ll ll;
        std::memset(&ll, 0, sizeof(ll));
        ll.sll_family = PF_PACKET;
        ll.sll_protocol = htons(ETH_P_IP);
        if (netdev) {
            ll.sll_ifindex = if_nametoindex(netdev);
            if (ll.sll_ifindex == 0) {
                throw socket_error{"Interface \"%s\" not found", netdev};
            }
        } else {
            ll.sll_ifindex = 0; /* All interfaces */
        }
        ll.sll_hatype = 0;
        ll.sll_pkttype = 0;
        ll.sll_halen = 0;
        auto bind_result = socket_.bind((sockaddr*)& ll, sizeof(ll));
        if (!bind_result) {
            throw socket_error{"Failed to bind mmap_rx socket (%s)", bind_result.str()};
        }
    }

    /** Destructor */
    virtual ~mmap_rx() = default;

    /* TEST */
    void run_once()
    {
        tpacket2_hdr* header = reinterpret_cast< tpacket2_hdr* >(rd_[block_num_].iov_base);
        if ((header->tp_status & TP_STATUS_USER) == 0) {
            return;
        }

        std::cout << block_num_ << ' ' << header->tp_sec << '.' << header->tp_nsec << " packet\n";

        header->tp_status = TP_STATUS_KERNEL;
        block_num_ = (block_num_ + 1) % blocks_count_;
    }
};

} /* namespace net */
} /* namespace plusone */

#endif /* KSERGEY_mmap_rx_240417160759 */
