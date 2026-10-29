#ifndef BPB_H
#define BPB_H
#include "main.h"
#include "keyboard.h"
#include <stdint.h>

typedef struct {
    uint16_t bytes_per_sector; //bytes per sector
    uint8_t sectors_per_cluster; //sectors per cluster
    uint16_t reserved_sector_count; //reserved sectors
    uint8_t num_fats; // how many fats?
    uint32_t fat_size_32; //fat size
    uint32_t root_cluster; //root cluster
} fat32_bpb_t;

extern fat32_bpb_t g_bpb;
fat32_bpb_t fat32_read_bpb();




#endif