#include <stdint.h>
#include <hal/gdt.h>

struct gdt_descriptor  {
    uint16_t limit;
    uint16_t base_lo;
    uint8_t  base_mi;
    uint8_t  access;
    uint8_t  granularity;
    uint8_t  base_hi;
};

struct tss_descriptor {
    uint16_t len;
    uint16_t base_lo;
    uint8_t  base_milo;
    uint8_t  flags_lo;
    uint8_t  flags_hi;
    uint8_t  base_mihi;
    uint32_t base_hi;
    uint32_t reserved;
};

struct gdt {
    struct gdt_descriptor descriptors[11];
    struct tss_descriptor tss_descriptor;
};

struct gdtr {
    uint16_t Limit;
    uint64_t base;
} __attribute__((packed));

static struct gdt gdt;
static struct gdtr gdtr;

void hal_gdt_init(void) {
    // NULL descriptor
    gdt.descriptors[0] = (struct gdt_descriptor) {
        .limit = 0,
        .base_lo = 0,
        .base_mi = 0,
        .access = 0,
        .granularity = 0,
        .base_hi = 0,
    };

    // kernel 16-bit code
    gdt.descriptors[1] = (struct gdt_descriptor) {
        .limit = 0xFFFF,
        .base_lo = 0,
        .base_mi = 0,
        .access = 0b10011010,
        .granularity = 0,
        .base_hi = 0
    };

    // kernel 16-bit data
    gdt.descriptors[2] = (struct gdt_descriptor) {
        .limit = 0xFFFF,
        .base_lo = 0,
        .base_mi = 0,
        .access = 0b10010010,
        .granularity = 0,
        .base_hi = 0
    };

    // kernel 32-bit code
    gdt.descriptors[3] = (struct gdt_descriptor) {
        .limit = 0xFFFF,
        .base_lo = 0,
        .base_mi = 0,
        .access = 0b10011010,
        .granularity = 0b11001111,
        .base_hi = 0
    };

    // kernel 32-bit data
    gdt.descriptors[4] = (struct gdt_descriptor) {
        .limit = 0xFFFF,
        .base_lo = 0,
        .base_mi = 0,
        .access = 0b10010010,
        .granularity = 0b11001111,
        .base_hi = 0
    };

    // Kernel 64-bit Code
    gdt.descriptors[5] = (struct gdt_descriptor) {
        .limit = 0,
        .base_lo = 0,
        .base_mi = 0,
        .access = 0b10011010,
        .granularity = 0b00100000,
        .base_hi = 0,
    };

    // Kernel 64-bit Data
    gdt.descriptors[6] = (struct gdt_descriptor) {
        .limit = 0,
        .base_lo = 0,
        .base_mi = 0,
        .access = 0b10010010,
        .granularity = 0,
        .base_hi = 0,
    };

    // SYSENTER entries
    gdt.descriptors[7] = (struct gdt_descriptor){0};
    gdt.descriptors[8] = (struct gdt_descriptor){0};

    // User 64-bit Code
    gdt.descriptors[9] = (struct gdt_descriptor) {
        .limit = 0,
        .base_lo = 0,
        .base_mi = 0,
        .access = 0b11111010,
        .granularity = 0b00100000,
        .base_hi = 0,
    };

    // User 64-bit Data
    gdt.descriptors[10] = (struct gdt_descriptor) {
        .limit = 0,
        .base_lo = 0,
        .base_mi = 0,
        .access = 0b11110010,
        .granularity = 0,
        .base_hi = 0,
    };

    gdt.tss_descriptor = (struct tss_descriptor) {
        .len = 0x68,
        .base_lo = 0,
        .base_milo = 0,
        .flags_lo = 0x89,
        .flags_hi = 0,
        .base_mihi = 0,
        .base_hi = 0,
        .reserved = 0,
    };

    gdtr.Limit = sizeof(struct gdt) - 1;
    gdtr.base = (uint64_t)&gdt;

    hal_gdt_reload();
}

void hal_gdt_reload(void) {
    asm volatile (
        "lgdt %0\n\t"
        "push $0x28\n\t"
        "lea 1f(%%rip), %%rax\n\t"
        "push %%rax\n\t"
        "lretq\n\t"
        "1:\n\t"
        "mov $0x30, %%eax\n\t"
        "mov %%eax, %%ds\n\t"
        "mov %%eax, %%es\n\t"
        "mov %%eax, %%fs\n\t"
        "mov %%eax, %%gs\n\t"
        "mov %%eax, %%ss\n\t"
        :
        : "m"(gdtr)
        : "rax", "memory");
}

void
hal_gdt_load_tss(struct tss *tss) {
    // TODO: locking
    uintptr_t addr = (uintptr_t)tss;
    gdt.tss_descriptor = (struct tss_descriptor) {
        .base_lo = (uint16_t)addr,
        .base_milo = (uint8_t)(addr >> 16),
        .flags_lo = 0x89,
        .flags_hi = 0,
        .base_mihi = (uint8_t)(addr >> 24),
        .base_hi = (uint32_t)(addr >> 32),
        .reserved = 0,
    };

    asm volatile ("ltr %0" : : "rm" ((uint16_t)0x58) : "memory");
}
