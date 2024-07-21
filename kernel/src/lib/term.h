#ifndef _LIB_TERM_H
#define _LIB_TERM_H

#include <stddef.h>

void term_init(void);
void term_write(const char *str, size_t length);

#endif /* _LIB_TERM_H */
