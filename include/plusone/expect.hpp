/*
 * Copyright (c) 2016-2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef MADLIFE_expect_111116215130_MADLIFE
#define MADLIFE_expect_111116215130_MADLIFE

#include <plusone/compiler.hpp>

#if ! defined(expect)
#   if defined(NDEBUG)
#       define __expect(expr)
#       define __expectm(expr, text)
#   else
#       define __expect(expr) do { if (__unlikely(!(expr))) { plusone::abort(__FILE__, __LINE__, #expr); } } while (false)
#       define __expectm(expr, text) do { if (__unlikely(!(expr))) { plusone::abort(__FILE__, __LINE__, #expr, text); } } while (false)
#   endif /* defined(NDEBUG) */
#endif /* ! defined(expect) */

#include <plusone/impl/expect.ipp>

#endif /* MADLIFE_expect_111116215130_MADLIFE */
