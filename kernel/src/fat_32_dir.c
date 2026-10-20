#include "fat32.h"
#include "bpb.h"
#include "disk.h"
#include <stdint.h>
#include <string.h>
#include "video.h"
#include "main.h"

int fat32_find_file(
    const char *name,
    fat32_bpb_t *bpb,
    fat32_file_t *file
) {
    if (name == NULL || bpb == NULL || file == NULL)
        return 0;

    uint32_t cluster = bpb->root_cluster;

    uint8_t sector[512];

    while (1) {

        uint32_t lba =
            fat32_cluster_to_lba(cluster, *bpb);

      
            
        for (uint32_t s = 0;
             s < bpb->sectors_per_cluster;
             s++) {

            if (read_sector(lba + s, sector) != 0)
                return 0;

          
                
            for (uint32_t i = 0;
                 i < bpb->bytes_per_sector;
                 i += 32) {

                uint8_t *entry = sector + i;

              
                if (entry[0] == 0x00)
                    return 0;

              
                if (entry[0] == 0xE5)
                    continue;

              
                if (entry[11] == 0x0F)
                    continue;

                char entry_name[12];

                for (int j = 0; j < 11; j++)
                    entry_name[j] = entry[j];

                entry_name[11] = '\0';

                if (memcmp(entry_name, name, 11) == 0) {

                    uint16_t lo =
                        *(uint16_t *)(entry + 26);

                    uint16_t hi =
                        *(uint16_t *)(entry + 20);

                    file->start_cluster =
                        ((uint32_t)hi << 16) | lo;

                    file->size =
                        *(uint32_t *)(entry + 28);

                    return 1;
                }
            }
        }

        uint32_t next =
            fat32_get_next_cluster(cluster, bpb);

      
        if (next == 0xFFFFFFFF)
            return 0;


        if (next >= 0x0FFFFFF8)
            return 0;

        cluster = next;
    }
}




int fat32_list_directory(
    uint32_t cluster,
    fat32_bpb_t *bpb,
    fat32_dir_entry_t *entries,
    uint32_t max_entries,
    uint32_t *entry_count
) {
    if (bpb == NULL ||
        entries == NULL ||
        entry_count == NULL ||
        max_entries == 0)
        return -1;

    *entry_count = 0;

    uint8_t sector[512];

    while (1) {

        uint32_t lba =
            fat32_cluster_to_lba(cluster, *bpb);

        for (uint32_t s = 0;
             s < bpb->sectors_per_cluster;
             s++) {

            if (read_sector(lba + s, sector) != 0)
                return -1;

            for (uint32_t i = 0;
                 i < bpb->bytes_per_sector;
                 i += 32) {

                uint8_t *entry = sector + i;

                if (entry[0] == 0x00)
                    return 0;

                if (entry[0] == 0xE5)
                    continue;


                if (entry[11] == 0x0F)
                    continue;

                if (*entry_count >= max_entries)
                    return 0;

                fat32_dir_entry_t *out =
                    &entries[*entry_count];

                for (int j = 0; j < 11; j++)
                    out->name[j] = entry[j];

                out->name[11] = '\0';

            
                out->attributes = entry[11];

                uint16_t lo =
                    *(uint16_t *)(entry + 26);

                uint16_t hi =
                    *(uint16_t *)(entry + 20);

                out->start_cluster =
                    ((uint32_t)hi << 16) | lo;

               
                out->size =
                    *(uint32_t *)(entry + 28);

                (*entry_count)++;
            }
        }

       
        uint32_t next =
            fat32_get_next_cluster(cluster, bpb);

        if (next == 0xFFFFFFFF)
            return -1;

        if (next >= 0x0FFFFFF8)
            return 0;

        cluster = next;
    }
}



int fat32_create_file(
    uint32_t directory_cluster,
    const char *name83,
    fat32_bpb_t *bpb,
    const uint8_t *data,
    uint32_t size
) {
    if (name83 == NULL || bpb == NULL ||
        data == NULL || size == 0)
        return -1;

    if (bpb->bytes_per_sector != 512)
        return -2;

    uint32_t cluster_size =
        bpb->bytes_per_sector * bpb->sectors_per_cluster;

    if (size > cluster_size)
        return -3;

    uint32_t cluster = directory_cluster;
    uint32_t dir_lba = 0;
    uint32_t dir_offset = 0;
    uint8_t dir_sector[512];
    int found = 0;

    while (1) {
        uint32_t lba =
            fat32_cluster_to_lba(cluster, *bpb);

        for (uint32_t s = 0;
             s < bpb->sectors_per_cluster; s++) {

            if (read_sector(lba + s, dir_sector) != 0)
                return -4;

            for (uint32_t i = 0; i < 512; i += 32) {
                uint8_t *entry = dir_sector + i;

                if (entry[0] == 0x00 ||
                    entry[0] == 0xE5) {
                    dir_lba = lba + s;
                    dir_offset = i;
                    found = 1;
                    break;
                }
            }

            if (found)
                break;
        }

        if (found)
            break;

        uint32_t next =
            fat32_get_next_cluster(cluster, bpb);

        if (next == 0xFFFFFFFF ||
            next >= 0x0FFFFFF8)
            return -5;

        cluster = next;
    }

    uint32_t file_cluster;

    if (fat32_find_free_cluster(bpb, &file_cluster) != 0)
        return -6;

    if (fat32_set_fat_entry(
            bpb, file_cluster, 0x0FFFFFFF) != 0)
        return -7;

  
    uint8_t buffer[512] = {0};
    uint32_t file_lba =
        fat32_cluster_to_lba(file_cluster, *bpb);
    uint32_t copied = 0;

    for (uint32_t s = 0;
         s < bpb->sectors_per_cluster; s++) {

        for (uint32_t i = 0; i < 512; i++)
            buffer[i] = 0;

        uint32_t remaining = size - copied;
        uint32_t amount = remaining < 512 ? remaining : 512;

        if (amount > 0) {
            memcpy(buffer, data + copied, amount);
            copied += amount;
        }

        if (write_sector(file_lba + s, buffer) != 0) {
            fat32_set_fat_entry(bpb, file_cluster, 0);
            return -8;
        }
    }


    if (read_sector(dir_lba, dir_sector) != 0) {
        fat32_set_fat_entry(bpb, file_cluster, 0);
        return -9;
    }

    uint8_t *entry = dir_sector + dir_offset;

    for (uint32_t i = 0; i < 32; i++)
        entry[i] = 0;

    memcpy(entry, name83, 11);

    entry[11] = 0x20;
    entry[20] = (file_cluster >> 16) & 0xFF;
    entry[21] = (file_cluster >> 24) & 0xFF;
    entry[26] = file_cluster & 0xFF;
    entry[27] = (file_cluster >> 8) & 0xFF;
    entry[28] = size & 0xFF;
    entry[29] = (size >> 8) & 0xFF;
    entry[30] = (size >> 16) & 0xFF;
    entry[31] = (size >> 24) & 0xFF;

    if (write_sector(dir_lba, dir_sector) != 0) {
        fat32_set_fat_entry(bpb, file_cluster, 0);
        return -10;
    }

    return 0;
}
