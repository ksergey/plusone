/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_mmap_rx_240417160759
#define KSERGEY_mmap_rx_240417160759

#include <net/if.h>
#include <unistd.h>
#include <cmath>
#include <cassert>
#include <type_traits>
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
    /** Mmap RX packet */
    class packet final
    {
    private:
        tpacket2_hdr* data_{nullptr};

    public:
        packet(const packet&) = delete;
        packet& operator=(const packet&) = delete;

        /** Move constructor */
        packet(packet&& v) noexcept
        { std::swap(v.data_, data_); }

        /** Move operator */
        packet& operator=(packet&& v) noexcept
        {
            std::swap(v.data_, data_);
            return *this;
        }

        /** Construct non-valid packet */
        packet() = default;

        /** Construct packet from native header */
        packet(tpacket2_hdr* data)
            : data_{data}
        {}

        /** Destructor */
        ~packet()
        { assert( (data_ ? data_->tp_status == TP_STATUS_KERNEL : true) && "packet not commited" ); }

        /** Return true if packet valid */
        __force_inline explicit operator bool() const noexcept
        { return data_ != nullptr; }

        /** Return true if packet not valid */
        __force_inline bool operator!() const noexcept
        { return data_ == nullptr; }

        /** Return packet timestamp */
        __force_inline std::uint32_t sec() const noexcept
        {
            assert( data_ );
            return data_->tp_sec;
        }

        /** Return packet timestamp (nanosecond part) */
        __force_inline std::uint32_t nsec() const noexcept
        {
            assert( data_ );
            return data_->tp_nsec;
        }

        /** Return packet data (started from ip header) */
        __force_inline const std::uint8_t* data() const noexcept
        {
            assert( data_ );
            return reinterpret_cast< const std::uint8_t* >(data_) + data_->tp_mac;
        }

        /** Return packet data size */
        __force_inline std::size_t size() const noexcept
        {
            assert( data_ );
            return data_->tp_len;
        }

        /** Return struct as required type */
        template< class T >
        __force_inline const T& as(std::size_t offset = 0) const noexcept
        {
            static_assert( std::is_trivially_copyable< T >::value, "class T must be trivially copyable" );
            assert( data_ && offset + sizeof(T) <= size() );
            return *reinterpret_cast< const T* >(data() + offset);
        }

        /** Return packet to RX ring */
        __force_inline void commit() noexcept
        {
            assert( data_ );
            data_->tp_status = TP_STATUS_KERNEL;
            __sync_synchronize();
        }
    };

    mmap_rx(const mmap_rx&) = delete;
    mmap_rx& operator=(const mmap_rx&) = delete;

    /** Construct memory mmaped RX queue */
    mmap_rx(const char* netdev, std::size_t buffer_size, std::size_t max_packet_size = 2048)
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
        /* Calculate minimum power-of-two aligned size for frames */
        req.tp_frame_size = upper_power_of_two(TPACKET_ALIGN(TPACKET2_HDRLEN) + TPACKET_ALIGN(max_packet_size));
        /* Calculate minimum contiguous pages needed to enclose a frame */
        const std::size_t page_size = getpagesize();
        const std::size_t page_nr = page_size > req.tp_frame_size ? 1 : ((req.tp_frame_size + page_size - 1) / page_size);
        /* buffer_size should be greater than page size */
        buffer_size = std::max(buffer_size, page_size);
        req.tp_block_size = page_size << int(std::ceil(std::log2(page_nr)));
        req.tp_block_nr = buffer_size / req.tp_block_size;
        req.tp_frame_nr = (req.tp_block_nr * req.tp_block_size) / req.tp_frame_size;
        frame_size_ = req.tp_frame_size;
        frame_nr_ = req.tp_frame_nr;

        /* Apply PACKET_RX_RING option */
        option_result = socket_.set_option(rx_ring_request{req});
        if (__unlikely(!option_result)) {
            throw socket_error{"Failed to set socket option PACKET_RX_RING (%s)", option_result.str()};
        }

        /* Mmap socket RX queue */
        region_ = plusone::mapped_region{socket_.get(), req.tp_block_size * req.tp_block_nr};

        /* Prepare iovec buffers */
        rd_ = static_vector< struct iovec >{req.tp_frame_nr};
        for (std::size_t i = 0; i < req.tp_frame_nr; ++i) {
            auto& entry = rd_.emplace_back();
            entry.iov_base = region_.data() + (i * req.tp_frame_size);
            entry.iov_len = req.tp_frame_size;
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

    /** Get next packet */
    __force_inline packet get() noexcept
    {
        tpacket2_hdr* header = reinterpret_cast< tpacket2_hdr* >(rd_[frame_index_].iov_base);

        if (__likely((header->tp_status & TP_STATUS_USER) == TP_STATUS_USER)) {
            /* TODO: if frame_nr_ is power-of-two -> make bitwise */
            frame_index_ = (frame_index_ + 1) % frame_nr_;
            return packet{header};
        } else {
            return packet{};
        }
    }
};

} /* namespace net */
} /* namespace plusone */

#endif /* KSERGEY_mmap_rx_240417160759 */
