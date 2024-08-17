#pragma once

#define PAGE_SIZE 0x1000

void mm_pmm_init(void);
void mm_slab_init(void);
void *mm_pmm_alloc_page(void);
void mm_pmm_free_page(void *Page);
