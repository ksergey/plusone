/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_mmap_rx_120717234654
#define KSERGEY_mmap_rx_120717234654

#include <net/if.h>
#include <unistd.h>
#include <cmath>
#include <type_traits>
#include <plusone/expect.hpp>

namespace plusone {
namespace net {

class mmap_rx::frame
{
private:
    tpacket2_hdr* data_{nullptr};

public:
    /** Move constructor */
    __force_inline frame(frame&& v) noexcept
    {
        std::swap(v.data_, data_);
    }

    /** Move operator */
    __force_inline frame& operator=(frame&& v) noexcept
    {
        std::swap(v.data_, data_);
        return *this;
    }

    /** Construct non-valid frame */
    frame() = default;

    /** Construct frame from native header */
    __force_inline frame(tpacket2_hdr* data)
        : data_{data}
    {}

    /** Destructor */
    __force_inline ~frame()
    {
        if (data_) {
            /* Make frame available for kernel */
            data_->tp_status = TP_STATUS_KERNEL;
            __sync_synchronize();
        }
    }

    /** Return true if frame valid */
    __force_inline explicit operator bool() const noexcept
    {
        return data_ != nullptr;
    }

    /** Return frame receiving timestamp */
    __force_inline std::uint32_t sec() const noexcept
    {
        __expect( data_ );
        return data_->tp_sec;
    }

    /** Return frame receiving timestamp (nanosecond part) */
    __force_inline std::uint32_t nsec() const noexcept
    {
        __expect( data_ );
        return data_->tp_nsec;
    }

    /** Return frame data (started from ip header) */
    __force_inline const std::uint8_t* data() const noexcept
    {
        __expect( data_ );
        return reinterpret_cast< const std::uint8_t* >(data_) + data_->tp_mac;
    }

    /** Return frame data size */
    __force_inline std::size_t size() const noexcept
    {
        __expect( data_ );
        return data_->tp_len;
    }

    /** Return struct as required type */
    template< class T >
    __force_inline const T& as(std::size_t offset = 0) const noexcept
    {
        static_assert( std::is_trivially_copyable< T >::value, "class T must be trivially copyable" );
        __expect( data_ && offset + sizeof(T) <= size() );
        return *reinterpret_cast< const T* >(data() + offset);
    }
};

__force_inline mmap_rx::mmap_rx(const char* netdev, std::size_t buffer_size, std::size_t max_packet_size)
{
    /* Create socket */
    socket_ = socket::create(AF_PACKET, SOCK_DGRAM, htons(ETH_P_IP));

    /* Apply timestamp option */
    auto option_result = socket_.set_option(socket_options::packet::timestamp{timestamp_option});
    if (__unlikely(!option_result)) {
        throw_ex< socket_error >("Failed to set socket option PACKET_TIMESTAMP ({})", option_result.str());
    }

    /* Apply TPACKET_V2 option */
    option_result = socket_.set_option(socket_options::packet::version{TPACKET_V2});
    if (__unlikely(!option_result)) {
        throw_ex< socket_error >("Failed to set socket option PACKET_VERSION ({})", option_result.str());
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
        throw_ex< socket_error >("Failed to set socket option PACKET_RX_RING ({})", option_result.str());
    }

    /* Mmap socket RX queue */
    region_ = plusone::mapped_region{socket_.native_handle(), req.tp_block_size * req.tp_block_nr};

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
            throw_ex< socket_error >("Interface \"{}\" not found", netdev);
        }
    } else {
        ll.sll_ifindex = 0; /* All interfaces */
    }
    ll.sll_hatype = 0;
    ll.sll_pkttype = 0;
    ll.sll_halen = 0;
    auto bind_result = socket_.bind((sockaddr*)& ll, sizeof(ll));
    if (!bind_result) {
        throw_ex< socket_error >("Failed to bind mmap_rx socket ({})", bind_result.str());
    }
}

__force_inline mmap_rx::frame mmap_rx::get_next_frame() noexcept
{
    tpacket2_hdr* header = reinterpret_cast< tpacket2_hdr* >(rd_[frame_index_].iov_base);
    if (__likely((header->tp_status & TP_STATUS_USER) == TP_STATUS_USER)) {
        /* TODO: if frame_nr_ is power-of-two -> make bitwise */
        frame_index_ = (frame_index_ + 1) % frame_nr_;
        return frame{header};
    } else {
        __sync_synchronize();
        return frame{};
    }
}

} /* namespace net */
} /* namespace plusone */

#endif /* KSERGEY_mmap_rx_120717234654 */
