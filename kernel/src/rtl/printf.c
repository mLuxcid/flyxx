#include <hal/term.h>
#include <rtl/printf.h>
#include <rtl/string.h>
#include <printf/printf.h>

void putchar_(char c) {
    (void)c;
}

int printf(const char *restrict format, ...) {
    va_list args;
    va_start(args, format);

    int ret = vprintf(format, args);

    va_end(args);
    return ret;
}

int vprintf(const char *restrict format, va_list args) {
    char buf[2048];
    int ret = vsnprintf(buf, sizeof(buf), format, args);
    term_write(buf);

    return ret;
}

int vsnprintf(char *buf, size_t length, const char *restrict format, va_list args) {
    return vsnprintf_(buf, length, format, args);
}
