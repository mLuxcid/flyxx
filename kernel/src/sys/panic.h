#ifndef _SYS_PANIC_H
#define _SYS_PANIC_H

#include <sys/cpu.h>

_Noreturn void panic(struct cpu_context *context, const char *restrict format, ...);

#endif /* _SYS_PANIC_H */
