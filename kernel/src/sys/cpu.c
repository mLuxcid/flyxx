#include <sys/cpu.h>

_Noreturn void halt(void)
{
    for (;;)
        asm ("hlt");
    __builtin_unreachable();
}
