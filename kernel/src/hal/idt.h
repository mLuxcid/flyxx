#ifndef _SYS_IDT_H
#define _SYS_IDT_H

#include <stdint.h>

void hal_idt_init(void);
void hal_idt_reload(void);

void hal_idt_install_handler(uint8_t vector, void *handler, uint8_t flags);

void hal_idt_set_ist(uint8_t vector, uint8_t ist);
uint8_t hal_idt_get_ist(uint8_t vector);

void hal_idt_set_flags(uint8_t vector, uint8_t flags);

#endif /* _SYS_IDT_H */
