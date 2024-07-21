#ifndef _VM_PMM_H
#define _VM_PMM_H

#define PAGE_SIZE 0x1000

void vm_phys_init(void);
void vm_slab_init(void);
void *vm_phys_alloc_page(void);
void vm_phys_free_page(void *page);


#endif /* _VM_PMM_H */
