/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_file_descriptor_230617002324
#define KSERGEY_file_descriptor_230617002324

#include <plusone/exception.hpp>

namespace plusone {

using file_descriptor_error = tagged_exception< struct file_descriptor_tag >;

/**
 * RAII wrapper around Linux file descriptor.
 * typically you need close file descriptor
 */
class file_descriptor
{
public:
    static constexpr int invalid_descriptor = -1;

private:
    /* OS native file descritor */
    int fd_{invalid_descriptor};

public:
    file_descriptor(const file_descriptor&) = delete;
    file_descriptor& operator=(const file_descriptor&) = delete;

    file_descriptor(file_descriptor&& value) noexcept;
    file_descriptor& operator=(file_descriptor&& value) noexcept;

    file_descriptor() = default;

    file_descriptor(int fd) noexcept;
    ~file_descriptor() noexcept;

    /** Cast to OS native descriptor */
    int get() const noexcept;

    /** Cast to OS native descriptor */
    operator int() const noexcept;

    /**
     * Cast to bool.
     * return true if descriptor valid
     */
    explicit operator bool() const noexcept;

    /**
     * Cast to bool.
     * return true if descriptor invalid
     */
    bool operator!() const noexcept;

    /** Swap descriptors */
    void swap(file_descriptor& value) noexcept;

    /** Replace file descriptor with another */
    void reset(int fd = invalid_descriptor) noexcept;

    /** Duplicate file descriptor */
    file_descriptor dup() const;
};

} /* namespace plusone */

#include <plusone/impl/file_descriptor.ipp>

#endif /* KSERGEY_file_descriptor_230617002324 */
