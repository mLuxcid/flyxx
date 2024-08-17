#include <rtl/printf.h>
#include <hal/term.h>
#include <limine.h>
#include <stdint.h>
#include <hal/cpu.h>
#include <hal/gdt.h>
#include <hal/idt.h>
#include <mm/pmm.h>

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

void kentry(void) {
    if (!LIMINE_BASE_REVISION_SUPPORTED) {
        halt();
    }
    term_init();
    printf("Copyright (C) mLuxcid 2024\nFlyxx version 0.0.1\n");

    // TODO: generic Hal Init thing?
    hal_gdt_init();
    hal_idt_init();
    mm_pmm_init();

    halt();
}
