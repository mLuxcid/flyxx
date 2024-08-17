#include <hal/cpu.h>
#include <stdnoreturn.h>

noreturn void halt(void) {
    for (;;)
        asm ("hlt");
    __builtin_unreachable();
}
