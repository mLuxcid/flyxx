#include <stddef.h>
#include <limine.h>
#include <stdint.h>
#include <mm/pmm.h>
#include <rtl/printf.h>
#include <hal/panic.h>

__attribute__((section(".requests"), used))
static volatile struct limine_hhdm_request hhdm_request = {
    .id = LIMINE_HHDM_REQUEST,
    .revision = 0,
};

__attribute__((section(".requests"), used))
static volatile struct limine_memmap_request memmap_request = {
    .id = LIMINE_MEMMAP_REQUEST,
    .revision = 0,
};

struct pmm_block {
    struct pmm_block *next;
};
static struct pmm_block *head = NULL;

void mm_pmm_init(void) {
    struct limine_memmap_entry **entries = memmap_request.response->entries;
    struct pmm_block *prev = NULL;
    uintptr_t first_usable = 0;
    uintptr_t last_usable = 0;
    bool in_usable_region = false;

    printf("usable memory:\n");
    for (size_t i = 0; i < memmap_request.response->entry_count; i++) {
        switch (entries[i]->type) {
        case LIMINE_MEMMAP_USABLE:
            if (!in_usable_region) {
                first_usable = entries[i]->base + hhdm_request.response->offset;
                in_usable_region = true;
            }
            for (size_t j = 0; j < entries[i]->length; j += PAGE_SIZE) {
                uintptr_t current_address = entries[i]->base + j + hhdm_request.response->offset;
                if (head == NULL) {
                    head = (void *)current_address;
                }
                if (prev != NULL) {
                    prev->next = (struct pmm_block *)current_address;
                }
                prev = (struct pmm_block *)current_address;
                last_usable = current_address + PAGE_SIZE - 1;
            }
            break;
        default:
            if (in_usable_region) {
                printf("  %p->%p\n",
                    (void *)first_usable, (void *)last_usable);
                in_usable_region = false;
            }
            break;
        }
    }

    if (in_usable_region) {
        printf("  %p->%p\n",
            (void *)first_usable, (void *)last_usable);
    }

    size_t cnt = 0;
    struct pmm_block *cur = head;
    while (cur != NULL) {
        cnt++;
        cur = cur->next;
    }

    printf("got a total of %zu free pages (%zu KiB).\n", cnt, cnt * 4096 / 1024);
}

void * mm_pmm_alloc_page(void) {
    if (head != NULL) {
        struct pmm_block *node = head;
        head = head->next;
        return (void *)((uintptr_t)node - hhdm_request.response->offset);
    } else {
        panic(NULL, "OOM error!");
    }
}

void mm_pmm_free_page(void *page) {
    struct pmm_block *block = (void *)((uintptr_t)page + hhdm_request.response->offset);
    block->next = head;
    head = block;
}
