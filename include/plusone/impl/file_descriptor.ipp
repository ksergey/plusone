/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_file_descriptor_130717072034
#define KSERGEY_file_descriptor_130717072034

#include <unistd.h>
#include <cstring>
#include <cerrno>
#include <algorithm>
#include <plusone/compiler.hpp>

namespace plusone {

__force_inline file_descriptor::file_descriptor(file_descriptor&& value) noexcept
{
    swap(value);
}

__force_inline file_descriptor& file_descriptor::operator=(file_descriptor&& value) noexcept
{
    if (__likely(this != &value)) {
        swap(value);
    }
    return *this;
}

__force_inline file_descriptor::file_descriptor(int fd) noexcept
    : fd_{fd}
{}

__force_inline file_descriptor::~file_descriptor() noexcept
{
    if (fd_ != invalid_descriptor) {
        ::close(fd_);
    }
}

__force_inline int file_descriptor::get() const noexcept
{
    return fd_;
}

__force_inline file_descriptor::operator int() const noexcept
{
    return get();
}

__force_inline file_descriptor::operator bool() const noexcept
{
    return fd_ != invalid_descriptor;
}

__force_inline bool file_descriptor::operator!() const noexcept
{
    return fd_ == invalid_descriptor;
}

__force_inline void file_descriptor::swap(file_descriptor& value) noexcept
{
    std::swap(fd_, value.fd_);
}

__force_inline void file_descriptor::reset(int fd) noexcept
{
    file_descriptor other{fd};
    swap(other);
}

__force_inline file_descriptor file_descriptor::dup() const
{
    if (operator bool()) {
        int fd = ::dup(fd_);
        if (__unlikely(fd == -1)) {
            throw file_descriptor_error("Failed duplicate descriptor ({})", std::strerror(errno));
        }
        return file_descriptor{fd};
    }
    return file_descriptor{};
}

} /* namespace plusone */

#endif /* KSERGEY_file_descriptor_130717072034 */
