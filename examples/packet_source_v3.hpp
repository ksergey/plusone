/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_packet_source_v3_240417160131
#define KSERGEY_packet_source_v3_240417160131

#include <plusone/net/socket.hpp>
#include <plusone/mapped_region.hpp>
#include <iostream>
#include <vector>
#include <poll.h>

namespace sample {

class packet_source_v3 final
{
private:
    plusone::net::socket socket_;
    plusone::mapped_region region_;
    std::vector< struct iovec > rd_;
    unsigned int block_num_{0};

    pollfd pfd_;

public:
    packet_source_v3(const packet_source_v3&) = delete;
    packet_source_v3& operator=(const packet_source_v3&) = delete;

    explicit packet_source_v3(const char* netdev);

    void run();

private:
    void setup_socket(const char* netdev);
    void setup_poll();
};

} /* namespace sample */

#endif /* KSERGEY_packet_source_v3_240417160131 */
