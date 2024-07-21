#include <limine.h>
#include <lib/term.h>
#include <stdint.h>
#include <sys/gdt.h>
#include <sys/idt.h>
#include <sys/cpu.h>
#include <vm/pmm.h>
#include <lib/printf.h>

// we want to use base revision 2, as it is the latest described base revsion
// by the limine boot protocol. Older revision are deprecated.
__attribute__((used, section(".requests")))
static volatile LIMINE_BASE_REVISION(2);

// limine requires us to place the requests in a specific section. It is also
// important that the requests do not get optimized away by the compiler.
// here we define the start & end markers for the limine requests
__attribute__((used, section(".requests_start_marker")))
static volatile LIMINE_REQUESTS_START_MARKER;

__attribute__((used, section(".requests_end_marker")))
static volatile LIMINE_REQUESTS_END_MARKER;

_Noreturn
void _start()
{
    if (!LIMINE_BASE_REVISION_SUPPORTED) {
        halt();
    }
    term_init();
    gdt_init();
    idt_init();
    vm_phys_init();

    halt();
}
