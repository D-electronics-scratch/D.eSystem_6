#ifndef FAT32_UTILS_H
#define FAT32_UTILS_H


int fat32_make_83_name(
    const char *input,
    char *output
);

int fat32_format_83_name(
    const char *input,
    char *output
);

#endif