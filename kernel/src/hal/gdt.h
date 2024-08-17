#ifndef _SYS_GDT_H
#define _SYS_GDT_H

#include <hal/cpu.h>

void hal_gdt_init(void);
void hal_gdt_reload(void);
void hal_gdt_load_tss(struct tss *tss);

#endif /* _SYS_GDT_H */
