/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_file_120717222113
#define KSERGEY_file_120717222113

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cerrno>
#include <cstring>
#include <utility>
#include "../compiler.hpp"

namespace plusone {

__force_inline file::file(file&& other) noexcept
    : fd_(other.fd_)
{
    other.fd_ = -1;
}

__force_inline file& file::operator=(file&& other) noexcept
{
    std::swap(fd_, other.fd_);
    return *this;
}

__force_inline file::file(const create_only_tag&, const std::string& path, open_mode mode,
        int perms)
{
    init(do_create, path, mode, perms);
}

__force_inline file::file(const open_only_tag&, const std::string& path, open_mode mode)
{
    init(do_open, path, mode);
}

__force_inline file::file(const open_or_create_tag&, const std::string& path, open_mode mode,
        int perms)
{
    init(do_open_or_create, path, mode, perms);
}

__force_inline file::~file() noexcept
{
    if (fd_ >= 0) {
        ::close(fd_);
    }
}

__force_inline file::operator bool() const noexcept
{
    return fd_ >= 0;
}

__force_inline bool file::operator!() const noexcept
{
    return fd_ < 0;
}

__force_inline int file::handle() const noexcept
{
    return fd_;
}

__force_inline std::size_t file::size() const
{
    struct stat st;
    if (::fstat(fd_, &st) == -1) {
        throw file_error("Read file size error ({})", std::strerror(errno));
    }
    return st.st_size;
}

__force_inline void file::truncate(std::size_t size)
{
    if (::ftruncate(fd_, size) == -1) {
        throw file_error("Truncate file error({})", std::strerror(errno));
    }
}

__force_inline void file::init(what_do what, const std::string& path, open_mode mode,
        int perms)
{
    /* Setup open flags */
    int oflags = 0;
    if (mode == read_only) {
        oflags |= O_RDONLY;
    } else if (mode == read_write) {
        oflags |= O_RDWR;
    } else {
        throw file_error("Unknown open mode ({})", std::strerror(EINVAL));
    }

    /* Open file */
    switch (what) {
        case do_open:
            {
                fd_ = ::open(path.c_str(), oflags, perms);
            }
            break;
        case do_create:
            {
                oflags |= (O_CREAT | O_EXCL);
                fd_ = ::open(path.c_str(), oflags, perms);
                if (fd_ >= 0) {
                    ::fchmod(fd_, perms);
                }
            }
            break;
        case do_open_or_create:
            {
                while (true) {
                    fd_ = ::open(path.c_str(), oflags | O_CREAT | O_EXCL, perms);
                    if (fd_ >= 0) {
                        ::fchmod(fd_, perms);
                    } else if (errno == EEXIST) {
                        fd_ = ::open(path.c_str(), oflags, perms);
                        if (fd_ < 0 && errno == ENOENT) {
                            continue;
                        }
                    }
                    break;
                }
            }
            break;
        default:
            break;
    }

    if (fd_ < 0) {
        throw file_error("Open file \"{}\" error ({})}",
            path.c_str(), std::strerror(errno));
    }
}

} /* namespace plusone */

#endif /* KSERGEY_file_120717222113 */
