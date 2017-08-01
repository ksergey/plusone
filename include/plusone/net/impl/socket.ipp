/*
 * Copyright (c) 2016-2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef MADLIFE_socket_impl_051116234321_MADLIFE
#define MADLIFE_socket_impl_051116234321_MADLIFE

#include <plusone/net/result.hpp>

namespace plusone {
namespace net {

__force_inline socket::socket(socket&& s) noexcept
{
    swap(s);
}

__force_inline socket& socket::operator=(socket&& s) noexcept
{
    if (__likely(this != &s)) {
        swap(s);
    }
    return *this;
}

__force_inline socket::socket(int d)
    : sock_(d)
{}

__force_inline socket::~socket()
{
    close();
}

__force_inline socket::operator bool() const noexcept
{
    return sock_ != invalid_socket;
}

__force_inline socket::native_handle_type socket::native_handle() noexcept
{
    return sock_;
}

__force_inline void socket::close() noexcept
{
    if (operator bool()) {
        ::close(sock_);
        sock_ = invalid_socket;
    }
}

__force_inline bool socket::set_nonblock(bool flag) noexcept
{
    int flags = ::fcntl(native_handle(), F_GETFL, 0);
    if (flags == -1) {
        flags = 0;
    }
    if (flag) {
        flags |= O_NONBLOCK;
    } else {
        flags &= ~int(O_NONBLOCK);
    }
    return 0 == ::fcntl(native_handle(), F_SETFL, flags);
}

__force_inline bool socket::set_cloexec(bool flag) noexcept
{
    int flags = ::fcntl(native_handle(), F_GETFL, 0);
    if (flags == -1) {
        flags = 0;
    }
    if (flag) {
        flags |= FD_CLOEXEC;
    } else {
        flags &= ~int(FD_CLOEXEC);
    }
    return 0 == ::fcntl(native_handle(), F_SETFL, flags);
}

__force_inline socket socket::create(int family, int socktype, int protocol)
{
    int s = ::socket(family, socktype, protocol);
    if (s == invalid_socket) {
        throw socket_error("Socket create error ({})", std::strerror(errno));
    }
    return s;
}

__force_inline socket socket::create(const protocol& p)
{
    return create(p.domain, p.type, p.proto);
}

__force_inline op_result socket::connect(const sockaddr* addr, socklen_t addrlen) noexcept
{
    return ::connect(native_handle(), addr, addrlen);
}

__force_inline op_result socket::bind(const sockaddr* addr, socklen_t addrlen) noexcept
{
    return ::bind(native_handle(), addr, addrlen);
}

__force_inline op_result socket::bind(uint16_t port, const address_v4& addr) noexcept
{
    sockaddr_in bind_addr{};
    bind_addr.sin_family = AF_INET;
    bind_addr.sin_addr.s_addr = htonl(addr.to_ulong());
    bind_addr.sin_port = htons(port);

    return bind(reinterpret_cast< sockaddr* >(&bind_addr), sizeof(bind_addr));
}

__force_inline op_result socket::listen(int backlog) noexcept
{
    return ::listen(native_handle(), backlog);
}

__force_inline accept_result socket::accept(sockaddr* addr, socklen_t* addrlen) noexcept
{
    return ::accept(native_handle(), addr, addrlen);
}

__force_inline io_result socket::send(const void* buf, size_t len) noexcept
{
    return ::send(native_handle(), buf, len, 0);
}

__force_inline io_result socket::sendto(const void* buf, size_t len,
        const sockaddr* dest_addr, socklen_t addrlen) noexcept
{
    return ::sendto(native_handle(), buf, len, 0, dest_addr, addrlen);
}

__force_inline io_result socket::sendmsg(const msghdr* message) noexcept
{
    return ::sendmsg(native_handle(), message, 0);
}

__force_inline io_result socket::recv(void* buf, size_t len) noexcept
{
    return ::recv(native_handle(), buf, len, 0);
}

__force_inline io_result socket::recvfrom(void* buf, size_t len, sockaddr* src_addr, socklen_t* addrlen) noexcept
{
    return ::recvfrom(native_handle(), buf, len, 0, src_addr, addrlen);
}

__force_inline io_result socket::recvmsg(msghdr* message) noexcept
{
    return ::recvmsg(native_handle(), message, 0);
}

__force_inline io_result socket::recvmmsg(mmsghdr* msgvec, unsigned int vlen, timespec* timeout) noexcept
{
    return ::recvmmsg(native_handle(), msgvec, vlen, 0, timeout);
}

template< typename OptionT >
__force_inline op_result socket::set_option(const OptionT& option) noexcept
{
    return ::setsockopt(native_handle(), option.level(), option.name(), option.data(), option.size());
}

template< typename OptionT >
__force_inline op_result socket::get_option(OptionT& option) noexcept
{
    socklen_t size = option.size();
    op_result result = ::getsockopt(native_handle(), option.level(), option.name(), option.data(), &size);
    if (result) {
        option.resize(size);
    }
    return result;
}

__force_inline void socket::swap(socket& v) noexcept
{
    std::swap(v.sock_, sock_);
}

} /* namespace net */
} /* namespace plusone */

#endif /* MADLIFE_socket_impl_051116234321_MADLIFE */
