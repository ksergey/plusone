/*
 * Copyright (c) 2016-2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef MADLIFE_expect_111116215130_MADLIFE
#define MADLIFE_expect_111116215130_MADLIFE

#include <iostream>
#include <cstdlib>
#include "compiler.hpp"

namespace plusone {

/**
 * abort function for any assert-like macroses
 */
__force_inline void abort(const char* file, int line, const char* expr, const char* text = nullptr)
{
    std::cerr << "\n *** expression \"" << expr << "\" at \"" << file << ':' << line << "\" failed ***\n";
    if (text) {
        std::cerr << "   \"" << text << "\"\n";
    }
    std::cerr << '\n';
    std::abort();
}

} /* namespace plusone */

#if ! defined(expect)
#   if defined(NDEBUG)
#       define __expect(expr)
#       define __expectm(expr, text)
#   else
#       define __expect(expr) do { if (__unlikely(!(expr))) { plusone::abort(__FILE__, __LINE__, #expr); } } while (false)
#       define __expectm(expr, text) do { if (__unlikely(!(expr))) { plusone::abort(__FILE__, __LINE__, #expr, text); } } while (false)
#   endif /* defined(NDEBUG) */
#endif /* ! defined(expect) */

#endif /* MADLIFE_expect_111116215130_MADLIFE */
