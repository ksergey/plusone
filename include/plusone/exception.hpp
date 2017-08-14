/*
 * Copyright (c) 2016-2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_exception_051216171739
#define KSERGEY_exception_051216171739

#include <string>
#include <exception>
#include <plusone/fmt.hpp>

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

/** Throw exception */
template< class Ex >
void throw_ex();

/** Throw exception */
template< class Ex, class... Args >
void throw_ex(fmt::CStringRef format, Args&&... args);

} /* namespace plusone */

#include <plusone/impl/exception.ipp>

#endif /* KSERGEY_exception_051216171739 */
