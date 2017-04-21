/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#include <plusone/net/socket.hpp>
#include <plusone/mapped_region.hpp>
#include <iostream>
#include <vector>
#include <poll.h>

namespace sample {

class packet_source final
{
private:
    plusone::net::socket socket_;
    plusone::mapped_region region_;
    struct tpacket_req3 req_;
    //std::vector< struct iovec > rd_;
    struct iovec* rd_{nullptr};
    unsigned int block_num_{0};

    pollfd pfd_;

public:
    packet_source(const packet_source&) = delete;
    packet_source& operator=(const packet_source&) = delete;

    explicit packet_source(const char* netdev);

    void run();

private:
    void setup_socket(const char* netdev);
    void setup_poll();
};

} /* namespace sample */
