/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#include "packet_source.hpp"
#include <net/if.h>
#include <linux/ip.h>
#include <unistd.h>
#include <cstring>
#include <cassert>
#include <cmath>
#include <plusone/net/socket_option.hpp>

namespace sample {

struct block_desc {
	uint32_t version;
	uint32_t offset_to_priv;
	struct tpacket_hdr_v1 h1;
};

using generic_socket_option = plusone::net::detail::generic_socket_option;

static void flush_block(block_desc* pbd)
{ pbd->h1.block_status = TP_STATUS_KERNEL; }

static void display(struct tpacket3_hdr *ppd)
{
	struct iphdr *ip = (struct iphdr *) ((uint8_t *) ppd + ppd->tp_mac);

    struct sockaddr_in ss, sd;
    char sbuff[NI_MAXHOST], dbuff[NI_MAXHOST];

    memset(&ss, 0, sizeof(ss));
    ss.sin_family = PF_INET;
    ss.sin_addr.s_addr = ip->saddr;
    getnameinfo((struct sockaddr *) &ss, sizeof(ss),
            sbuff, sizeof(sbuff), NULL, 0, NI_NUMERICHOST);

    memset(&sd, 0, sizeof(sd));
    sd.sin_family = PF_INET;
    sd.sin_addr.s_addr = ip->daddr;
    getnameinfo((struct sockaddr *) &sd, sizeof(sd),
            dbuff, sizeof(dbuff), NULL, 0, NI_NUMERICHOST);

    printf("%s -> %s, ", sbuff, dbuff);

	printf("rxhash: 0x%x\n", ppd->hv1.tp_rxhash);
}

static void walk_block(struct block_desc* pbd, const int block_num)
{
	int num_pkts = pbd->h1.num_pkts, i;
	unsigned long bytes = 0;
	struct tpacket3_hdr *ppd;

	ppd = (struct tpacket3_hdr *) ((uint8_t *) pbd +
				       pbd->h1.offset_to_first_pkt);
	for (i = 0; i < num_pkts; ++i) {
		bytes += ppd->tp_snaplen;
		display(ppd);

		ppd = (struct tpacket3_hdr *) ((uint8_t *) ppd +
					       ppd->tp_next_offset);
	}
}

packet_source::packet_source(const char* netdev)
{
    std::cout << netdev << '\n';
    setup_socket(netdev);
    setup_poll();
}

void packet_source::run()
{
	unsigned int blocks = 64;

    struct block_desc* pbd = (struct block_desc *) rd_[block_num_].iov_base;

    //std::cout << index_ << ' ' << pbd->h1.block_status << ' ' << TP_STATUS_USER << '\n';

    if ((pbd->h1.block_status & TP_STATUS_USER) == 0) {
        auto rc = ::poll(&pfd_, 1, -1);
        return;
    }

    std::cout << block_num_ << " xxx\n";

    walk_block(pbd, block_num_);
    flush_block(pbd);
    block_num_ = (block_num_ + 1) %blocks;
}

void packet_source::setup_socket(const char* netdev)
{
	unsigned int blocksiz = 1 << 22, framesiz = 1 << 11;
	unsigned int blocknum = 64;

    socket_ = plusone::net::socket::create(AF_PACKET, SOCK_DGRAM, htons(ETH_P_IP));

    auto option_result = socket_.set_option(plusone::net::packet_version{TPACKET_V3});
    //int val = TPACKET_V3;
    //auto option_result = socket_.set_option(generic_socket_option(SOL_PACKET, PACKET_VERSION, val));
    if (__unlikely(!option_result)) {
        throw std::runtime_error("TPACKET_V3");
    }

    std::memset(&req_, 0, sizeof(req_));
    req_.tp_block_size = blocksiz;
    req_.tp_frame_size = framesiz;
    req_.tp_block_nr = blocknum;
	req_.tp_frame_nr = (blocksiz * blocknum) / framesiz;
	req_.tp_retire_blk_tov = 60;
	req_.tp_feature_req_word = TP_FT_REQ_FILL_RXHASH;
    option_result = socket_.set_option(generic_socket_option(SOL_PACKET, PACKET_RX_RING, &req_, sizeof(req_)));
    if (__unlikely(!option_result)) {
        throw std::runtime_error("PACKET_RX_RING failed");
    }

    region_ = plusone::mapped_region{socket_.get(), req_.tp_block_size * req_.tp_block_nr};
    assert( region_.data() != nullptr );
    std::cout << region_.size() << '\n';
    //rd_.resize(req_.tp_block_nr);
	rd_ = (struct iovec*) malloc(req_.tp_block_nr * sizeof(*rd_));
    std::cout << req_.tp_block_nr << '\n';
    assert( rd_ );
    for (std::size_t i = 0; i < req_.tp_block_nr; ++i) {
        rd_[i].iov_base = region_.data() + (i * req_.tp_block_size);
        rd_[i].iov_len = req_.tp_block_size;
    }

    struct sockaddr_ll ll;
    std::memset(&ll, 0, sizeof(ll));
	ll.sll_family = PF_PACKET;
	ll.sll_protocol = htons(ETH_P_IP);
	ll.sll_ifindex = if_nametoindex(netdev);
	ll.sll_hatype = 0;
	ll.sll_pkttype = 0;
	ll.sll_halen = 0;
    auto bind_result = socket_.bind((sockaddr*)& ll, sizeof(ll));
    if (!bind_result) {
        throw std::runtime_error("Failed to bind socket (" + std::string(bind_result.str()) + ")");
    }

    assert(socket_);
}

void packet_source::setup_poll()
{
    std::memset(&pfd_, 0, sizeof(pfd_));
    pfd_.fd = socket_.get();
    pfd_.events = POLLIN | POLLERR;
}

} /* namespace sample */
