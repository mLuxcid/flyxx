#ifndef _LIB_PRINTF_H
#define _LIB_PRINTF_H

#include <stdarg.h>
#include <stddef.h>

int printf(const char *restrict format, ...);
int vprintf(const char *restrict format, va_list args);
int vsnprintf(char *buf, size_t length, const char *restrict format, va_list args);

#endif /* _LIB_PRINTF_H */
