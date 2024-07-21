#ifndef _SYS_GDT_H
#define _SYS_GDT_H

#include <sys/cpu.h>

void gdt_init(void);
void gdt_reload(void);
void gdt_load_tss(struct tss *tss);

#endif /* _SYS_GDT_H */
