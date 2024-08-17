#include <hal/cpu.h>
#include <stdarg.h>
#include <hal/panic.h>
#include <rtl/printf.h>
#include <stdnoreturn.h>

noreturn void panic(struct cpu_context *context, const char *restrict format, ... ) {
    asm ("cli");
    printf("**** KERNEL PANIC ****\n");
 
    va_list args;
    va_start(args, format);

    vprintf(format, args);
    va_end(args);

    printf("\n");

    if (context == NULL) {
        goto cleanup;
    }

    printf("CPU context:\n");
    printf(
        "  RAX=%016lx  RBX=%016lx\n"
        "  RCX=%016lx  RDX=%016lx\n"
        "  RSI=%016lx  RDI=%016lx\n"
        "  RBP=%016lx  RSP=%016lx\n"
        "  R08=%016lx  R09=%016lx\n"
        "  R10=%016lx  R11=%016lx\n"
        "  R12=%016lx  R13=%016lx\n"
        "  R14=%016lx  R15=%016lx\n"
        "  RIP=%016lx  RFLAGS=%08lx\n"
        "  CS=%04lx DS=%04lx ES=%04lx SS=%04lx\n"
        "  ERR=%016lx\n",
        context->rax, context->rbx, context->rcx, context->rdx,
        context->rsi, context->rdi, context->rbp, context->rsp,
        context->r8, context->r9, context->r10, context->r11,
        context->r12, context->r13, context->r14, context->r15,
        context->rip, context->rflags,
        context->cs, context->ds, context->es, context->ss,
        context->err);

    goto cleanup;
cleanup:
    halt();
    __builtin_unreachable();
}
