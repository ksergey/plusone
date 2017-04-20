/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_packet_source_200417175433
#define KSERGEY_packet_source_200417175433

#include <plusone/net/socket.hpp>

namespace better_poll {

class packet_source
{
private:
    plusone::net::socket socket_;

public:
    packet_source(const packet_source&) = delete;
    packet_source& operator=(const packet_source&) = delete;

    packet_source(const char* iface = nullptr, std::size_t socket_buffer_size = 0);

private:
    void apply_socket_options(std::size_t socket_buffer_size);
};

} /* namespace better_poll */

#endif /* KSERGEY_packet_source_200417175433 */
