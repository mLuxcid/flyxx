#pragma once

#include <stddef.h>

void* memcpy (void *, const void *, size_t);
void* memset (void *, int, size_t);
void* memmove (void *, const void *, size_t);
int memcmp (const void *, const void *, size_t);

char* strdup (const char *);
char* strcpy (char *, const char *);
char* strncpy (char *, const char *, size_t);

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wincompatible-library-redeclaration"
#endif

// it was easier to do const :3
const char *strstr (const char *s1, const char *s2);

#ifdef __clang__
#pragma clang diagnostic pop
#endif

int strcmp (const char *, const char *);
int strncmp (const char *, const char *, size_t);
size_t strlen (const char *);
char *strrchr (const char *str, int ch);
