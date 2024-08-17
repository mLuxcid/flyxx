#pragma once

#include <stdbool.h>

typedef struct _RTL_SPINLOCK {
    bool Locked;
} RTL_SPINLOCK;
