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

/** socket class */
class socket
{
private:
    /* socket handler */
    sock_t sock_{invalid_socket};

public:
    /** disable copy constructor */
    socket(const socket&) = delete;

    /** disable copy operator */
    socket& operator=(const socket&) = delete;

    /** construct socket from lvalue reference */
    socket(socket&& s);

    /** move operator */
    socket& operator=(socket&& s);

    /** construct socket from native descriptor */
    socket(sock_t d = invalid_socket);

    /** socket destructor */
    ~socket();

    /** return true if socket initialized */
    bool valid() const noexcept;

    /** same as valid() */
    explicit operator bool() const noexcept;

    /** same as !valid() */
    bool operator!() const noexcept;

    /**  return native socket handler */
    sock_t get() noexcept;

    /** close the socket */
    void close() noexcept;

    /** switch the socket non-blocking mode */
    bool set_nonblock(bool flag = true) noexcept;

    /** switch the socket cloexec mode */
    bool set_cloexec(bool flag = true) noexcept;

    /** create socket */
    static socket create(int family, int socktype, int protocol);

    /** create socket */
    static socket create(const protocol& p);

    /** establish connection */
    op_result connect(const sockaddr* addr, socklen_t addrlen) noexcept;

    /** bind socket */
    op_result bind(const sockaddr* addr, socklen_t addrlen) noexcept;

    /** bind socket */
    op_result bind(uint16_t port, const address_v4& addr = address_v4::any()) noexcept;

    /** place socket in a listen state */
    op_result listen(int backlog = 10) noexcept;

    /** accept incomming connection */
    accept_result accept(sockaddr* addr = nullptr, socklen_t* addrlen = nullptr) noexcept;

    /** send data into socket */
    io_result send(const void* buf, size_t len) noexcept;

    /** send data into socket */
    io_result sendto(const void* buf, size_t len,
            const sockaddr* dest_addr, socklen_t addrlen) noexcept;

    /** send data into socket */
    io_result sendmsg(const msghdr* message) noexcept;

    /** recv data from socket */
    io_result recv(void* buf, size_t len) noexcept;

    /** recv data from socket */
    io_result recvfrom(void* buf, size_t len,
            sockaddr* src_addr, socklen_t* addrlen) noexcept;

    /** recv data from socket */
    io_result recvmsg(msghdr* message) noexcept;

    /** recv multiple data from socket */
    io_result recvmmsg(mmsghdr* msgvec, unsigned int vlen,
            timespec* timeout = nullptr) noexcept;

    /** set socket option */
    template< typename OptionT >
    op_result set_option(const OptionT& option) noexcept;

    /** get socket option */
    template< typename OptionT >
    op_result get_option(OptionT& option) noexcept;
};

} /* namespace net */
} /* namespace plusone */

#include "socket_impl.hpp"

#endif /* MADLIFE_socket_051116231354_MADLIFE */
