/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_mapped_region_120717222922
#define KSERGEY_mapped_region_120717222922

#include <sys/mman.h>
#include <cstring>
#include "../compiler.hpp"

namespace plusone {
namespace detail {

/* Get size of opened file */
__force_inline std::size_t get_size(int fd)
{
    struct stat st;
    if (::fstat(fd, &st) == -1) {
        throw file_error("Read file size error (%s)", std::strerror(errno));
    }
    return st.st_size;
}

} /* namespace detail */

__force_inline mapped_region::mapped_region(mapped_region&& other) noexcept
{
    swap(other);
}

__force_inline mapped_region& mapped_region::operator=(mapped_region&& other) noexcept
{
    if (this != &other) {
        swap(other);
    }
    return *this;
}

__force_inline mapped_region::mapped_region(int fd, std::size_t file_size, open_mode mode)
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

__force_inline mapped_region::mapped_region(int fd, open_mode mode)
    : mapped_region(fd, detail::get_size(fd), mode)
{}

__force_inline mapped_region::mapped_region(file& f, std::size_t file_size, open_mode mode)
    : mapped_region(f.handle(), file_size, mode)
{}

__force_inline mapped_region::mapped_region(file& f, open_mode mode)
    : mapped_region(f, f.size(), mode)
{}

__force_inline mapped_region::~mapped_region() noexcept
{
    if (data_) {
        ::munmap(data_, size_);
    }
}

__force_inline void mapped_region::swap(mapped_region& other) noexcept
{
    std::swap(data_, other.data_);
    std::swap(size_, other.size_);
}

__force_inline mapped_region::operator bool() const noexcept
{
    return data_ != nullptr;
}

__force_inline bool mapped_region::operator!() const noexcept
{
    return data_ == nullptr;
}

__force_inline const char* mapped_region::data() const noexcept
{
    return data_;
}

__force_inline char* mapped_region::data() noexcept
{
    return data_;
}

__force_inline std::size_t mapped_region::size() const noexcept
{
    return size_;
}

__force_inline void mapped_region::fill(int c)
{
    std::memset(data_, c, size_);
}

} /* namespace plusone */

#endif /* KSERGEY_mapped_region_120717222922 */
