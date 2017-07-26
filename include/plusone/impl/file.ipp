/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_file_120717222113
#define KSERGEY_file_120717222113

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <unistd.h>
#include <cerrno>
#include <cstring>
#include <cstdio>
#include <utility>
#include <plusone/compiler.hpp>

namespace plusone {

__force_inline file::file(file&& other) noexcept
{
    swap(other);
}

__force_inline file& file::operator=(file&& other) noexcept
{
    if (__likely(this != &other)) {
        swap(other);
    }
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

__force_inline file::operator bool() const noexcept
{
    return fd_.operator bool();
}

__force_inline bool file::operator!() const noexcept
{
    return fd_.operator!();
}

__force_inline const file_descriptor& file::descriptor() const noexcept
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

__force_inline void file::swap(file& v) noexcept
{
    fd_.swap(v.fd_);
}

__force_inline void file::lock()
{
    do_lock(LOCK_EX);
}

__force_inline bool file::try_lock()
{
    return do_try_lock(LOCK_EX);
}

__force_inline void file::unlock()
{
    auto rc = ::flock(fd_, LOCK_UN);
    if (__unlikely(rc == -1)) {
        throw file_error("Unlock file error ({})", std::strerror(errno));
    }
}

__force_inline void file::lock_shared()
{
    do_lock(LOCK_SH);
}

__force_inline bool file::try_lock_shared()
{
    return do_try_lock(LOCK_SH);
}

__force_inline void file::unlock_shared()
{
    unlock();
}

__force_inline file file::dup() const
{
    return file{descriptor().dup()};
}

__force_inline file file::temporary()
{
    FILE* temp_file = tmpfile();
    if (__unlikely(!temp_file)) {
        throw file_error("Failed to create temp file (tmpfile, {})", std::strerror(errno));
    }

    int fd = ::dup(::fileno(temp_file));
    ::fclose(temp_file);

    if (__unlikely(fd == -1)) {
        throw file_error("Failed to create temp file (dup, {})", std::strerror(errno));
    }

    return file(fd);
}

__force_inline file::file(file_descriptor fd)
    : fd_{std::move(fd)}
{}

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

    if (__unlikely(!fd_)) {
        throw file_error("Open file \"{}\" error ({})",
            path.c_str(), std::strerror(errno));
    }
}

__force_inline void file::do_lock(int op)
{
    auto rc = ::flock(fd_, op);
    if (__unlikely(rc == -1)) {
        throw file_error("Lock file error ({})", std::strerror(errno));
    }
}

__force_inline bool file::do_try_lock(int op)
{
    auto rc = ::flock(fd_, op | LOCK_NB);
    if (rc == -1) {
        if (__unlikely(errno != EWOULDBLOCK)) {
            throw file_error("Lock file error ({})", std::strerror(errno));
        } else {
            return false;
        }
    }
    return true;
}

} /* namespace plusone */

#endif /* KSERGEY_file_120717222113 */
