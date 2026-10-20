#include "fat32_utils.h"
#include <stdint.h>
#include <stddef.h>

//int fat32_make_83_name(
//    const char *input,
//    char *output
//);

int fat32_make_83_name(const char *input, char *output)
{
    if (input == NULL || output == NULL) {
        return -1;
    }

    int length = 0;

    while (input[length] != '\0') {
        length++;
    }

    int dot = -1;

    for (int i = 0; i < length; i++) {
        if (input[i] == '.') {
            dot = i;
            break;
        }
    }

    if (dot == -1) {
        return -1;
    }

    int name_length = dot;
    int extension_length = length - dot - 1;

    if (name_length < 1 || name_length > 8) {
        return -1;
    }

    if (extension_length > 3) {
        return -1;
    }

    for (int i = 0; i < name_length; i++) {
        output[i] = input[i];
    }

    for (int i = name_length; i < 8; i++) {
        output[i] = ' ';
    }

    for (int i = 0; i < extension_length; i++) {
        output[8 + i] = input[dot + 1 + i];
    }

    output[11] = '\0';

    return 0;
}

int fat32_format_83_name(
    const char *input,
    char *output
) {
    if (input == NULL || output == NULL)
        return -1;

    int name_end = 7;

    // Leerzeichen am Ende des Dateinamens entfernen
    while (name_end >= 0 && input[name_end] == ' ')
        name_end--;

    int ext_end = 10;

    // Leerzeichen am Ende der Extension entfernen
    while (ext_end >= 8 && input[ext_end] == ' ')
        ext_end--;

    int out = 0;

    // Dateiname kopieren
    for (int i = 0; i <= name_end; i++) {
        output[out++] = input[i];
    }

    // Extension vorhanden?
    if (ext_end >= 8) {
        output[out++] = '.';

        for (int i = 8; i <= ext_end; i++) {
            output[out++] = input[i];
        }
    }

    output[out] = '\0';

    return 0;
}