#include "fat32.h"
#include "bpb.h"
#include "disk.h"
#include <stdint.h>
#include "string.h"

//uint32_t fat32_find_file(const char *name, fat32_bpb_t *bpb) {
//    return 0;
//}

//int fat32_read_file(uint32_t start_cluster, fat32_bpb_t *bpb, uint8_t *out) {
    
//    return 0;
//}


int fat32_read_file(
    fat32_file_t *file,
    fat32_bpb_t *bpb,
    uint8_t *out
) {
    if (file == NULL || bpb == NULL || out == NULL)
        return -1;

    if (file->start_cluster < 2)
        return -1;

    if (bpb->bytes_per_sector != 512)
        return -1;

    uint32_t cluster = file->start_cluster;
    uint32_t remaining = file->size;
    uint32_t out_offset = 0;

    uint32_t cluster_size =
        bpb->bytes_per_sector * bpb->sectors_per_cluster;

    uint8_t sector[512];

    while (remaining > 0) {

        uint32_t lba =
            fat32_cluster_to_lba(cluster, *bpb);

        uint32_t bytes_this_cluster = remaining;

        if (bytes_this_cluster > cluster_size)
            bytes_this_cluster = cluster_size;

        uint32_t cluster_read = 0;

        for (uint32_t s = 0;
             s < bpb->sectors_per_cluster &&
             cluster_read < bytes_this_cluster;
             s++) {

            if (read_sector(lba + s, sector) != 0)
                return -1;

            uint32_t bytes =
                bytes_this_cluster - cluster_read;

            if (bytes > bpb->bytes_per_sector)
                bytes = bpb->bytes_per_sector;

            /*
             * read_sector() liest einen kompletten Sektor.
             * Deshalb kopieren wir nur die tatsächlich benötigten Bytes.
             */
            for (uint32_t i = 0; i < bytes; i++) {
                out[out_offset + i] = sector[i];
            }

            out_offset += bytes;
            cluster_read += bytes;
        }

        remaining -= bytes_this_cluster;

        if (remaining == 0)
            break;

        uint32_t next =
            fat32_get_next_cluster(cluster, bpb);

        if (next == 0xFFFFFFFF)
            return -1;

        if (next >= 0x0FFFFFF8)
            return -1;

        cluster = next;
    }

    return 0;
}