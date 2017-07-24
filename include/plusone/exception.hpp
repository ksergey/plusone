/*
 * Copyright (c) 2016-2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_exception_051216171739
#define KSERGEY_exception_051216171739

#include <string>
#include <exception>

namespace plusone {

/** Exception class */
class exception
    : public std::exception
{
private:
    const std::string what_;

public:
    /** Empty string constructor */
    exception() = default;

    /** Exception with message */
    explicit exception(std::string text);

    /** Exception with inplace message formatting */
    template< class... Args >
    exception(const char* format, const Args&... args);

    /** Destructor */
    virtual ~exception() noexcept = default;

    /** Return exception description */
    const char* what() const noexcept override;
};

/** Make unique exception class */
template< class... Tags >
struct tagged_exception
    : exception
{
    using exception::exception;
};

} /* namespace plusone */

#include <plusone/impl/exception.ipp>

#endif /* KSERGEY_exception_051216171739 */
