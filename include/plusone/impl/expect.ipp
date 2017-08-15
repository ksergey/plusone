/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_expect_240717092911
#define KSERGEY_expect_240717092911

#include <iostream>
#include <exception>

namespace plusone {

/** Abort function for any assert-like macroses */
__force_inline void abort(const char* file, int line, const char* expr, const char* text = nullptr)
{
    std::cerr << "\n *** expression \"" << expr << "\" at \"" << file << ':' << line << "\" failed ***\n";
    if (text) {
        std::cerr << "   \"" << text << "\"\n";
    }
    std::cerr << '\n';
    std::terminate();
}

} /* namespace plusone */

#endif /* KSERGEY_expect_240717092911 */
