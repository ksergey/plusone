/*
 * Copyright (c) 2016-2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef MADLIFE_socket_051116231354_MADLIFE
#define MADLIFE_socket_051116231354_MADLIFE

#include <utility>
#include "common.hpp"
#include "protocol.hpp"
#include "address_v4.hpp"

namespace plusone {
namespace net {

class op_result;
class io_result;
class accept_result;

/** Socket class */
class socket
{
private:
    /* Socket descriptor */
    int sock_{invalid_socket};

public:
    /** Disable copy constructor */
    socket(const socket&) = delete;

    /** Disable copy operator */
    socket& operator=(const socket&) = delete;

    /** Construct socket from lvalue reference */
    socket(socket&& s);

    /** Move operator */
    socket& operator=(socket&& s);

    /** Construct socket from native descriptor */
    socket(int d = invalid_socket);

    /** Socket destructor */
    ~socket();

    /** Return true if socket initialized */
    bool valid() const noexcept;

    /** Same as valid() */
    explicit operator bool() const noexcept;

    /** Same as !valid() */
    bool operator!() const noexcept;

    /** Return native socket descriptor */
    int get() noexcept;

    /** Close the socket */
    void close() noexcept;

    /** Switch the socket non-blocking mode */
    bool set_nonblock(bool flag = true) noexcept;

    /** Switch the socket cloexec mode */
    bool set_cloexec(bool flag = true) noexcept;

    /** Create socket */
    static socket create(int family, int socktype, int protocol);

    /** Create socket */
    static socket create(const protocol& p);

    /** Establish connection */
    op_result connect(const sockaddr* addr, socklen_t addrlen) noexcept;

    /** Bind socket */
    op_result bind(const sockaddr* addr, socklen_t addrlen) noexcept;

    /** Bind socket */
    op_result bind(uint16_t port, const address_v4& addr = address_v4::any()) noexcept;

    /** Place socket in a listen state */
    op_result listen(int backlog = 10) noexcept;

    /** Accept incomming connection */
    accept_result accept(sockaddr* addr = nullptr, socklen_t* addrlen = nullptr) noexcept;

    /** Send data into socket */
    io_result send(const void* buf, size_t len) noexcept;

    /** Send data into socket */
    io_result sendto(const void* buf, size_t len,
            const sockaddr* dest_addr, socklen_t addrlen) noexcept;

    /** Send data into socket */
    io_result sendmsg(const msghdr* message) noexcept;

    /** Recv data from socket */
    io_result recv(void* buf, size_t len) noexcept;

    /** Recv data from socket */
    io_result recvfrom(void* buf, size_t len,
            sockaddr* src_addr, socklen_t* addrlen) noexcept;

    /** Recv data from socket */
    io_result recvmsg(msghdr* message) noexcept;

    /** Recv multiple data from socket */
    io_result recvmmsg(mmsghdr* msgvec, unsigned int vlen,
            timespec* timeout = nullptr) noexcept;

    /** Set socket option */
    template< typename OptionT >
    op_result set_option(const OptionT& option) noexcept;

    /** Get socket option */
    template< typename OptionT >
    op_result get_option(OptionT& option) noexcept;
};

} /* namespace net */
} /* namespace plusone */

#include "impl/socket.ipp"

#endif /* MADLIFE_socket_051116231354_MADLIFE */
