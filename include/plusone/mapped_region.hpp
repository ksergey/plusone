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

/** Mmap error type */
using mmap_error = tagged_exception< struct mmap_tag >;

namespace detail {

/** Get size of opened file */
__force_inline std::size_t get_size(int fd)
{
    struct stat st;
    if (::fstat(fd, &st) == -1) {
        throw file_error("Read file size error (%s)", std::strerror(errno));
    }
    return st.st_size;
}

} /* namespace detail */

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
    mapped_region(mapped_region&& other) noexcept
        : data_(other.data_)
        , size_(other.size_)
    {
        other.data_ = nullptr;
        other.size_ = 0;
    }

    /** Move operator */
    __force_inline mapped_region& operator=(mapped_region&& other) noexcept
    {
        if (this != &other) {
            std::swap(data_, other.data_);
            std::swap(size_, other.size_);
        }
        return *this;
    }

    /** Construct non-initialized mapped region */
    mapped_region() = default;

    /** Construct the mapped region from file descriptor */
    mapped_region(int fd, std::size_t file_size, open_mode mode = read_write)
    {
        /* Prepare protection flags */
        int prot = 0;
        if (mode == read_only) {
            prot = PROT_READ;
        } else if (mode == read_write) {
            prot = PROT_WRITE | PROT_READ;
        } else {
            throw mmap_error("Unknown open mode (%s)", std::strerror(EINVAL));
        }

        /* Mmap region */
        auto region = ::mmap(nullptr, file_size, prot,
                MAP_SHARED | MAP_POPULATE | MAP_LOCKED, fd, 0);
        if (region == MAP_FAILED) {
            throw mmap_error("Mmap error (%s)", std::strerror(errno));
        }

        data_ = reinterpret_cast< char* >(region);
        size_ = file_size;
    }

    /** Construct the mapped region from file descriptor */
    explicit mapped_region(int fd, open_mode mode = read_write)
        : mapped_region(fd, detail::get_size(fd), mode)
    {}

    /** Construct the mapped region from file object */
    mapped_region(file& f, std::size_t file_size, open_mode mode = read_write)
        : mapped_region(f.handle(), file_size, mode)
    {}

    /** Construct the mapped region from file object */
    explicit mapped_region(file& f, open_mode mode = read_write)
        : mapped_region(f, f.size(), mode)
    {}

    /** Destructor */
    virtual ~mapped_region() noexcept
    {
        if (data_) {
            ::munmap(data_, size_);
        }
    }

    /** Swap content with another object */
    __force_inline void swap(mapped_region& other) noexcept
    {
        std::swap(data_, other.data_);
        std::swap(size_, other.size_);
    }

    /** Return true if mapped region initialized */
    __force_inline explicit operator bool() const noexcept
    { return data_ != nullptr; }

    /** Return true if mapped region non-initialized */
    __force_inline bool operator!() const noexcept
    { return data_ == nullptr; }

    /** Return pointer to mapped region data */
    __force_inline const char* data() const noexcept
    { return data_; }

    /** Return pointer to mapped region data */
    __force_inline char* data() noexcept
    { return data_; }

    /** Return region size */
    __force_inline std::size_t size() const noexcept
    { return size_; }

    /** Fill region memory with a constant byte */
    __force_inline void fill(int c = 0)
    { std::memset(data_, c, size_); }
};

} /* namespace plusone */

#endif /* KSERGEY_mapped_region_050117020149 */
