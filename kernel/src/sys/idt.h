#ifndef _SYS_IDT_H
#define _SYS_IDT_H

#include <stdint.h>
void idt_init(void);
void idt_reload(void);

void idt_install_handler(uint8_t vector, void *handler, uint8_t flags);

void idt_set_ist(uint8_t vector, uint8_t ist);
uint8_t idt_get_ist(uint8_t vector);

void idt_set_flags(uint8_t vector, uint8_t flags);

#endif /* _SYS_IDT_H */
