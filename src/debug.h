
#include <stdio.h>

#define DEBUG 1

__fortify_function int
_debug_print(const char *__restrict __fmt, ...)
{
#if (DEBUG)
    return __printf_chk(__USE_FORTIFY_LEVEL - 1, __fmt, __va_arg_pack());
#else
    return 0;
#endif
}
