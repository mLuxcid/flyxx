#pragma once

#include "rtl/printf.h"
#include <stdbool.h>
#include <stdint.h>
enum pfn_type {
    PFN_FREED,
    PFN_ZEROED,
    PFN_RESERVED,
    PFN_ACTIVE,
    PFN_PAGETABLE
};

struct pfn_entry {
    struct pfn_entry *next;
    enum pfn_type type;
    uint32_t refs;
    bool swappable;
    size_t pte;
};

void pfn_init(void);
void *pfn_alloc(void);
void pfn_free(void *page);
