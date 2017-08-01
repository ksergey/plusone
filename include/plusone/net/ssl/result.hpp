/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_result_020817020344
#define KSERGEY_result_020817020344

namespace plusone {
namespace net {
namespace ssl {

/** SSL stream error code */
class error_code
{
private:
    unsigned long code_{0};

public:
    error_code() = default;

    /** Construct error code */
    error_code(unsigned long code) noexcept;

    /** Get error code */
    unsigned long code() const noexcept;

    /** Return error code description */
    const char* str() const noexcept;
};

/** SSL stream handshake operation result */
class handshake_result
    : public error_code
{
private:
    int value_{0};

public:
    handshake_result() = default;

    /** Construct result */
    handshake_result(int value);

    /** Return true if operation successfully executed */
    explicit operator bool() const noexcept;
};

/** SSL stream operation result */
class op_result
    : public error_code
{
private:
    int value_{0};

public:
    op_result() = default;

    /** Construct result */
    op_result(int value);

    /** Return true if operation successfully executed */
    explicit operator bool() const noexcept;

    /** Return operation bytes processed */
    std::size_t bytes() const noexcept;
};

} /* namespace ssl */
} /* namespace net */
} /* namespace plusone */

#include <plusone/net/ssl/impl/result.ipp>

#endif /* KSERGEY_result_020817020344 */
