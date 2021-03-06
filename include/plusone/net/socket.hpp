/*
 * Copyright (c) 2016-2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef MADLIFE_socket_051116231354_MADLIFE
#define MADLIFE_socket_051116231354_MADLIFE

#include <utility>
#include <plusone/buffer_base.hpp>
#include <plusone/net/common.hpp>
#include <plusone/net/protocol.hpp>
#include <plusone/net/address_v4.hpp>

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
    /** Native socket handle type */
    using native_handle_type = int;

    /** Move constructor */
    socket(socket&& s) noexcept;

    /** Move operator */
    socket& operator=(socket&& s) noexcept;

    /** Construct socket from native descriptor */
    socket(int d = invalid_socket);

    /** Socket destructor */
    ~socket();

    /** Return true if socket is valid for i/o */
    explicit operator bool() const noexcept;

    /** Return native socket descriptor */
    native_handle_type native_handle() noexcept;

    /** Close the socket */
    void close() noexcept;

    /** Switch the socket non-blocking mode */
    bool set_nonblock(bool flag = true) noexcept;

    /** Switch the socket cloexec mode */
    bool set_cloexec(bool flag = true) noexcept;

    /**
     * Create socket
     * @throw socket_error in case of error
     */
    static socket create(int family, int socktype, int protocol);

    /**
     * Create socket
     * @throw socket_error in case of error
     */
    static socket create(const protocol& p);

    /** Establish connection */
    op_result connect(const sockaddr* addr, socklen_t addrlen) noexcept;

    /** Bind socket */
    op_result bind(const sockaddr* addr, socklen_t addrlen) noexcept;

    /** Bind socket */
    op_result bind(std::uint16_t port, const address_v4& addr = address_v4::any()) noexcept;

    /** Place socket in a listen state */
    op_result listen(int backlog = 10) noexcept;

    /** Accept incomming connection */
    accept_result accept(sockaddr* addr = nullptr, socklen_t* addrlen = nullptr) noexcept;

    /** Send data into socket */
    io_result send(const void* buf, std::size_t len) noexcept;

    /** Send data into socket */
    io_result send(const const_buffer& buf);

    /** Send data into socket */
    io_result sendto(const void* buf, std::size_t len, const sockaddr* dest_addr, socklen_t addrlen) noexcept;

    /** Send data into socket */
    io_result sendto(const const_buffer& buf, const sockaddr* dest_addr, socklen_t addrlen);

    /** Send data into socket */
    io_result sendmsg(const msghdr* message) noexcept;

    /** Recv data from socket */
    io_result recv(void* buf, std::size_t len) noexcept;

    /** Recv data from socket */
    io_result recv(const mutable_buffer& buf);

    /** Recv data from socket */
    io_result recvfrom(void* buf, std::size_t len, sockaddr* src_addr, socklen_t* addrlen) noexcept;

    /** Recv data from socket */
    io_result recvfrom(const mutable_buffer& buf, sockaddr* src_addr, socklen_t* addrlen);

    /** Recv data from socket */
    io_result recvmsg(msghdr* message) noexcept;

    /** Recv multiple data from socket */
    io_result recvmmsg(mmsghdr* msgvec, unsigned int vlen, timespec* timeout = nullptr) noexcept;

    /** Set socket option */
    template< typename OptionT >
    op_result set_option(const OptionT& option) noexcept;

    /** Get socket option */
    template< typename OptionT >
    op_result get_option(OptionT& option) noexcept;

    /** Swap two sockets */
    void swap(socket& v) noexcept;
};

} /* namespace net */
} /* namespace plusone */

#include <plusone/net/impl/socket.ipp>

#endif /* MADLIFE_socket_051116231354_MADLIFE */
