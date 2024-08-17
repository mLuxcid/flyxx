#include <hal/cpu.h>
#include <stdint.h>
#include <hal/idt.h>
#include <rtl/array.h>
#include <stddef.h>
#include <hal/panic.h>

struct idt_entry {
    uint16_t offset_lo;
    uint16_t selector;
    uint8_t  ist;
    uint8_t  flags;
    uint16_t offset_mi;
    uint32_t offset_hi;
    uint32_t reserved;
};

struct idtr {
    uint16_t limit;
    uint64_t base;
} __attribute__((packed));

static struct idt_entry idt[256];
void *isr[256];

void hal_idt_install_handler(uint8_t vector, void *handler, uint8_t flags) {
    uintptr_t addr = (uintptr_t)handler;
    idt[vector] = (struct idt_entry) {
        .offset_lo = (uint16_t)addr & 0xFFFF,
        .selector = 0x28,
        .ist = 0,
        .flags = flags,
        .offset_mi = (uint16_t)((addr >> 16) & 0xFFFF),
        .offset_hi = (uint32_t)(addr >> 32),
        .reserved = 0,
    };
}

void hal_idt_set_ist(uint8_t vector, uint8_t ist) {
    idt[vector].ist = ist;
}

uint8_t hal_idt_get_ist(uint8_t vector) {
    return idt[vector].ist;
}

void hal_idt_set_flags(uint8_t vector, uint8_t flags) {
    idt[vector].flags = flags;
}

void hal_idt_reload(void) {
    struct idtr idtr = {
        .limit = sizeof(idt) - 1,
        .base = (uint64_t)&idt,
    };

    asm volatile ("lidt %0" : : "m" (idtr) : "memory");
}

extern void *isr_stubs[];

static void generic_isr(uint8_t vector, struct cpu_context *context) {
    panic(context, "ISR %u triggered!", vector);
    __builtin_unreachable();
}

static const char *exceptions[21] = {
    "Division by Zero",
    "Debug",
    "NMI",
    "Breakpoint",
    "Overflow",
    "Bound range exceeded",
    "Invalid opcode",
    "Device not available",
    "Double fault",
    "RESERVED",
    "Invalid TSS",
    "Segment not present",
    "Stack-segment fault",
    "General protection fault",
    "Page fault",
    "RESERVED",
    "x87",
    "Alignment check",
    "Machine check",
    "SIMD",
    "Virtualisation"
};

static void exception_handler(uint8_t vector, struct cpu_context *context) {
    panic(context, "Exception %u (%s) triggered!", vector, exceptions[vector]);
    __builtin_unreachable();
}

void hal_idt_init(void) {
    for (uint16_t i = 0; i < 256; i++) {
        if (isr_stubs[i] == NULL) {
            // this is where i implement HalCrash
            panic(NULL, "trying to install NULL Handler!");
        }

        hal_idt_install_handler(i, isr_stubs[i], 0x8e);
        isr[i] = generic_isr;
    }

    for (uint8_t i = 0; i < ARRAY_LEN(exceptions); i++) {
        isr[i] = exception_handler;
    }

    // GPF require a ist
    hal_idt_set_ist(0xe, 0x2);
    hal_idt_reload();
}
