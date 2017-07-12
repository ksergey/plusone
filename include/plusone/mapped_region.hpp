/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_mapped_region_050117020149
#define KSERGEY_mapped_region_050117020149

#include "exception.hpp"
#include "file.hpp"

namespace plusone {

/** Mmap error type */
using mmap_error = tagged_exception< struct mmap_tag >;

/** Memory mapped region */
class mapped_region
{
private:
    /* Pointer to mapped data */
    char* data_{nullptr};
    /* Size of mapped data */
    std::size_t size_{0};

public:
    mapped_region(const mapped_region&) = delete;
    mapped_region& operator=(const mapped_region&) = delete;

    /** Move constructor */
    mapped_region(mapped_region&& other) noexcept;

    /** Move operator */
    mapped_region& operator=(mapped_region&& other) noexcept;

    /** Construct non-initialized mapped region */
    mapped_region() = default;

    /** Construct the mapped region from file descriptor */
    mapped_region(int fd, std::size_t file_size, open_mode mode = read_write);

    /** Construct the mapped region from file descriptor */
    explicit mapped_region(int fd, open_mode mode = read_write);

    /** Construct the mapped region from file object */
    mapped_region(file& f, std::size_t file_size, open_mode mode = read_write);

    /** Construct the mapped region from file object */
    explicit mapped_region(file& f, open_mode mode = read_write);

    /** Destructor */
    virtual ~mapped_region() noexcept;

    /** Swap content with another object */
    void swap(mapped_region& other) noexcept;

    /** Return true if mapped region initialized */
    explicit operator bool() const noexcept;

    /** Return true if mapped region non-initialized */
    bool operator!() const noexcept;

    /** Return pointer to mapped region data */
    const char* data() const noexcept;

    /** Return pointer to mapped region data */
    char* data() noexcept;

    /** Return region size */
    std::size_t size() const noexcept;

    /** Fill region memory with a constant byte */
    void fill(int c = 0);
};

} /* namespace plusone */

#include "impl/mapped_region.ipp"

#endif /* KSERGEY_mapped_region_050117020149 */
