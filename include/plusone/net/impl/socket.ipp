/*
 * Copyright (c) 2016-2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef MADLIFE_socket_impl_051116234321_MADLIFE
#define MADLIFE_socket_impl_051116234321_MADLIFE

#include "../result.hpp"

namespace plusone {
namespace net {

__force_inline socket::socket(socket&& s)
{ std::swap(sock_, s.sock_); }

__force_inline socket& socket::operator=(socket&& s)
{ std::swap(sock_, s.sock_); return *this; }

__force_inline socket::socket(int d)
    : sock_(d)
{}

__force_inline socket::~socket()
{ close(); }

__force_inline bool socket::valid() const noexcept
{ return sock_ != invalid_socket; }

__force_inline socket::operator bool() const noexcept
{ return valid(); }

__force_inline bool socket::operator!() const noexcept
{ return !valid(); }

__force_inline int socket::get() noexcept
{ return sock_; }

__force_inline void socket::close() noexcept
{
    if (valid()) {
        ::close(sock_);
        sock_ = invalid_socket;
    }
}

__force_inline bool socket::set_nonblock(bool flag) noexcept
{
    int flags = ::fcntl(get(), F_GETFL, 0);
    if (flags == -1) {
        flags = 0;
    }
    if (flag) {
        flags |= O_NONBLOCK;
    } else {
        flags &= ~int(O_NONBLOCK);
    }
    return 0 == ::fcntl(get(), F_SETFL, flags);
}

__force_inline bool socket::set_cloexec(bool flag) noexcept
{
    int flags = ::fcntl(get(), F_GETFL, 0);
    if (flags == -1) {
        flags = 0;
    }
    if (flag) {
        flags |= FD_CLOEXEC;
    } else {
        flags &= ~int(FD_CLOEXEC);
    }
    return 0 == ::fcntl(get(), F_SETFL, flags);
}

__force_inline socket socket::create(int family, int socktype, int protocol)
{
    int s = ::socket(family, socktype, protocol);
    if (s == invalid_socket) {
        throw socket_error("Failed to create socket");
    }
    return s;
}

__force_inline socket socket::create(const protocol& p)
{ return create(p.domain, p.type, p.proto); }

__force_inline op_result socket::connect(const sockaddr* addr, socklen_t addrlen) noexcept
{ return ::connect(get(), addr, addrlen); }

__force_inline op_result socket::bind(const sockaddr* addr, socklen_t addrlen) noexcept
{ return ::bind(get(), addr, addrlen); }

__force_inline op_result socket::bind(uint16_t port, const address_v4& addr) noexcept
{
    sockaddr_in bind_addr{};
    bind_addr.sin_family = AF_INET;
    bind_addr.sin_addr.s_addr = htonl(addr.to_ulong());
    bind_addr.sin_port = htons(port);

    return bind(reinterpret_cast< sockaddr* >(&bind_addr), sizeof(bind_addr));
}

__force_inline op_result socket::listen(int backlog) noexcept
{ return ::listen(get(), backlog); }

__force_inline accept_result socket::accept(sockaddr* addr, socklen_t* addrlen) noexcept
{ return ::accept(get(), addr, addrlen); }

__force_inline io_result socket::send(const void* buf, size_t len) noexcept
{ return ::send(get(), buf, len, 0); }

__force_inline io_result socket::sendto(const void* buf, size_t len,
        const sockaddr* dest_addr, socklen_t addrlen) noexcept
{ return ::sendto(get(), buf, len, 0, dest_addr, addrlen); }

__force_inline io_result socket::sendmsg(const msghdr* message) noexcept
{ return ::sendmsg(get(), message, 0); }

__force_inline io_result socket::recv(void* buf, size_t len) noexcept
{ return ::recv(get(), buf, len, 0); }

__force_inline io_result socket::recvfrom(void* buf, size_t len,
        sockaddr* src_addr, socklen_t* addrlen) noexcept
{ return ::recvfrom(get(), buf, len, 0, src_addr, addrlen); }

__force_inline io_result socket::recvmsg(msghdr* message) noexcept
{ return ::recvmsg(get(), message, 0); }

__force_inline io_result socket::recvmmsg(mmsghdr* msgvec, unsigned int vlen,
        timespec* timeout) noexcept
{ return ::recvmmsg(get(), msgvec, vlen, 0, timeout); }

template< typename OptionT >
__force_inline op_result socket::set_option(const OptionT& option) noexcept
{
    return ::setsockopt(get(), option.level(), option.name(),
            option.data(), option.size());
}

template< typename OptionT >
__force_inline op_result socket::get_option(OptionT& option) noexcept
{
    socklen_t size = option.size();
    op_result result = ::getsockopt(get(), option.level(), option.name(),
            option.data(), &size);
    if (result) {
        option.resize(size);
    }
    return result;
}

} /* namespace net */
} /* namespace plusone */

#endif /* MADLIFE_socket_impl_051116234321_MADLIFE */
