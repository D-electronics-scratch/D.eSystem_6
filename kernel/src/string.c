#include <stddef.h>

char* itoa(int value, char* buffer, int base) {
    char* ptr = buffer;
    char* ptr1 = buffer;
    char tmp_char;
    int tmp_value;

    // Handle negative numbers
    if (value < 0 && base == 10) {
        value = -value;
        *ptr++ = '-';
        ptr1++;
    }

    // Convert number
    do {
        tmp_value = value;
        value /= base;
        *ptr++ = "0123456789ABCDEF"[tmp_value - value * base];
    } while (value);

    // Null-terminate
    *ptr-- = '\0';

    // Reverse string
    while (ptr1 < ptr) {
        tmp_char = *ptr;
        *ptr-- = *ptr1;
        *ptr1++ = tmp_char;
    }

    return buffer;
}

int strcmp(const char* s1, const char* s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

char *strcpy(char *dest, const char *src) {
    char *ret = dest;
    while (*src) {
        *dest++ = *src++;
    }
    *dest = '\0';
    return ret;
}

//void *memcpy(void *dest, const void *src, size_t n)
//{
//    unsigned char *d = (unsigned char *)dest;
//    const unsigned char *s = (const unsigned char *)src;

//    for (size_t i = 0; i < n; i++) {
//        d[i] = s[i];
//    }

//    return dest;
//}

//int memcmp(const void *s1, const void *s2, size_t n)
//{
//    const unsigned char *a = (const unsigned char *)s1;
//    const unsigned char *b = (const unsigned char *)s2;

//    for (size_t i = 0; i < n; i++) {
//        if (a[i] != b[i]) {
//            return (int)a[i] - (int)b[i];
//        }
//    }

//    return 0;
//}
