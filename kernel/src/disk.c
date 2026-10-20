#include "main.h"
//#include <limine.h>
#include "disk.h"

//static volatile struct limine_disk_request disk_request = {
//    .id = LIMINE_DISK_REQUEST,
//    .revision = 0
//};
//EFI_BLOCK_IO_PROTOCOL *g_block_io = NULL;

int read_sector(uint32_t lba, uint8_t *buffer) {
    disk_read_t req = {
        .lba = lba,
        .count = 1,
        .buffer = buffer
    };

    //disk_request.read(&read);
   return disk_api.read(&req);
}