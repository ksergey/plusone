/*
 * Copyright (c) 2016-2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef MADLIFE_compiler_051116230444_MADLIFE
#define MADLIFE_compiler_051116230444_MADLIFE

#include <cstddef>

/*
 * XXX(ksergey):
 * Double underscore reserved for compiler needs, so
 * need a new naming for compiler stuff
 */

#ifndef __force_inline
#   define __force_inline inline __attribute__((always_inline))
#endif

#ifndef __no_inline
#   define __no_inline __attribute__((noinline))
#endif

#ifndef __packed
#   define __packed __attribute__((packed))
#endif

#ifndef __aligned
#   define __aligned(amt) __attribute__((aligned(amt)))
#endif

#ifndef __likely
#   define __likely(x) __builtin_expect(static_cast< bool >(x), true)
#endif

#ifndef __unlikely
#   define __unlikely(x) __builtin_expect(static_cast< bool >(x), false)
#endif

namespace plusone {

static constexpr std::size_t false_sharing_range = 128;

/**
 * Suspend compiler variable unused warnings.
 * Function usable in cases when a variable could be unsued in case of
 * Debug/Release mode.
 */
template< class... Args >
void ignore_unused(Args&&...)
{}

} /* namespace plusone */

#endif /* MADLIFE_compiler_051116230444_MADLIFE */
