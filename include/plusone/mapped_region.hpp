/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_mapped_region_050117020149
#define KSERGEY_mapped_region_050117020149

#include <sys/mman.h>
#include <cstring>
#include "file.hpp"
#include "compiler.hpp"

namespace plusone {

/** mmap error type */
using mmap_error = tagged_exception< struct mmap_tag >;

namespace detail {

/** get size from opened file */
__force_inline std::size_t get_size(int fd)
{
    struct stat st;
    if (::fstat(fd, &st) == -1) {
        throw file_error("failed to read file size (%s)", std::strerror(errno));
    }
    return st.st_size;
}

} /* namespace detail */

/** memory mapped region */
class mapped_region
{
private:
    /* pointer to mapped data */
    char* data_{nullptr};
    /* size of mapped data */
    std::size_t size_{0};

public:
    mapped_region(const mapped_region&) = delete;
    mapped_region& operator=(const mapped_region&) = delete;

    /** move constructor */
    mapped_region(mapped_region&& other) noexcept
        : data_(other.data_)
        , size_(other.size_)
    {
        other.data_ = nullptr;
        other.size_ = 0;
    }

    /** move operator */
    mapped_region& operator=(mapped_region&& other) noexcept
    {
        if (this != &other) {
            std::swap(data_, other.data_);
            std::swap(size_, other.size_);
        }
        return *this;
    }

    /** construct non-initialized mapped region */
    mapped_region() = default;

    /**
     * construct the mapped region from file descriptor.
     * mmap file descriptor
     */
    mapped_region(int fd, std::size_t file_size, open_mode mode = read_write)
    {
        /* prepare protection flags */
        int prot = 0;
        if (mode == read_only) {
            prot = PROT_READ;
        } else if (mode == read_write) {
            prot = PROT_WRITE | PROT_READ;
        } else {
            throw mmap_error("unknown open mode (%s)", std::strerror(EINVAL));
        }

        /* mmap mapped_region */
        auto mapped_region = ::mmap(nullptr, file_size, prot, MAP_SHARED | MAP_POPULATE,
                fd, 0);
        if (mapped_region == MAP_FAILED) {
            throw mmap_error("mmap error (%s)", std::strerror(errno));
        }

        /* construct mapped_regions */
        data_ = reinterpret_cast< char* >(mapped_region);
        size_ = file_size;
    }

    /**
     * construct the mapped region from file descriptor.
     * mmap file descriptor
     */
    explicit mapped_region(int fd, open_mode mode = read_write)
        : mapped_region(fd, detail::get_size(fd), mode)
    {}

    /**
     * construct the mapped region from file.
     * mmap file descriptor
     */
    mapped_region(file& f, std::size_t file_size, open_mode mode = read_write)
        : mapped_region(f.handle(), file_size, mode)
    {}

    /**
     * construct the mapped region from file.
     * mmap file descriptor
     */
    explicit mapped_region(file& f, open_mode mode = read_write)
        : mapped_region(f, f.size(), mode)
    {}

    /** mapped_region destructor */
    virtual ~mapped_region() noexcept
    {
        if (data_) {
            ::munmap(data_, size_);
        }
    }

    /** swap content of the mapped_region */
    void swap(mapped_region& other) noexcept
    {
        std::swap(data_, other.data_);
        std::swap(size_, other.size_);
    }

    /** return true if mapped_region initialized */
    __force_inline explicit operator bool() const noexcept
    { return data_ != nullptr; }

    /** return true if mapped_region non-initialized */
    __force_inline bool operator!() const noexcept
    { return data_ == nullptr; }

    /**
     * return pointer to mapped_region data.
     * read only
     */
    __force_inline const char* data() const noexcept
    { return data_; }

    /**
     * return pointer to mapped_region data.
     * for modification
     */
    __force_inline char* data() noexcept
    { return data_; }

    /** return mapped_region size */
    __force_inline std::size_t size() const noexcept
    { return size_; }

    /** fill region memory with a constant byte */
    __force_inline void fill(int c = 0)
    { std::memset(data_, c, size_); }
};

} /* namespace plusone */

#endif /* KSERGEY_mapped_region_050117020149 */
