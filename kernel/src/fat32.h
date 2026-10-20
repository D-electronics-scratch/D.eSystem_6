#ifndef FAT32_H
#define FAT32_H

#include "bpb.h"
#include <stdint.h>

typedef struct {
    uint32_t start_cluster;
    uint32_t size;
} fat32_file_t;

uint32_t fat32_get_fat_start(fat32_bpb_t bpb);

uint32_t fat32_get_data_start(fat32_bpb_t bpb);

uint32_t fat32_cluster_to_lba(uint32_t cluster, fat32_bpb_t bpb);



uint32_t fat32_get_next_cluster(
    uint32_t cluster,
    fat32_bpb_t *bpb
);

int fat32_find_file(
    const char *name,
    fat32_bpb_t *bpb,
    fat32_file_t *file
);

int fat32_read_file(
    fat32_file_t *file,
    fat32_bpb_t *bpb,
    //uint32_t start_cluster,
    //fat32_bpb_t *bpb,
    uint8_t *out
);




typedef struct {
    char name[12];
    uint32_t start_cluster;
    uint32_t size;
    uint8_t attributes;
} fat32_dir_entry_t;

int fat32_list_directory(
    uint32_t cluster,
    fat32_bpb_t *bpb,
    fat32_dir_entry_t *entries,
    uint32_t max_entries,
    uint32_t *entry_count
);

int fat32_is_directory(fat32_dir_entry_t *entry);

int fat32_find_file_in_directory(
    uint32_t directory_cluster,
    const char *name,
    fat32_bpb_t *bpb,
    fat32_file_t *file
);

int fat32_find_free_cluster(fat32_bpb_t *bpb, uint32_t * cluster);

int fat32_set_fat_entry(fat32_bpb_t *bpb, uint32_t cluster, uint32_t value);

int fat32_create_file(uint32_t directory_cluster, const char *name83, fat32_bpb_t *bpb, const uint8_t *data, uint32_t size);

#endif 