/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_file_descriptor_230617002324
#define KSERGEY_file_descriptor_230617002324

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

    file_descriptor(file_descriptor&& value) noexcept;
    file_descriptor& operator=(file_descriptor&& value) noexcept;

    file_descriptor() = default;

    file_descriptor(int fd) noexcept;
    ~file_descriptor() noexcept;

    /**
     * Cast to OS native descriptor
     */
    int get() noexcept;

    /**
     * Cast to OS native descriptor
     */
    operator int() noexcept;

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
};

} /* namespace plusone */

#include "impl/file_descriptor.ipp"

#endif /* KSERGEY_file_descriptor_230617002324 */
