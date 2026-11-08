#include "fat32.h"
#include <stddef.h>
#include "disk.h"
#include <string.h>

uint32_t fat32_get_fat_start(fat32_bpb_t bpb) {
    return bpb.reserved_sector_count;
}

uint32_t fat32_get_data_start(fat32_bpb_t bpb) {
    return fat32_get_fat_start(bpb) + (bpb.num_fats * bpb.fat_size_32);
}

uint32_t fat32_cluster_to_lba(uint32_t cluster, fat32_bpb_t bpb) {
    return fat32_get_data_start(bpb) + ((cluster - 2) * bpb.sectors_per_cluster);
}


int fat32_is_directory(fat32_dir_entry_t *entry)
{
    if (entry == NULL)
        return 0;

    return (entry->attributes & 0x10) != 0;
}

int fat32_find_file_in_directory(
    uint32_t directory_cluster,
    const char *name,
    fat32_bpb_t *bpb,
    fat32_file_t *file
) {
    if (name == NULL || bpb == NULL || file == NULL)
        return 0;

    if (directory_cluster < 2)
        return 0;

    uint32_t cluster = directory_cluster;
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




//int fat32_find_free_cluster(fat32_bpb_t * bpb, uint32_t * cluster) {
//
//    if (bpb == NULL || cluster == NULL)
//    return -1;
//
//    uint32_t fat_start = fat32_get_fat_start(*bpb);
//
//    uint32_t total_fat_entries = (bpb->fat_size_32 * bpb->bytes_per_sector) / 4;

//    uint8_t sector[512];
//  for (uint32_t c = 2;
//         c < total_fat_entries;
//        c++) {

//        uint32_t offset = c * 4;

//        uint32_t sector_number =
//            fat_start +
//            (offset / bpb->bytes_per_sector);

//        uint32_t sector_offset =
//            offset % bpb->bytes_per_sector;

//        if (read_sector(sector_number, sector) != 0)
//            return -1;

//        uint32_t value =
//            *(uint32_t *)(sector + sector_offset);

//        value &= 0x0FFFFFFF;

//        if (value == 0) {
//            *cluster = c;
//            return 0;
//        }
//    }

//    return -1;

//}

int fat32_find_free_cluster(
    fat32_bpb_t *bpb,
    uint32_t *cluster
) {
    if (bpb == NULL || cluster == NULL)
        return -1;

    if (bpb->bytes_per_sector != 512 ||
        bpb->sectors_per_cluster == 0 ||
        bpb->fat_size_32 == 0 ||
        bpb->num_fats == 0)
        return -1;

    uint32_t fat_start =
        fat32_get_fat_start(*bpb);

    uint32_t data_start =
        fat32_get_data_start(*bpb);

    uint32_t fat_entries =
        (bpb->fat_size_32 * bpb->bytes_per_sector) / 4;


    uint8_t sector[512];

    for (uint32_t c = 2; c < fat_entries; c++) {
        uint32_t offset = c * 4;

        uint32_t lba =
            fat_start + offset / bpb->bytes_per_sector;

        uint32_t sector_offset =
            offset % bpb->bytes_per_sector;

        if (read_sector(lba, sector) != 0)
            return -1;

        uint32_t value;
        memcpy(&value, sector + sector_offset,
               sizeof(value));

        value &= 0x0FFFFFFF;

        if (value == 0) {
            uint32_t cluster_lba =
                fat32_cluster_to_lba(c, *bpb);

            if (cluster_lba < data_start)
                return -1;

            *cluster = c;
            return 0;
        }
    }

    return -1;
}


int fat32_set_fat_entry(
    fat32_bpb_t *bpb,
    uint32_t cluster,
    uint32_t value
) {
    if (bpb == NULL)
        return -1;

    if (bpb->bytes_per_sector != 512 ||
        bpb->fat_size_32 == 0 ||
        bpb->num_fats == 0)
        return -1;

    uint32_t fat_entries =
        (bpb->fat_size_32 * bpb->bytes_per_sector) / 4;

    if (cluster < 2 || cluster >= fat_entries)
        return -1;

    uint32_t offset = cluster * 4;
    uint32_t sector_offset =
        offset % bpb->bytes_per_sector;

    uint32_t fat_start =
        fat32_get_fat_start(*bpb);

    uint32_t sector_number =
        fat_start + offset / bpb->bytes_per_sector;

    uint8_t sector[512];

    for (uint32_t fat = 0; fat < bpb->num_fats; fat++) {
        uint32_t lba =
            sector_number + fat * bpb->fat_size_32;

        if (read_sector(lba, sector) != 0)
            return -1;

        uint32_t old_value;
        memcpy(&old_value, sector + sector_offset, 4);

        uint32_t new_value =
            (old_value & 0xF0000000) |
            (value & 0x0FFFFFFF);

        memcpy(sector + sector_offset, &new_value, 4);

        if (write_sector(lba, sector) != 0)
            return -1;
    }

    return 0;
}