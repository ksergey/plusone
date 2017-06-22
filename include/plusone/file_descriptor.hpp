/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_file_descriptor_230617002324
#define KSERGEY_file_descriptor_230617002324

#include <unistd.h>
#include <algorithm>
#include "compiler.hpp"

namespace plusone {

/**
 * RAII wrapper around Linux file descriptor.
 * typically you need close file descriptor
 */
class file_descriptor
{
private:
    static constexpr int invalid_descriptor = -1;

    /* OS native file descritor */
    int fd_{invalid_descriptor};

public:
    file_descriptor(const file_descriptor&) = delete;
    file_descriptor& operator=(const file_descriptor&) = delete;

    __force_inline file_descriptor(file_descriptor&& value) noexcept
    { swap(value); }

    __force_inline file_descriptor& operator=(file_descriptor&& value) noexcept
    {
        if (__likely(this != &value)) {
            swap(value);
        }
        return *this;
    }

    file_descriptor() = default;

    __force_inline file_descriptor(int fd) noexcept
        : fd_{fd}
    {}

    __force_inline ~file_descriptor() noexcept
    {
        if (fd_ != invalid_descriptor) {
            /*
             * Actually close could return error
             */
            ::close(fd_);
        }
    }

    /**
     * Cast to OS native descriptor
     */
    __force_inline int get() noexcept
    { return fd_; }

    /**
     * Cast to OS native descriptor
     */
    __force_inline operator int() noexcept
    { return get(); }

    /**
     * Cast to bool.
     * return true if descriptor valid
     */
    __force_inline explicit operator bool() const noexcept
    { return fd_ != invalid_descriptor; }

    /**
     * Cast to bool.
     * return true if descriptor invalid
     */
    __force_inline bool operator!() const noexcept
    { return fd_ == invalid_descriptor; }

    /** Swap descriptors */
    __force_inline void swap(file_descriptor& value) noexcept
    { std::swap(fd_, value.fd_); }

    /** Replace file descriptor with another */
    __force_inline void reset(int fd = invalid_descriptor) noexcept
    {
        file_descriptor other{fd};
        swap(other);
    }
};

} /* namespace plusone */

#endif /* KSERGEY_file_descriptor_230617002324 */
