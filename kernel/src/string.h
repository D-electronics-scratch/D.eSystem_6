#pragma once
#include <stddef.h>
//#include "bpb.h"
//#include "string.h"
#include <stdint.h>

char* itoa(int value, char* buffer, int base);

int strcmp(const char* s1, const char* s2);

char *strcpy(char *dest, const char *src);


void *memcpy(void *dest, const void *src, size_t n);

int memcmp(const void *s1, const void *s2, size_t n);
