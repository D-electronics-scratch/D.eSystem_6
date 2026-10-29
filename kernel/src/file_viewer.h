#pragma once

#include <stdint.h>
#include "bpb.h"

void fs_list_dir(const char *path);
int fs_read_file(const char *path, uint8_t *out);
void file_viewer_open();
