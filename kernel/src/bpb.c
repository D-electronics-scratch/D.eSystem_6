#include "disk.h"
#include "bpb.h"
#include <stdint.h>
#include "string.h"

fat32_bpb_t g_bpb;


fat32_bpb_t fat32_read_bpb(void)
{
    uint8_t sector[512] = {0};
    fat32_bpb_t bpb = {0};

    if (read_sector(0, sector) != 0)
        return bpb;

    memcpy(&bpb.bytes_per_sector,      sector + 11, 2);
    memcpy(&bpb.sectors_per_cluster,   sector + 13, 1);
    memcpy(&bpb.reserved_sector_count, sector + 14, 2);
    memcpy(&bpb.num_fats,              sector + 16, 1);
    memcpy(&bpb.fat_size_32,           sector + 36, 4);
    memcpy(&bpb.root_cluster,          sector + 44, 4);

    g_bpb = bpb;
    return bpb;
}


// D.electronics think smart!!
