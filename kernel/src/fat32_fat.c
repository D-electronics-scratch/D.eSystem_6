#include "fat32.h"
#include "disk.h"
#include "bpb.h"
#include <stdint.h>

uint32_t fat32_get_next_cluster(
    uint32_t cluster,
    fat32_bpb_t * bpb 
) {

    if(bpb == NULL) {
        return 0xFFFFFFFF;
    }

    uint32_t fat_start = fat32_get_fat_start(* bpb);

    uint32_t offset = cluster * 4;

    uint32_t sector = fat_start + (offset / bpb->bytes_per_sector);

    uint32_t sector_offset = offset % bpb->bytes_per_sector;

    uint8_t buffer[512];

    if (read_sector(sector , buffer) != 0) return 0xFFFFFFFF;

    uint32_t next = *(uint32_t *)(buffer + sector_offset);

    return next & 0x0FFFFFFF;



//get next cluseter

}