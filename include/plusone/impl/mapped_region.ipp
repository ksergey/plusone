/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_mapped_region_120717222922
#define KSERGEY_mapped_region_120717222922

#include <sys/mman.h>
#include <cstring>
#include <plusone/compiler.hpp>

namespace plusone {
namespace detail {

/* Get size of opened file */
__force_inline std::size_t get_size(int fd)
{
    struct stat st;
    if (::fstat(fd, &st) == -1) {
        throw file_error("Read file size error ({})", std::strerror(errno));
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

__force_inline mapped_region::mapped_region(int fd, std::size_t file_size, const options& opts)
{
    /* Prepare protection flags */
    int prot = 0;
    if (opts.mode == read_only) {
        prot = PROT_READ;
    } else if (opts.mode == read_write) {
        prot = PROT_WRITE | PROT_READ;
    } else {
        throw mmap_error("Unknown open mode ({})", std::strerror(EINVAL));
    }

    int flags = opts.shared ? MAP_SHARED : MAP_PRIVATE;
    if (opts.prefault) {
        flags |= MAP_POPULATE;
    }
    if (opts.locked) {
        flags |= MAP_LOCKED;
    }

    /* Mmap region */
    auto region = ::mmap(opts.address, file_size, prot, flags, fd, 0);
    if (region == MAP_FAILED) {
        throw mmap_error("Mmap error ({})", std::strerror(errno));
    }

    data_ = reinterpret_cast< char* >(region);
    size_ = file_size;
}

__force_inline mapped_region::mapped_region(int fd, const options& opts)
    : mapped_region(fd, detail::get_size(fd), opts)
{}

__force_inline mapped_region::mapped_region(file& f, std::size_t file_size, const options& opts)
    : mapped_region(f.handle(), file_size, opts)
{}

__force_inline mapped_region::mapped_region(file& f, const options& opts)
    : mapped_region(f, f.size(), opts)
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

__force_inline void mapped_region::fill(int c)
{
    std::memset(data_, c, size_);
}

} /* namespace plusone */

#endif /* KSERGEY_mapped_region_120717222922 */
