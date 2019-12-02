#include "error.h"


/* Globals */
unsigned int x_debug_level; /* GLOBAL */


void
x_msg(const unsigned int flags, const char *format, ...)
{
    va_list arglist;
    va_start(arglist, format);
    x_msg_va(flags, format, arglist);
    va_end(arglist);
}
